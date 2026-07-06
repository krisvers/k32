#pragma once

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "../uuid.hpp"
#include "../memory.hpp"
#include "device.hpp"
#include "gpu.hpp"

namespace k32 {

namespace dev {

using CPUInstructionWord = uint32_t;
using CPURegisterID = uint32_t;

enum class CPUInstructionID : uint32_t {
    /* load/store */
    LI = 0x00,
    LMR = 0x01,
    LMIP = 0x02,
    SMR = 0x03,
    SMIP = 0x04,

    /* arithmetic */
    ADD4 = 0x05,
    UADD = 0x06,
    SUB3 = 0x07,
    USUB = 0x08,
    MUL4 = 0x09,
    UMUL = 0x0a,
    DIV4 = 0x0b,
    UDIV = 0x0c,

    /* logic */
    AND4 = 0x0d,
    AND = 0x0e,
    OR4 = 0x0f,
    OR = 0x10,
    XOR4 = 0x11,
    XOR = 0x12,
    SH4 = 0x13,
    SH = 0x14,

    /* jumps/branches */
    JR = 0x15,
    JIP = 0x16,

    /* special */
    SPI = 0x17,

    /* floating point */
    FI = 0x18,
    FAS = 0x19,
    FMUL = 0x1a,
    FDIV = 0x1b,
    FSQRT = 0x1c,
    FLOG = 0x1d,
    FPOW = 0x1e,
    FTYPE = 0x1f,
};

inline std::ostream& operator<<(std::ostream& os, CPUInstructionID id) {
    switch (id) {
        case CPUInstructionID::LI:
            os << "li";
            break;
        case CPUInstructionID::LMR:
            os << "lmr";
            break;
        case CPUInstructionID::LMIP:
            os << "lmip";
            break;
        case CPUInstructionID::SMR:
            os << "smr";
            break;
        case CPUInstructionID::SMIP:
            os << "smip";
            break;
        case CPUInstructionID::ADD4:
            os << "add4";
            break;
        case CPUInstructionID::UADD:
            os << "uadd";
            break;
        case CPUInstructionID::SUB3:
            os << "sub3";
            break;
        case CPUInstructionID::USUB:
            os << "usub";
            break;
        case CPUInstructionID::MUL4:
            os << "mul4";
            break;
        case CPUInstructionID::UMUL:
            os << "umul";
            break;
        case CPUInstructionID::DIV4:
            os << "div4";
            break;
        case CPUInstructionID::UDIV:
            os << "udiv";
            break;
        case CPUInstructionID::AND4:
            os << "and4";
            break;
        case CPUInstructionID::AND:
            os << "and";
            break;
        case CPUInstructionID::OR4:
            os << "or4";
            break;
        case CPUInstructionID::OR:
            os << "or";
            break;
        case CPUInstructionID::XOR4:
            os << "xor4";
            break;
        case CPUInstructionID::XOR:
            os << "xor";
            break;
        case CPUInstructionID::SH4:
            os << "sh4";
            break;
        case CPUInstructionID::SH:
            os << "sh";
            break;
        case CPUInstructionID::JR:
            os << "jr";
            break;
        case CPUInstructionID::JIP:
            os << "jip";
            break;
        case CPUInstructionID::SPI:
            os << "spi";
            break;
        case CPUInstructionID::FI:
            os << "fi";
            break;
        case CPUInstructionID::FAS:
            os << "fas";
            break;
        case CPUInstructionID::FMUL:
            os << "fmul";
            break;
        case CPUInstructionID::FDIV:
            os << "fdiv";
            break;
        case CPUInstructionID::FSQRT:
            os << "fsqrt";
            break;
        case CPUInstructionID::FLOG:
            os << "flog";
            break;
        case CPUInstructionID::FPOW:
            os << "fpow";
            break;
        case CPUInstructionID::FTYPE:
            os << "ftype";
            break;
        default:
            os << "unknown";
            break;
    }

    return os;
}

enum class CPUConditionID : uint32_t {
    IFZ = 0x0,
    IFNZ = 0x1,
    IFEQ = IFZ,
    IFNEQ = IFNZ,

