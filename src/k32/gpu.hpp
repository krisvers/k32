#pragma once

#include "uuid.hpp"
#include "memory.hpp"
#include "device.hpp"

namespace k32 {

enum class GPUStatusFlags : uint32_t {
    None = 0x00000000,
};

struct alignas(4) GPURegisters {
    uint32_t status;

    GPURegisters() = default;
};

class GPU : public IDevice {
private:
    BufferMemory _privateMemory;
    RegisterMemory<GPURegisters> _registerMemory;
    BufferMemory _sharedMemory;

    MemoryGroup _internalMMU;
    MemoryGroup _externalMMU;

    static const MemoryAddress INTERNAL_PRIVATE_MEMORY_ADDRESS = 0x00000000;
    static const MemoryAddress INTERNAL_SHARED_MEMORY_ADDRESS = 0x80000000;

    static const MemoryAddress EXTERNAL_REGISTER_MEMORY_ADDRESS = 0x00000000;
    static const MemoryAddress EXTERNAL_SHARED_MEMORY_ADDRESS = 0x00001000;

public:
    GPU(MemoryExtent privateMemoryExtent, MemoryExtent sharedMemoryExtent) : _privateMemory(privateMemoryExtent), _registerMemory(GPURegisters()), _sharedMemory(sharedMemoryExtent) {
        _internalMMU.mapDevice(INTERNAL_PRIVATE_MEMORY_ADDRESS, &_privateMemory);
        _internalMMU.mapDevice(INTERNAL_SHARED_MEMORY_ADDRESS, &_sharedMemory);

        _externalMMU.mapDevice(EXTERNAL_REGISTER_MEMORY_ADDRESS, &_registerMemory);
        _externalMMU.mapDevice(EXTERNAL_SHARED_MEMORY_ADDRESS, &_sharedMemory);
    }

    IMemoryDevice* getExternalMemoryDevice() override {
        return &_externalMMU;
    }

    UUID getTypeUUID() const override {
        return uuid();
    }

    MemoryAddress getRegisterMemoryExternalAddress() const {
        return EXTERNAL_REGISTER_MEMORY_ADDRESS;
    }

    MemoryAddress getSharedMemoryExternalAddress() const {
        return EXTERNAL_SHARED_MEMORY_ADDRESS;
    }

    static UUID uuid() {
        return UUID("4132ba0e-2faf-4a13-b701-9e26f354f1d2");
    }
};

}
