#pragma once

#include <corecrt_wstring.h>
#include <cstdint>
#include <stdexcept>
#include <map>
#include <limits>

namespace k32 {

using MemoryAddress = uint32_t;
using MemoryExtent = uint32_t;

inline constexpr MemoryExtent kilobytes(unsigned long long kb) {
    return kb << 10;
}

inline constexpr MemoryExtent megabytes(unsigned long long mb) {
    return mb << 20;
}

inline constexpr MemoryExtent gigabytes(unsigned long long gb) {
    return gb << 30;
}

namespace memory_literals {

inline constexpr MemoryExtent operator""_KB(unsigned long long kb) {
    return kilobytes(kb);
}

inline constexpr MemoryExtent operator""_MB(unsigned long long mb) {
    return megabytes(mb);
}

inline constexpr MemoryExtent operator""_GB(unsigned long long gb) {
    return gigabytes(gb);
}

}

constexpr MemoryAddress MEMORY_ADDRESS_INVALID = std::numeric_limits<MemoryAddress>::max();

class IMemoryDevice {
public:
    virtual MemoryExtent capacity() const noexcept = 0;
    virtual bool contiguous() const noexcept = 0;

    virtual bool validateRegion(MemoryAddress address, MemoryExtent extent = 0) const noexcept = 0;

    virtual uint32_t word(MemoryAddress address) const = 0;
    virtual void word(MemoryAddress address, uint32_t value) = 0;
};

class BufferMemory : public IMemoryDevice {
private:
    uint32_t* _buffer;
    MemoryExtent _extent;

public:
    BufferMemory(MemoryExtent extent) {
        if ((extent % 4) != 0) {
            throw std::runtime_error("Memory extent must be a multiple of 4 bytes");
        }

        _extent = extent;
        _buffer = new uint32_t[_extent / 4];
    }

    ~BufferMemory() {
        delete[] _buffer;
    }

    MemoryExtent capacity() const noexcept override {
        return _extent;
    }

    bool contiguous() const noexcept override {
        return true;
    }

    bool validateRegion(MemoryAddress address, MemoryExtent extent) const noexcept override {
        return (address + extent) < _extent;
    }

    uint32_t word(MemoryAddress address) const override {
        if ((address % 4) != 0) {
            throw std::runtime_error("Memory address is not multiple of 4 bytes");
        }

        if (!validateRegion(address, 4)) {
            throw std::runtime_error("Memory address is out of bounds");
        }

        return _buffer[address / 4];
    }

    void word(MemoryAddress address, uint32_t value) override {
        if ((address % 4) != 0) {
            throw std::runtime_error("Memory address is not multiple of 4 bytes");
        }

        if (!validateRegion(address, 4)) {
            throw std::runtime_error("Memory address is out of bounds");
        }

        _buffer[address / 4] = value;
    }
};

template<typename S>
class RegisterMemory : public IMemoryDevice {
private:
    S _data;

public:
    RegisterMemory(S const& data) : _data(data) {}

    MemoryExtent capacity() const noexcept override {
        return ((sizeof(S) + 3) / 4) * 4;
    }

    bool contiguous() const noexcept override {
        return true;
    }

    bool validateRegion(MemoryAddress address, MemoryExtent extent = 0) const noexcept override {
        return (address + extent) < capacity();
    }

    uint32_t word(MemoryAddress address) const override {
        if ((address % 4) != 0) {
            throw std::runtime_error("Memory address is not multiple of 4 bytes");
        }

        if (!validateRegion(address, 4)) {
            throw std::runtime_error("Memory address is out of bounds");
        }

        uint32_t const* data = reinterpret_cast<uint32_t const*>(&_data);
        return data[address / 4];
    }

    void word(MemoryAddress address, uint32_t value) override {
        if ((address % 4) != 0) {
            throw std::runtime_error("Memory address is not multiple of 4 bytes");
        }

        if (!validateRegion(address, 4)) {
            throw std::runtime_error("Memory address is out of bounds");
        }

        uint32_t* data = reinterpret_cast<uint32_t*>(&_data);
        data[address / 4] = value;
    }