    IFC = 0x2,
    IFNC = 0x3,
    IFUL = IFC,
    IFUNGEQ = IFC,
    IFUNL = IFNC,
    IFUGEQ = IFNC,

    IFUG = 0x4,
    IFUNG = 0x5,
    IFUNLEQ = IFUG,
    IFULEQ = IFUNG,

    IFO = 0x6,
    IFNO = 0x7,

    IFS = 0x8,
    IFNS = 0x9,

    IFSYS = 0xa,

    IFL = 0xb,
    IFNL = 0xc,
    IFNGEQ = IFL,
    IFGEQ = IFNL,

    IFG = 0xd,
    IFNG = 0xe,
    IFNLEQ = IFG,
    IFLEQ = IFNG,

    ALWAYS = 0xf,
};

inline std::ostream& operator<<(std::ostream& os, CPUConditionID id) {
    switch (id) {
        case CPUConditionID::IFZ:
            os << "ifz";
            break;
        case CPUConditionID::IFNZ:
            os << "ifnz";
            break;
        case CPUConditionID::IFC:
            os << "ifc";
            break;
        case CPUConditionID::IFNC:
            os << "ifnc";
            break;
        case CPUConditionID::IFUG:
            os << "ifug";
            break;
        case CPUConditionID::IFUNG:
            os << "ifung";
            break;
        case CPUConditionID::IFO:
            os << "ifo";
            break;
        case CPUConditionID::IFNO:
            os << "ifno";
            break;
        case CPUConditionID::IFS:
            os << "ifs";
            break;
        case CPUConditionID::IFNS:
            os << "ifns";
            break;
        case CPUConditionID::IFSYS:
            os << "ifsys";
            break;
        case CPUConditionID::IFL:
            os << "ifl";
            break;
        case CPUConditionID::IFNL:
            os << "ifnl";
            break;
        case CPUConditionID::IFG:
            os << "ifg";
            break;
        case CPUConditionID::IFNG:
            os << "ifng";
            break;
        case CPUConditionID::ALWAYS:
            break;
        default:
            os << "unknown";
            break;
    }

    return os;
}

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

enum class CPUInstructionImmediateType : uint32_t {
    None = 0x00,
    Unsigned8 = 0x01,
    Unsigned12 = 0x02,
    Unsigned16 = 0x03,
    Unsigned20 = 0x04,
    Signed8 = 0x05,
    Signed12 = 0x06,
    Signed16 = 0x07,
    Signed20 = 0x08,
};

class CPUInstructionInfo {
private:
    CPUInstructionID _opcode = {};
    CPUConditionID _condition = {};
    std::vector<CPURegisterID> _integerRegisterIDs = {};
    std::vector<CPURegisterID> _floatRegisterIDs = {};
    std::vector<CPURegisterID> _specialRegisterIDs = {};
    uint32_t _immediateValue = {};
    uint32_t _flags = {};
    CPUInstructionImmediateType _immediateType = CPUInstructionImmediateType::None;

public:
    CPUInstructionInfo(CPUInstructionWord word) {
        _opcode = static_cast<CPUInstructionID>(word & 0x001f);
        _condition = static_cast<CPUConditionID>((word >> 5) & 0x000f);

        /* decode parameters */
        switch (_opcode) {
            case CPUInstructionID::MUL4:
            case CPUInstructionID::DIV4:
            case CPUInstructionID::AND4:
            case CPUInstructionID::OR4:
            case CPUInstructionID::XOR4:
            case CPUInstructionID::SH4:
                /* A.4r.flag1 */
                _flags = (word >> 29) & 0x01;
            case CPUInstructionID::ADD4:
                /* A.4r.X */
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x1f));
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 14) & 0x1f));
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 19) & 0x1f));
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 24) & 0x1f));
                break;

            case CPUInstructionID::SUB3:
                /* A.3r.X */
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x1f));
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 14) & 0x1f));
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 19) & 0x1f));
                break;

            case CPUInstructionID::AND:
            case CPUInstructionID::OR:
            case CPUInstructionID::XOR:
            case CPUInstructionID::SH:
                /* A.2r.1imm12.flag1 */
                _flags = (word >> 19) & 0x01;
            case CPUInstructionID::LMR:
            case CPUInstructionID::SMR:
            case CPUInstructionID::UADD:
            case CPUInstructionID::USUB:
            case CPUInstructionID::UMUL:
            case CPUInstructionID::UDIV:
                /* A.2r.1imm12.X / A.2r.1signed12.X */
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x1f));
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 14) & 0x1f));

                _immediateValue = (word >> 20) & 0xfff;
                if (_opcode == CPUInstructionID::LMR || _opcode == CPUInstructionID::SMR) {
                    _immediateType = CPUInstructionImmediateType::Signed12;
                } else {
                    _immediateType = CPUInstructionImmediateType::Unsigned12;
                }

                break;

            case CPUInstructionID::LI:
            case CPUInstructionID::LMIP:
            case CPUInstructionID::SMIP:
            case CPUInstructionID::JR:
                /* A.1r.1imm16 */
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x1f));

                _immediateValue = (word >> 16) & 0xffff;
                if (_opcode == CPUInstructionID::LI) {
                    _immediateType = CPUInstructionImmediateType::Unsigned16;
                } else {
                    _immediateType = CPUInstructionImmediateType::Signed16;
                }

                break;

            case CPUInstructionID::SPI:
                /* A.1r.1sp.flag3 */
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x1f));
                _specialRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 14) & 0x0f));

                _flags = (word >> 18) & 0x07;
                break;

            case CPUInstructionID::JIP:
                _immediateValue = (word >> 12) & 0xfffff;
                _immediateType = CPUInstructionImmediateType::Signed20;
                break;

            case CPUInstructionID::FI:
                /* A.1r.1f.flag3 */
                _integerRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x1f));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 14) & 0x07));

                _flags = (word >> 17) & 0x07;
                break;

            case CPUInstructionID::FAS:
                /* A.6f.flag1 */
                _flags = (word >> 27) & 0x01;
            case CPUInstructionID::FMUL:
            case CPUInstructionID::FSQRT:
            case CPUInstructionID::FLOG:
            case CPUInstructionID::FPOW:
                /* A.6f.X */
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 12) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 15) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 18) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 21) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 24) & 0x07));
                break;

            case CPUInstructionID::FDIV:
                /* A.7f */
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 12) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 15) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 18) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 21) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 24) & 0x07));
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 27) & 0x07));
                break;


            case CPUInstructionID::FTYPE:
                /* A.1f */
                _floatRegisterIDs.push_back(static_cast<CPURegisterID>((word >> 9) & 0x07));
                break;

            default:
                throw CPUFault(CPUInterruptID::InvalidInstruction, "Unrecognized opcode");
        }
    }

    CPUInstructionID opcode() const {
        return _opcode;
    }

    CPUConditionID condition() const {
        return _condition;
    }

    CPURegisterID rid(size_t index) const {
        if (_integerRegisterIDs.size() <= index) {
            throw std::runtime_error("Invalid integer register lookup index");
        }

        return _integerRegisterIDs[index];
    }

    CPURegisterID fid(size_t index) const {
        if (_floatRegisterIDs.size() <= index) {
            throw std::runtime_error("Invalid float register lookup index");
        }

        return _floatRegisterIDs[index];
    }

    CPURegisterID spid(size_t index) const {
        if (_integerRegisterIDs.size() <= index) {
            throw std::runtime_error("Invalid special register lookup index");
        }

        return _specialRegisterIDs[index];
    }

    uint32_t unsigned8() const {
        return _immediateValue;
    }

    uint32_t unsigned12() const {
        return _immediateValue;
    }

    uint32_t unsigned16() const {
        return _immediateValue;
    }

    uint32_t unsigned20() const {
        return _immediateValue;
    }

    int32_t signed8() const {
        if ((_immediateValue & 0x80) != 0) {
            return -static_cast<int32_t>(((~_immediateValue) & 0xff) - 1);
        }

        return _immediateValue;
    }

    int32_t signed12() const {
        if ((_immediateValue & 0x800) != 0) {
            return -static_cast<int32_t>(((~_immediateValue) & 0xfff) - 1);
        }

        return _immediateValue;
    }

    int32_t signed16() const {
        if ((_immediateValue & 0x8000) != 0) {
            return -static_cast<int32_t>(((~_immediateValue) & 0xffff) - 1);
        }

        return _immediateValue;
    }

    int32_t signed20() const {
        if ((_immediateValue & 0x80000) != 0) {
            return -static_cast<int32_t>(((~_immediateValue) & 0xfffff) - 1);
        }

        return _immediateValue;
    }

    uint32_t flags() const {
        return _flags;
    }

    friend std::ostream& operator<<(std::ostream& os, CPUInstructionInfo const& instruction);
};

