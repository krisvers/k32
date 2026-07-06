#pragma once

#include <cstdint>
#include <stdexcept>

#include "memory.hpp"
#include "device.hpp"
#include "gpu.hpp"

namespace k32 {

enum class CPUInterruptID : uint32_t {
    None = 0,
    DivideByZero = 1,
    MemoryFault = 2,
    InvalidPermissions = 3,
    InvalidInstruction = 4,

    DeviceMessage = 32,
};

class CPUFault : public std::runtime_error {
private:
    CPUInterruptID _id;

public:
    CPUFault(CPUInterruptID id, const char* message) : std::runtime_error(message), _id(id) {}

    CPUInterruptID id() const {
        return _id;
    }
};

template<typename T>
class CPURegister {
private:
    T _value;
    bool _readAllowed;
    bool _writeAllowed;

public:
    CPURegister() = default;
    CPURegister(T const& value) : _value(value), _readAllowed(false), _writeAllowed(false) {}

    CPURegister<T>& operator=(T const& value) {
        if (!_writeAllowed) {
            throw CPUFault(CPUInterruptID::InvalidPermissions, "Register is not writable");
        }

        _value = value;
        return *this;
    }

    operator T() const {
        if (!_readAllowed) {
            throw CPUFault(CPUInterruptID::InvalidPermissions, "Register is not readable");
        }

        return _value;
    }

    void setReadable(bool readable) {
        _readAllowed = readable;
    }

    bool getReadable() {
        return _readAllowed;
    }

    void setWritable(bool writable) {
        _writeAllowed = writable;
    }

    bool getWritable() {
        return _writeAllowed;
    }

    T& raw() {
        return _value;
    }
};

struct CPUStatus {
    bool zero;
    bool carry;
    bool overflow;
    bool sign;
    bool system;
    bool interrupt;

    CPUStatus() = default;

    CPUStatus(CPUStatus const& other) {
        zero = other.zero;
        carry = other.carry;
        overflow = other.overflow;
        sign = other.sign;
        system = other.system;
        interrupt = other.interrupt;
    }

    CPUStatus& operator=(CPUStatus const& other) {
        zero = other.zero;
        carry = other.carry;
        overflow = other.overflow;
        sign = other.sign;
        system = other.system;
        interrupt = other.interrupt;
        return *this;
    }
};

class CPURegisters {
private:
    /* general-purpose */
    CPURegister<uint32_t> _r[32];
    CPURegister<float> _f[8];

    /* special system-only */
    CPURegister<MemoryAddress> _p[4];
    CPURegister<MemoryAddress> _mmtable;
    CPURegister<MemoryAddress> _inthandler;
    CPURegister<uint32_t> _level;

    /* special system and user level */
    CPURegister<uint32_t> _interrupt;

    /* hidden */
    MemoryAddress _ip;
    CPUStatus _status;

    /* implementation detail */
    MemoryAddress _interruptOriginIP;
    CPUStatus _interruptOriginStatus;

    void updateAccessPermissions() {
        for (auto& r : _r) {
            r.setReadable(true);
            r.setWritable(true);
        }

        for (auto& f : _f) {
            f.setReadable(true);
            f.setWritable(true);
        }

        for (auto& p : _p) {
            p.setReadable(status().system);
            p.setWritable(status().system);
        }

        _mmtable.setReadable(status().system);
        _mmtable.setWritable(status().system);

        _inthandler.setReadable(status().system);
        _inthandler.setWritable(status().system);

        _level.setReadable(status().system);
        _level.setWritable(status().system);

        _interrupt.setReadable(status().system);
        _interrupt.setWritable(true);
    }

public:
    uint32_t r(uint32_t id) {
        if (id >= 32) {
            throw std::runtime_error("Integer register ID out of range");
        }

        return _r[id];
    }

    void r(uint32_t id, uint32_t value) {
        if (id >= 32) {
            throw std::runtime_error("Integer register ID out of range");
        }

        _r[id] = value;
    }

    float f(uint32_t id) {
        if (id >= 8) {
            throw std::runtime_error("Float register ID out of range");
        }

        return _f[id];
    }

    void f(uint32_t id, float value) {
        if (id >= 8) {
            throw std::runtime_error("Float register ID out of range");
        }

        _f[id] = value;
    }

    uint32_t p(uint32_t id) {
        if (id >= 4) {
            throw std::runtime_error("Pointer register ID out of range");
        }

        return _p[id];
    }

    void p(uint32_t id, uint32_t value) {
        if (id >= 4) {
            throw std::runtime_error("Pointer register ID out of range");
        }

        _p[id] = value;
    }

    MemoryAddress mmtable() {
        return _mmtable;
    }

    void mmtable(MemoryAddress mmtable) {
        _mmtable = mmtable;
    }

    MemoryAddress inthandler() {
        return _inthandler;
    }

    void inthandler(MemoryAddress inthandler) {
        _inthandler = inthandler;
    }

    uint32_t level() {
        return _level;
    }

    void level(uint32_t level) {
        _level = level;
    }

    uint32_t interrupt() {
        return _interrupt;
    }

    void interrupt(uint32_t interrupt) {
        _interrupt = interrupt;

        if (status().interrupt) {
            if (interrupt == 0) {
                _ip = _interruptOriginIP;
                status(_interruptOriginStatus);
            }
        } else if (interrupt != 0) {
            if (_inthandler != 0) {
                _interruptOriginIP = _ip;
                _interruptOriginStatus = _status;

                _ip = _inthandler;

                CPUStatus newStatus = _status;
                newStatus.system = true;
                newStatus.interrupt = true;
                status(newStatus);
            }
        }
    }

    MemoryAddress& ip() {
        return _ip;
    }

    CPUStatus status() {
        return _status;
    }

    void status(CPUStatus const& status) {
        _status = status;
        updateAccessPermissions();
    }
};

using CPUInstructionWord = uint32_t;

class CPU : public IDevice {
private:
    CPURegisters _registers;
    BufferMemory _systemMemory;
    MemoryGroup _mmu;

    GPU* _gpu;

    static const MemoryAddress GPU_SHARED_MEMORY_MAPPED_ADDRESS = 0xff000000;

public:
    CPU(MemoryExtent systemMemoryExtent) : _systemMemory(systemMemoryExtent) {
        _mmu.mapDevice(0x00000000, &_systemMemory);
    }

    ~CPU() {

    }

    bool addChildDevice(IDevice* device) {
        if (device->getTypeUUID() == GPU::uuid()) {
            _gpu = dynamic_cast<GPU*>(device);
            IMemoryDevice* gpuMemory = device->getExternalMemoryDevice();
            if (gpuMemory == nullptr) {
                throw std::runtime_error("GPU has no mappable memory");
            }

            if (!_mmu.mapDevice(GPU_SHARED_MEMORY_MAPPED_ADDRESS, gpuMemory)) {
                throw std::runtime_error("Failed to map GPU memory to MMU");
            }

            return true;
        }

        return false;
    }

    IMemoryDevice* getExternalMemoryDevice() override {
        return nullptr;
    }

    UUID getTypeUUID() const override {
        return uuid();
    }

    static UUID uuid() {
        return UUID("524c3600-02c2-4414-8273-d5ac484bdad7");
    }
};

}
