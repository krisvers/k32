#pragma once

#include "../uuid.hpp"
#include "../memory.hpp"

namespace k32 {

namespace dev {

class IDevice {
public:
    virtual UUID getTypeUUID() const = 0;
    virtual IMemoryDevice* getExternalMemoryDevice() = 0;

    virtual bool execute() = 0;
};

}

}