inline std::ostream& operator<<(std::ostream& os, CPUInstructionInfo const& instruction) {
    os << instruction._condition << " " << instruction._opcode << " ";
    for (CPURegisterID rid : instruction._integerRegisterIDs) {
        os << "r" << std::dec << rid << ", ";
    }

    for (CPURegisterID fid : instruction._floatRegisterIDs) {
        os << "f" << std::dec << fid << ", ";
    }

    for (CPURegisterID spid : instruction._specialRegisterIDs) {
        os << "sp" << std::dec << spid << ", ";
    }

    int32_t signedImmediate = 0;
    switch (instruction._immediateType) {
        case CPUInstructionImmediateType::Signed8:
            signedImmediate = instruction.signed8();
            break;
        case CPUInstructionImmediateType::Signed12:
            signedImmediate = instruction.signed12();
            break;
        case CPUInstructionImmediateType::Signed16:
            signedImmediate = instruction.signed16();
            break;
        case CPUInstructionImmediateType::Signed20:
            signedImmediate = instruction.signed20();
            break;
        default:
            break;
    }

    switch (instruction._immediateType) {
        case CPUInstructionImmediateType::Unsigned8:
            os << std::hex << instruction.unsigned8();
            break;
        case CPUInstructionImmediateType::Unsigned12:
            os << std::hex << instruction.unsigned12();
            break;
        case CPUInstructionImmediateType::Unsigned16:
            os << std::hex << instruction.unsigned16();
            break;
        case CPUInstructionImmediateType::Unsigned20:
            os << std::hex << instruction.unsigned20();
            break;
        case CPUInstructionImmediateType::Signed8:
        case CPUInstructionImmediateType::Signed12:
        case CPUInstructionImmediateType::Signed16:
        case CPUInstructionImmediateType::Signed20:
            if (signedImmediate < 0) {
                os << std::hex << "-" << -signedImmediate;
            } else {
                os << std::hex << "+" << signedImmediate;
            }
            break;
        case CPUInstructionImmediateType::None:
        default:
            break;
    }

    os << "; flags: " << std::hex << instruction._flags;
    return os;
}