    S& data() {
        return _data;
    }

    S const& data() const {
        return _data;
    }
};

struct MemoryRegion {
    MemoryAddress mappedAddress;
    IMemoryDevice* memoryDevice;

    MemoryRegion() : mappedAddress(MEMORY_ADDRESS_INVALID), memoryDevice(nullptr) {}
};

class MemoryGroup : public IMemoryDevice {
private:
    std::map<MemoryAddress, MemoryRegion> _regions;

    std::map<MemoryAddress, MemoryRegion>::const_iterator findNearestRegion(MemoryAddress address) const {
        if (_regions.empty()) {
            return _regions.end();
        }

        auto it = _regions.lower_bound(address);
        if (it == _regions.end()) {
            return _regions.end();
        }

        if (it->second.memoryDevice->validateRegion(address - it->second.mappedAddress)) {
            return it;
        }

        if (it == _regions.begin()) {
            return _regions.end();
        }

        it = std::prev(it);
        if (it->second.memoryDevice->validateRegion(address - it->second.mappedAddress)) {
            return it;
        }

        return _regions.end();
    }

public:
    MemoryExtent capacity() const noexcept override {
        MemoryExtent total = 0;
        for (auto it : _regions) {
            total += it.second.memoryDevice->capacity();
        }

        return total;
    }

    bool contiguous() const noexcept override {
        return false;
    }

    bool validateRegion(MemoryAddress address, MemoryExtent extent) const noexcept override {
        auto it = findNearestRegion(address);
        if (it == _regions.end()) {
            return false;
        }

        return it->second.memoryDevice->validateRegion(address - it->second.mappedAddress, extent);
    }

    uint32_t word(MemoryAddress address) const override {
        if ((address % 4) != 0) {
            throw std::runtime_error("Memory address is not multiple of 4 bytes");
        }

        auto it = findNearestRegion(address);
        if (it == _regions.end() || !it->second.memoryDevice->validateRegion(address - it->second.mappedAddress, 4)) {
            throw std::runtime_error("Memory address is out of bounds");
        }

        return it->second.memoryDevice->word(address - it->second.mappedAddress);
    }

    void word(MemoryAddress address, uint32_t value) override {
        if ((address % 4) != 0) {
            throw std::runtime_error("Memory address is not multiple of 4 bytes");
        }

        auto it = findNearestRegion(address);
        if (it == _regions.end() || !it->second.memoryDevice->validateRegion(address - it->second.mappedAddress, 4)) {
            throw std::runtime_error("Memory address is out of bounds");
        }

        it->second.memoryDevice->word(address - it->second.mappedAddress, value);
    }

    bool mapDevice(MemoryAddress mappedAddress, IMemoryDevice* memoryDevice) {
        auto it = findNearestRegion(mappedAddress);

        /* TODO: more intricate overlap checking */
        if (it != _regions.end() && it->second.memoryDevice->validateRegion(mappedAddress - it->second.mappedAddress)) {
            return false;
        }

        MemoryRegion region = {};
        region.mappedAddress = mappedAddress;
        region.memoryDevice = memoryDevice;

        _regions[mappedAddress] = region;
        return true;
    }

    MemoryAddress getMappedAddress(IMemoryDevice const* memoryDevice) const {
        for (auto it : _regions) {
            if (it.second.memoryDevice == memoryDevice) {
                return it.second.mappedAddress;
            }
        }

        return MEMORY_ADDRESS_INVALID;
    }

    MemoryRegion getMappedRegion(MemoryAddress address) const {
        auto it = findNearestRegion(address);
        if (it == _regions.end()) {
            return invalidRegion();
        }

        return it->second;
    }

    MemoryRegion invalidRegion() const {
        return MemoryRegion();
    }
};

}