template<typename T>
class CPURegister {
private:
    T _value = {};
    bool _readAllowed = true;
    bool _writeAllowed = true;

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

    bool operator==(CPUStatus const& other) {
        return
            zero == other.zero &&
            carry == other.carry &&
            overflow == other.overflow &&
            sign == other.sign &&
            system == other.system &&
            interrupt == other.interrupt;
    }

    bool operator!=(CPUStatus const& other) {
        return !CPUStatus::operator==(other);
    }

    bool condition(CPUConditionID condition) {
        switch (condition) {
            case CPUConditionID::IFZ:
                return zero;
            case CPUConditionID::IFNZ:
                return !zero;
            case CPUConditionID::IFC:
                return carry;
            case CPUConditionID::IFNC:
                return !carry;
            case CPUConditionID::IFUG:
                return !carry && !zero;
            case CPUConditionID::IFUNG:
                return !zero;
            case CPUConditionID::IFO:
                return overflow;
            case CPUConditionID::IFNO:
                return !overflow;
            case CPUConditionID::IFS:
                return sign;
            case CPUConditionID::IFNS:
                return !sign;
            case CPUConditionID::IFSYS:
                return system;
            case CPUConditionID::IFL:
                return sign == overflow;
            case CPUConditionID::IFNL:
                return sign != overflow;
            case CPUConditionID::IFG:
                return sign == overflow && !zero;
            case CPUConditionID::IFNG:
                return sign != overflow && zero;
            case CPUConditionID::ALWAYS:
                return true;
            default:
                throw std::runtime_error("Unknown condition ID");
        }
    }
};

inline std::ostream& operator<<(std::ostream& os, CPUStatus const& status) {
    os << (status.zero ? 'Z' : '_')
        << (status.carry ? 'C' : '_')
        << (status.overflow ? 'O' : '_')
        << (status.sign ? 'S' : '_')
        << (status.system ? 'Y' : '_')
        << (status.interrupt ? 'I' : '_');
    return os;
}

class CPURegisters {
private:
    /* general-purpose */
    CPURegister<uint32_t> _r[32] = {};
    CPURegister<float> _f[8] = {};

    /* special system-only */
    CPURegister<MemoryAddress> _p[4] = {};
    CPURegister<MemoryAddress> _mmtable = {};
    CPURegister<MemoryAddress> _inthandler = {};
    CPURegister<uint32_t> _level = {};

    /* special system and user level */
    CPURegister<uint32_t> _interrupt = {};

    /* hidden */
    MemoryAddress _ip = {};
    CPUStatus _status = {};

    /* implementation detail */
    MemoryAddress _interruptOriginIP = {};
    CPUStatus _interruptOriginStatus = {};

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
    CPURegisters() {
        updateAccessPermissions();
    }

    uint32_t r(CPURegisterID id) {
        if (id >= 32) {
            throw std::runtime_error("Integer register ID out of range");
        }

        return _r[id];
    }

    void r(CPURegisterID id, uint32_t value) {
        if (id >= 32) {
            throw std::runtime_error("Integer register ID out of range");
        }

        _r[id] = value;
    }

    float f(CPURegisterID id) {
        if (id >= 8) {
            throw std::runtime_error("Float register ID out of range");
        }

        return _f[id];
    }

    void f(CPURegisterID id, float value) {
        if (id >= 8) {
            throw std::runtime_error("Float register ID out of range");
        }

        _f[id] = value;
    }

    uint32_t p(CPURegisterID id) {
        if (id >= 4) {
            throw std::runtime_error("Pointer register ID out of range");
        }

        return _p[id];
    }

    void p(CPURegisterID id, uint32_t value) {
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

    uint32_t sp(CPURegisterID id) {
        if (id >= 16) {
            throw std::runtime_error("Special register ID out of range");
        }

        if (id < 4) {
            return p(id);
        }

        if (id == 4) {
            return mmtable();
        }

        if (id == 5) {
            return inthandler();
        }

        if (id == 6) {
            return level();
        }

        if (id == 8) {
            return interrupt();
        }

        throw std::runtime_error("Special register ID unallocated; cannot access register");
    }

    void sp(CPURegisterID id, uint32_t value) {
        if (id >= 16) {
            throw std::runtime_error("Special register ID out of range");
        }

        if (id < 4) {
            p(id, value);
        } else if (id == 4) {
            mmtable(value);
        } else if (id == 5) {
            inthandler(value);
        } else if (id == 6) {
            level(value);
        } else if (id == 8) {
            interrupt(value);
        } else {
            throw std::runtime_error("Special register ID unallocated; cannot access register");
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

    friend std::ostream& operator<<(std::ostream& os, CPURegisters registers);
};

inline std::ostream& operator<<(std::ostream& os, CPURegisters registers) {
    os << std::hex <<
        "r0: " << registers._r[0] << "\t" <<
        "r1: " << registers._r[1] << "\t" <<
        "r2: " << registers._r[2] << "\t" <<
        "r3: " << registers._r[3] << "\t" <<
        "r4: " << registers._r[4] << "\t" <<
        "r5: " << registers._r[5] << "\t" <<
        "r6: " << registers._r[6] << "\t" <<
        "r7: " << registers._r[7] << "\t" << std::endl <<
        "r8: " << registers._r[8] << "\t" <<
        "r9: " << registers._r[9] << "\t" <<
        "r10: " << registers._r[10] << "\t" <<
        "r11: " << registers._r[11] << "\t" <<
        "r12: " << registers._r[12] << "\t" <<
        "r13: " << registers._r[13] << "\t" <<
        "r14: " << registers._r[14] << "\t" <<
        "r15: " << registers._r[15] << "\t" << std::endl <<
        "r16: " << registers._r[16] << "\t" <<
        "r17: " << registers._r[17] << "\t" <<
        "r18: " << registers._r[18] << "\t" <<
        "r19: " << registers._r[19] << "\t" <<
        "r20: " << registers._r[20] << "\t" <<
        "r21: " << registers._r[21] << "\t" <<
        "r22: " << registers._r[22] << "\t" <<
        "r23: " << registers._r[23] << "\t" << std::endl <<
        "r24: " << registers._r[24] << "\t" <<
        "r25: " << registers._r[25] << "\t" <<
        "r26: " << registers._r[26] << "\t" <<
        "r27: " << registers._r[27] << "\t" <<
        "r28: " << registers._r[28] << "\t" <<
        "r29: " << registers._r[29] << "\t" <<
        "r30: " << registers._r[30] << "\t" <<
        "r31: " << registers._r[31] << "\t" << std::endl <<
        "ip: " << registers._ip << std::endl <<
        "status: " << registers._status;
    return os;
}

class CPU : public IDevice {
private:
    CPURegisters _registers = {};
    BufferMemory _systemMemory;
    MemoryGroup _mmu = {};

    GPU* _gpu = nullptr;

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
        return &_mmu;
    }

    bool execute() override {
        std::cout << _registers << std::endl;

        CPUStatus status = _registers.status();

        CPUInstructionWord instructionWord = _mmu.word(_registers.ip());
        CPUInstructionInfo instructionInfo = CPUInstructionInfo(instructionWord);

        std::cout << "instruction: " << std::hex << instructionWord << ": " << instructionInfo << std::endl << std::endl;

        if (!_registers.status().condition(instructionInfo.condition())) {
            _registers.ip() += 4;
            return true;
        }

        bool progressIP = true;
        switch (instructionInfo.opcode()) {
            case CPUInstructionID::LI:
                _registers.r(instructionInfo.rid(0),
                    (_registers.r(instructionInfo.rid(0)) & 0xffff0000) |
                    instructionInfo.unsigned16());
                break;
            case CPUInstructionID::LMR:
                _registers.r(instructionInfo.rid(0),
                    _mmu.word(_registers.r(instructionInfo.rid(1)) + instructionInfo.signed12()));
                break;
            case CPUInstructionID::LMIP:
                _registers.r(instructionInfo.rid(0),
                    _mmu.word(_registers.ip() + instructionInfo.signed16()));
                break;
            case CPUInstructionID::SMR:
                _mmu.word(_registers.r(instructionInfo.rid(1)) + instructionInfo.signed12(),
                    _registers.r(instructionInfo.rid(0)));
                break;
            case CPUInstructionID::SMIP:
                _mmu.word(_registers.ip() + instructionInfo.signed16(),
                    _registers.r(instructionInfo.rid(0)));
                break;
            case CPUInstructionID::ADD4: {
                uint64_t a = _registers.r(instructionInfo.rid(2));
                uint64_t b = _registers.r(instructionInfo.rid(3));
                uint64_t x = a + b;

                _registers.r(instructionInfo.rid(0),
                    static_cast<uint32_t>(x & 0xffffffff));

                if (instructionInfo.rid(1) != instructionInfo.rid(0)) {
                    _registers.r(instructionInfo.rid(1),
                        static_cast<uint32_t>(x >> 32));
                }

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::UADD: {
                uint64_t a = _registers.r(instructionInfo.rid(1));
                uint64_t b = instructionInfo.unsigned12();
                uint64_t x = a + b;

                _registers.r(instructionInfo.rid(0),
                    static_cast<uint32_t>(x & 0xffffffff));

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::SUB3: {
                uint32_t a = _registers.r(instructionInfo.rid(1));
                uint32_t b = _registers.r(instructionInfo.rid(2));
                uint32_t x = a - b;

                _registers.r(instructionInfo.rid(0), x);

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::USUB: {
                uint32_t a = _registers.r(instructionInfo.rid(1));
                uint32_t b = instructionInfo.unsigned12();
                uint32_t x = a - b;

                _registers.r(instructionInfo.rid(0), x);

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::MUL4: {
                uint32_t a = _registers.r(instructionInfo.rid(2));
                uint32_t b = _registers.r(instructionInfo.rid(3));

                int64_t x;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    int32_t ia = *reinterpret_cast<int32_t*>(&a);
                    int32_t ib = *reinterpret_cast<int32_t*>(&b);
                    x = ia * ib;
                } else {
                    x = a * b;
                }

                _registers.r(instructionInfo.rid(0),
                    static_cast<uint32_t>(x & 0xffffffff));

                if (instructionInfo.rid(1) != instructionInfo.rid(0)) {
                    _registers.r(instructionInfo.rid(1),
                        static_cast<uint32_t>(*reinterpret_cast<uint64_t*>(&x) >> 32));
                }

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::UMUL: {
                uint32_t a = _registers.r(instructionInfo.rid(1));
                uint32_t b = instructionInfo.unsigned12();
                uint32_t x = a * b;

                _registers.r(instructionInfo.rid(0), x);

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::DIV4: {
                uint64_t a = _registers.r(instructionInfo.rid(2));
                uint64_t b = _registers.r(instructionInfo.rid(3));

                int64_t x;
                uint32_t remainder;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    int32_t ia = *reinterpret_cast<int32_t*>(&a);
                    int32_t ib = *reinterpret_cast<int32_t*>(&b);
                    x = ia / ib;
                    remainder = ia % ib;
                } else {
                    x = a / b;
                    remainder = a % b;
                }

                _registers.r(instructionInfo.rid(0),
                    static_cast<uint32_t>(x & 0xffffffff));

                if (instructionInfo.rid(1) != instructionInfo.rid(0)) {
                    _registers.r(instructionInfo.rid(1), remainder);
                }

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::UDIV: {
                uint32_t a = _registers.r(instructionInfo.rid(1));
                uint32_t b = instructionInfo.unsigned12();
                uint32_t x = a / b;

                _registers.r(instructionInfo.rid(0), x);

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::AND4: {
                uint32_t a = _registers.r(instructionInfo.rid(2));
                uint32_t b = _registers.r(instructionInfo.rid(3));
                uint32_t x = a & b;

                uint32_t x0 = x;
                uint32_t x1 = ~x;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    x0 = ~x;
                    x1 = x;
                }

                _registers.r(instructionInfo.rid(0), x0);

                if (instructionInfo.rid(1) != instructionInfo.rid(0)) {
                    _registers.r(instructionInfo.rid(1), x1);
                }

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::AND: {
                uint32_t a = _registers.r(instructionInfo.rid(1));
                uint32_t b = instructionInfo.unsigned12();
                uint32_t x = a & b;

                uint32_t x0 = x;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    x0 = ~x;
                }

                _registers.r(instructionInfo.rid(0), x0);

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::OR4: {
                uint32_t a = _registers.r(instructionInfo.rid(2));
                uint32_t b = _registers.r(instructionInfo.rid(3));
                uint32_t x = a | b;

                uint32_t x0 = x;
                uint32_t x1 = ~x;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    x0 = ~x;
                    x1 = x;
                }

                _registers.r(instructionInfo.rid(0), x0);

                if (instructionInfo.rid(1) != instructionInfo.rid(0)) {
                    _registers.r(instructionInfo.rid(1), x1);
                }

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::OR: {
                uint32_t a = _registers.r(instructionInfo.rid(1));
                uint32_t b = instructionInfo.unsigned12();
                uint32_t x = a | b;

                uint32_t x0 = x;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    x0 = ~x;
                }

                _registers.r(instructionInfo.rid(0), x0);

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::XOR4: {
                uint32_t a = _registers.r(instructionInfo.rid(2));
                uint32_t b = _registers.r(instructionInfo.rid(3));
                uint32_t x = a ^ b;

                uint32_t x0 = x;
                uint32_t x1 = ~x;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    x0 = ~x;
                    x1 = x;
                }

                _registers.r(instructionInfo.rid(0), x0);

                if (instructionInfo.rid(1) != instructionInfo.rid(0)) {
                    _registers.r(instructionInfo.rid(1), x1);
                }

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::XOR: {
                uint32_t a = _registers.r(instructionInfo.rid(1));
                uint32_t b = instructionInfo.unsigned12();
                uint32_t x = a ^ b;

                uint32_t x0 = x;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    x0 = ~x;
                }

                _registers.r(instructionInfo.rid(0), x0);

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::SH4: {
                uint32_t a = _registers.r(instructionInfo.rid(2));
                uint32_t b = _registers.r(instructionInfo.rid(3));

                uint32_t x0;
                uint32_t x1;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    uint64_t x = static_cast<uint64_t>(a) << 32 >> static_cast<uint64_t>(b);
                    x0 = static_cast<uint32_t>(x >> 32);
                    x1 = static_cast<uint32_t>(x & 0xffffffff);
                } else {
                    uint64_t x = static_cast<uint64_t>(a) << static_cast<uint64_t>(b);
                    x0 = static_cast<uint32_t>(x & 0xffffffff);
                    x1 = static_cast<uint32_t>(x >> 32);
                }

                _registers.r(instructionInfo.rid(0), x0);

                if (instructionInfo.rid(1) != instructionInfo.rid(0)) {
                    _registers.r(instructionInfo.rid(1), x1);
                }

                status.zero = (x0 == 0);
                status.carry = (x0 > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x0) & 0x80000000) != 0);
                status.sign = ((x0 & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::SH: {
                uint32_t a = _registers.r(instructionInfo.rid(1));
                uint32_t b = instructionInfo.unsigned12();

                uint32_t x;
                if ((instructionInfo.flags() & 0x01) != 0) {
                    x = a >> b;
                } else {
                    x = a << b;
                }

                _registers.r(instructionInfo.rid(0), x);

                status.zero = (x == 0);
                status.carry = (x > std::numeric_limits<uint32_t>::max());
                status.overflow = (((a ^ b) & 0x80000000) && ((a ^ x) & 0x80000000) != 0);
                status.sign = ((x & 0x80000000) != 0);
                break;
            }
            case CPUInstructionID::JR:
                progressIP = false;
                _registers.ip() = _registers.r(instructionInfo.rid(0)) + instructionInfo.signed16();
                break;
            case CPUInstructionID::JIP:
                progressIP = false;
                _registers.ip() += instructionInfo.signed20();
                break;
            case CPUInstructionID::SPI: {
                uint32_t r = _registers.r(instructionInfo.rid(0));
                uint32_t sp = _registers.sp(instructionInfo.spid(0));
                if ((instructionInfo.flags() & 0x01) != 0) {
                    _registers.sp(instructionInfo.spid(0), r);
                }

                if ((instructionInfo.flags() & 0x02) != 0) {
                    _registers.r(instructionInfo.rid(0), sp);
                }

                break;
            }
            case CPUInstructionID::FI: {
                uint32_t r = _registers.r(instructionInfo.rid(0));
                float f = _registers.f(instructionInfo.fid(0));

                uint32_t rf;
                float fr;
                if ((instructionInfo.flags() & 0x04) != 0) {
                    rf = *reinterpret_cast<uint32_t*>(&f);
                    fr = *reinterpret_cast<float*>(&r);
                } else {
                    rf = static_cast<uint32_t>(f);
                    fr = static_cast<float>(r);
                }

                if ((instructionInfo.flags() & 0x01) != 0) {
                    _registers.f(instructionInfo.fid(0), fr);
                }

                if ((instructionInfo.flags() & 0x02) != 0) {
                    _registers.r(instructionInfo.rid(0), rf);
                }

                break;
            }
            default:
                throw CPUFault(CPUInterruptID::InvalidInstruction, "Invalid or unimplemented instruction");
        }

        if (status != _registers.status()) {
            _registers.status(status);
        }

        if (progressIP) {
            _registers.ip() += 4;
        }

        return true;
    }

    UUID getTypeUUID() const override {
        return uuid();
    }

    static UUID uuid() {
        return UUID("524c3600-02c2-4414-8273-d5ac484bdad7");
    }
};

}

}
