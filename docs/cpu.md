# k32 CPU

# 1. Registers

## 1.1. General Purpose Registers

- `r0`-`r31`: 32-bit integer registers [00000-11111]
- `f0`-`f7`: 32-bit IEEE 754 floating point registers [000-111]

## 1.2. Special Use Registers

### 1.2.1. System-Level Only

- `p0`-`p3`: 32-bit pointer registers (to be used by system code) [0000-0011]
- `mmtable`: 32-bit pointer register (holds address of memory mapping table) [0100]
- `inthandler`: 32-bit pointer register (holds address of interrupt handler which is exclusively system level) [0101]
- `level`: level register (used for transitioning between system and user level, only modifiable from system level) [0110]

### 1.2.2. System-Level and User-Level

- `interrupt`: 32-bit integer register (holds id of signaled interrupt) [1000]

## 1.3. Hidden Registers

- `ip`: 32-bit instruction pointer register
- `status`: 32-bit flags registers (holds information about previous instruction results and global processor state)
  - `zero`
  - `carry`
  - `overflow`
  - `sign`
  - `system`
  - `interrupt`

# 2. Memory

- 32-bit addressable
- accesses aligned to 4-byte boundaries
- initially physical addressing, but virtual addressing can be enabled after setting `mmtable`

## 2.1. Anatomy of an Address

| range | significance    |
| ----- | --------------- |
| 0-12  | physical offset |
| 12-22 | page index      |
| 22-32 | table index     |

## 2.2. Paging Structures

### 2.2.1. Table Entry

| range | significance                     |
| ----- | -------------------------------- |
| 0-32  | virtual address pointing to page |

### 2.2.2. Page Entry
| range | significance                             |
| ----- | ---------------------------------------- |
| 0-1   | read allowed                             |
| 1-2   | write allowed                            |
| 2-3   | execute allowed                          |
| 3-4   | system permission only                   |
| 4-11  | ...                                      |
| 11-12 | page enabled                             |
| 12-32 | upper 20 bits of page's physical address |

## 2.3. Address Translation

- physical = (memory[mmtable[(virtual >> 22) & 0x000003ff] + ((virtual >> 12) & 0x000003ff) * 4].upperPhysicalAddress & 0xfffff000) + virtual & 0x00000fff

# 3. Instructions

## 3.1. Encoding

- opcodes are 5-bit
- conditions are 4-bit
- GP integer registers are 5-bit
- GP float registers are 3-bit
- special registers are 4-bit
- immediate8 values are 8-bit (also unsigned8/signed8)
- immediate12 values are 12-bit (also unsigned12/signed12)
- immediate16 values are 16-bit (also unsigned16/signed16)
- immediate20 values are 20-bit (also unsigned20/signed20)
- flag1 values are 1-bit
- flag2 values are 2-bit

## 3.2. Conditions

- 0000: ifz/ifeq
  - if status.zero = 1
- 0001: ifnz/ifneq
  - if status.zero = 0
- 0010: ifc/iful/ifungeq
  - if status.carry = 1
- 0011: ifnc/ifunl/ifugeq
  - if status.carry = 0
- 0100: ifug/ifunleq
  - if status.carry = 0 and status.zero = 0
- 0101: ifung/ifuleq
  - if status.carry = 1 or status.zero = 1
- 0110: ifo
  - if status.overflow = 1
- 0111: ifno
  - if status.overflow = 0
- 1000: ifs
  - if status.sign = 1
- 1001: ifns
  - if status.sign = 0
- 1010: ifsys
  - if status.system = 1
- 1011: ifl/ifnleq
  - if status.sign = status.overflow
- 1100: ifnl/ifleq
  - if status.sign =/= status.overflow
- 1101: ifg/ifnleq
  - if status.sign = status.overflow and status.zero = 0
- 1110: ifng/ifleq
  - if status.sign =/= status.overflow and status.zero = 1
- 1111: always
  - always true

## 3.3. Layouts of Instructions

### 3.3.1. Layout Class A - Conditional

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-32  | ...          |                       |

#### 3.3.1.1. A.4r - Conditional 4 GP Integer Registers

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     | typically destination |
| 14-19 | register     |                       |
| 19-24 | register     |                       |
| 24-29 | register     |                       |
| 29-32 | ...          |                       |

#### 3.3.1.2. A.3r.1imm8 - Conditional 3 GP Integer Registers, 1 Immediate8

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     | typically destination |
| 14-19 | register     |                       |
| 19-24 | register     |                       |
| 24-32 | immediate8   |                       |

#### 3.3.1.3. A.2r.1imm12 - Conditional 2 GP Integer Registers, 1 Immediate12

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     | typically destination |
| 14-19 | register     |                       |
| 19-20 | ...          |                       |
| 20-32 | immediate12  |                       |

#### 3.3.1.4. A.1r.2imm8 - Conditional 1 GP Integer Register, 2 Immediate8

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     | typically destination |
| 14-16 | ...          |                       |
| 16-24 | immediate8   |                       |
| 24-32 | immediate8   |                       |

#### 3.3.1.5. A.1r.1imm16 - Conditional 1 GP Integer Register, 1 Immediate16

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     | typically destination |
| 14-16 | ...          |                       |
| 16-32 | immediate16  |                       |

#### 3.3.1.6. A.1r.1sp - Conditional 1 GP Integer Register, 1 Special Register

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     |                       |
| 14-18 | special      |                       |
| 18-32 | ...          |                       |

#### 3.3.1.7. A.1imm20 - Conditional 1 Immediate20

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-12  | ...          |                       |
| 12-32 | immediate20  |                       |

#### 3.3.1.8. A.3r - Conditional 3 GP Integer Registers

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     | typically destination |
| 14-19 | register     |                       |
| 19-24 | register     |                       |
| 24-32 | ...          |                       |

#### 3.3.1.9. A.4r.flag1 - Conditional 4 GP Integer Registers, Flag1

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     | typically destination |
| 14-19 | register     |                       |
| 19-24 | register     |                       |
| 24-29 | register     |                       |
| 29-30 | flag1        |                       |
| 30-32 | ...          |                       |

#### 3.3.1.10. A.2r.1imm12.flag1 - Conditional 2 GP Integer Registers, 1 Immediate12, Flag1

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     | typically destination |
| 14-19 | register     |                       |
| 19-20 | flag1        |                       |
| 20-32 | immediate12  |                       |

#### 3.3.1.11. A.1r.1sp.flag3 

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     |                       |
| 14-18 | special      |                       |
| 18-21 | flag3        |                       |

#### 3.3.1.12. A.1r.1f.flag3 

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-14  | register     |                       |
| 14-17 | float        |                       |
| 17-20 | flag3        |                       |

#### 3.3.1.13. A.6f

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-12  | float        |                       |
| 12-15 | float        |                       |
| 15-18 | float        |                       |
| 18-21 | float        |                       |
| 21-24 | float        |                       |
| 24-27 | float        |                       |

#### 3.3.1.14. A.6f.flag1

| range | significance | notes                 |
| ----- | ------------ | --------------------- |
| 0-5   | opcode       |                       |
| 5-9   | condition    |                       |
| 9-12  | float        |                       |
| 12-15 | float        |                       |
| 15-18 | float        |                       |
| 18-21 | float        |                       |
| 21-24 | float        |                       |
| 24-27 | float        |                       |
| 27-28 | flag1        |                       |

## 3.4. Instruction Listing and Opcodes

### 3.4.1. Load/Store

#### 3.4.1.1. 00000: `li` - (A.1r.1imm16) - Load Immediate16 into Lower Register

- if condition? register0 = (register0 & 0xffff0000) | immediate16

#### 3.4.1.2. 00001: `lmr` - (A.2r.1signed12) - Load Word from Memory Pointed to by Register1 Plus/Minus Signed12 Offset into Register0

- if condition? register0 = memory[register1 ± signed12]

#### 3.4.1.3. 00010: `lmip` - (A.1r.1signed16) - Load Word from Memory Pointed to by Instruction Pointer Plus/Minus Signed16 Offset into Register0

- if condition? register0 = memory[ip ± signed16]

#### 3.4.1.4. 00011: `smr` - (A.2r.1signed12) - Store Word from Register0 into Memory Pointed by Register1 Plus/Minus Signed12 Offset

- if condition? memory[register1 ± signed12] = register0

#### 3.4.1.5. 00100: `smip` - (A.1r.1signed16) - Store Word from Register0 into Memory Pointed by Instruction Pointer Plus/Minus Signed16 Offset

- if condition? memory[ip ± signed16] = register0

### 3.4.2. Arithmetic [S]

#### 3.4.2.1. 00101: `add4` [S] - (A.4r) - Add Register2 and Register3 Storing the Lower 32-bits of the Sum in Register0 and the Upper 32-bits of the in Register1

- if condition? register0 = (register2 + register3) & 0xffffffff; register1 = (register2 + register3) >> 32
- NOTE: if register0 is the same as register1, the upper 32-bits of the sum is discarded and register0/register1 is equal to the lower 32-bits of the sum

#### 3.4.2.2. 00110: `uadd` [S] - (A.2r.1imm12) - Add Register1 and Unsigned8 Storing the Lower 32-bits of the Sum in Register0

- if condition? register0 = (register1 + unsigned12) & 0xffffffff

#### 3.4.2.4. 00111: `sub3` [S] - (A.3r) - Subtract Register2 from Register1 Storing the Difference in Register0

- if condition? register0 = (register1 - register2)

#### 3.4.2.3. 01000: `usub` [S] - (A.2r.1imm12) - Sub Unsigned12 from Register1 Storing the Difference in Register0

- if condition? register0 = (register1 - unsigned12) & 0xffffffff

#### 3.4.2.5. 01001: `mul4` [S] - (A.4r.flag1) - Multiply Register2 by Register3 Storing the Lower 32-bits of the Product in Register0 and the Upper 32-bits of the Product in Register1

- if condition? register0 = (register2 * register3) & 0xffffffff; register1 = (register2 * register3) >> 32
- NOTE: if flag0 is 1, the multiplication performed is signed
- NOTE: if register0 is the same as register1, the upper 32-bits of the product are discarded and register0/register1 is equal to the lower 32-bits of the product

#### 3.4.2.6. 01010: `umul` [S] - (A.2r.1imm12) - Multiply Register1 by Unsigned12 Storing the Lower 32-bits of the Product in Register0

- if condition? register0 = (register1 * unsigned12) & 0xffffffff

#### 3.4.2.8. 01011: `div4` [S] - (A.4r.flag1) - Divide Register2 by Register3 Storing the Quotient in Register0 and the Remainder in Register1

- if condition? register0 = (register2 / register3); register1 = (register2 % register3)
- NOTE: if flag0 is 1, the division performed is signed
- NOTE: if register0 is the same as register1, the remainder is discarded and register0/register1 is equal to the quotient

#### 3.4.2.9. 01100: `udiv` [S] - (A.2r.1imm12) - Divide Register1 by Unsigned12 Storing the Quotient in Register0

- if condition? register0 = (register1 / unsigned12)

### 3.4.3. Logic

#### 3.4.3.1. 01101: `and4` [S] - (A.4r.flag1) - Binary And Register2 with Register3 Storing the Result in Register0 and the Result Negated in Register1

- if condition? register0 = (register2 & register3); register1 = ~(register2 & register3)
- NOTE: if register0 is the same as register1 and flag1 is 0, the negated result is discarded and register0/register1 is equal to the result of the binary and (non-negated)
- NOTE: if register0 is the same as register1 and flag1 is 1, the non-negated result is discarded and register0/register1 is equal to the negated result of the binary and

#### 3.4.3.2. 01110: `and` [S] - (A.2r.1imm12.flag1) - Binary And Register1 with Immediate12 Storing the Result in Register0

- if condition and flag1 is 0? register0 = (register1 & immediate12)
- if condition and flag1 is 1? register0 = ~(register1 & immediate12)

#### 3.4.3.3. 01111: `or4` [S] - (A.4r.flag1) - Binary Or Register2 with Register3 Storing the Result in Register0 and the Result Negated in Register1

- if condition? register0 = (register2 | register3); register1 = ~(register2 | register3)
- NOTE: if register0 is the same as register1 and flag1 is 0, the negated result is discarded and register0/register1 is equal to the result of the binary or (non-negated)
- NOTE: if register0 is the same as register1 and flag1 is 1, the non-negated result is discarded and register0/register1 is equal to the negated result of the binary or

#### 3.4.3.4. 10000: `or` [S] - (A.2r.1imm12.flag1) - Binary Or Register1 with Immediate12 Storing the Result in Register0

- if condition and flag1 is 0? register0 = (register1 | immediate12)
- if condition and flag1 is 1? register0 = ~(register1 | immediate12)

#### 3.4.3.5. 10001: `xor4` [S] - (A.4r.flag1) - Binary Exclusive Or Register2 with Register3 Storing the Result in Register0 and the Result Negated in Register1

- if condition? register0 = (register2 ^ register3); register1 = ~(register2 ^ register3)
- NOTE: if register0 is the same as register1 and flag1 is 0, the negated result is discarded and register0/register1 is equal to the result of the binary exclusive or (non-negated)
- NOTE: if register0 is the same as register1 and flag1 is 1, the non-negated result is discarded and register0/register1 is equal to the negated result of the binary exclusive or

#### 3.4.3.6. 10010: `xor` [S] - (A.2r.1imm12.flag1) - Binary Exclusive Or Register1 with Immediate12 Storing the Result in Register0

- if condition and flag1 is 0? register0 = (register1 ^ immediate12)
- if condition and flag1 is 1? register0 = ~(register1 ^ immediate12)

#### 3.4.3.7. 10011: `sh4` [S] - (A.4r.flag1) - Shift Register2 Left/Right by Register3 bits Storing the Result in Register0 and the Overflow/Underflow of the Result in Register1

- if condition and flag1 is 0? register0 = (register2 << register3) & 0xffffffff; register1 = (register2 << register3) >> 32
- if condition and flag1 is 1? register0 = (register2 >> register3) & 0xffffffff; register1 = (register2 << 32 >> register3)
- NOTE: if register0 is the same as register1, the overflow/underflow of the result is discarded and register0/register1 is equal to the non-overflowing/non-underflowing part of the result

#### 3.4.3.8. 10100: `sh` [S] - (A.2r.1imm12.flag1) - Shift Register1 Left/Right by Unsigned12 Storing the Result in Register0 and the Overflow/Underflow of the Result in Register1

- if condition and flag1 is 0? register0 = (register1 << unsigned12) & 0xffffffff
- if condition and flag1 is 1? register0 = (register1 >> unsigned12) & 0xffffffff

### 3.4.4. Jumps/Branches

#### 3.4.4.1. 10101: `jr` - (A.1r.1imm16) - Jump the Instruction Pointer to the Value Stored in Register0 Plus/Minus Signed16

- if condition? ip = register0 ± signed16

#### 3.4.4.2. 10110: `jip` - (A.1imm20) - Jump the Instruction Pointer to the Instruction Pointer Plus/Minus Signed20

- if condition? ip = ip ± signed20

### 3.4.5. Special

#### 3.4.5.1. 10111: `spi` - (A.1r.1sp.flag3) - Special Register/Integer Register Interop

- if condition?
  - if flag3.0 is 1? special0 = register0
  - if flag3.1 is 1? register0 = special0
- NOTE: if both flag3.0 and flag3.1 are 1, special0 and register0 will atomically swap values

### 3.4.6. Floating Point

#### 3.4.6.1. 11000: `fi` - (A.1r.1f.flag3) - Float/Integer Register Interop

- if condition?
  - if flag3.2 is 0?
    - if flag3.0 is 1? float0 = float(register0)
    - if flag3.1 is 1? register0 = uint(float0)
  - if flag3.2 is 1?
    - if flag3.0 is 1? float0 = reinterpret(register0)
    - if flag3.1 is 1? register0 = reinterpret(float0)
- NOTE: if both flag3.0 and flag3.1 are 1, float0 and register0 will atomically swap values

#### 3.4.6.2. 11001: `fas` [S] - (A.6f.flag1) - Add/Subtract Floats

- if condition and flag1 is 0? float0 = float1 + float2; float3 = float4 + float5
- if condition and flag1 is 1? float0 = float1 - float2; float3 = float4 - float5
- NOTE: if float0 and float3 are the same, the second operation is discarded and float0/float3 is equal to float1 + float2

#### 3.4.6.3. 11010: `fmul` [S] - (A.6f) - Multiply Floats

- if condition? float0 = float1 * float2; float3 = float4 * float5
- NOTE: if float0 and float3 are the same, the second operation is discarded and float0/float3 is equal to float1 * float2

#### 3.4.6.4. 11011: `fdiv` [S] - (A.7f) - Divide Floats

- if condition? float0 = float1 / float2; float3 = float4 / float5; float6 = float1 % float2
- NOTE: if float0 and float3 are the same, the second operation is discarded and float0/float3 is equal to float1 / float2
- NOTE: if float0 and float6 are the same, the remainder operation is discarded and float0/float6 is equal to float1 / float2
- NOTE: if float3 and float6 are the same, the remainder operation is discarded and float3/float6 is equal to float4 / float5

#### 3.4.6.7. 11100: `fsqrt` [S] - (A.6f) - Perform Square Root on Floats

- if condition? float0 = sqrt(float1); float2 = sqrt(float3); float4 = sqrt(float5)
- NOTE: if float0 and float2 are the same, the float2 operation is discarded and float0/float2 is equal to sqrt(float1)
- NOTE: if float2 and float4 are the same, the float4 operation is discarded and float2/float4 is equal to sqrt(float3)
- NOTE: if float0 and float4 are the same, the float4 operation is discarded and float0/float4 is equal to sqrt(float1)

#### 3.4.6.8. 11101: `flog` [S] - (A.6f) - Perform Logarithm on Floats

- if condition? float0 = log2(float1); float2 = log2(float3); float4 = log2(float5)
- NOTE: if float0 and float2 are the same, the float2 operation is discarded and float0/float2 is equal to log2(float1)
- NOTE: if float2 and float4 are the same, the float4 operation is discarded and float2/float4 is equal to log2(float3)
- NOTE: if float0 and float4 are the same, the float4 operation is discarded and float0/float4 is equal to log2(float1)

#### 3.4.6.9. 11110: `fpow` [S] - (A.6f) - Perform Exponentiation on Floats

- if condition? float0 = pow(float1, float2); float3 = pow(float4, float5)
- NOTE: if float0 and float3 are the same, the second exponentiation is discarded and float0/float3 is equal to pow(float1, float2)

#### 3.4.6.10. 11111: `ftype` [S] - (A.1f) - Determine Type of Float by Setting Status Flags

- if condition?
  - if float is non zero? status.zero = 0
  - if float is zero? status.zero = 1
  - if float is positive? status.sign = 0
  - if float is negative? status.sign = 1
  - if float is not infinite? status.carry = 0
  - if float is infinite? status.carry = 1
  - if float is not NaN? status.overflow = 0
  - if float is NaN? status.overflow = 1

# 4. Interrupts

- initially disabled, but can be enabled by setting `inthandler`

## 4.1. On Interrupt

- the previous `ip` and `status` is saved internally
- processor returns to system level and jumps to the address of `inthandler`
- after the interrupt handler finished, the previous `ip` and `status` are returned

## 4.2. Signaling Interrupts

### 4.2.1. Software Interrupts

- on write to `interrupt`, an interrupt is signaled

### 4.2.2. Hardware Interrupts

- if hardware requests an interrupt, the same process happens and the interrupt id is stored in `interrupt`

## 4.3. Exiting Interrupts

- the `inthandler` code must write a zero to `interrupt`, which will return `ip` and `status` to their previous state

## 4.4. Interrupt IDs

| id      | significance            |
| ------- | ----------------------- |
|       0 | none                    |
|       1 | divide by zero          |
|       2 | memory fault            |
|       3 | invalid permissions     |
|       4 | invalid instruction     |
|    5-32 | reserved faults         |
|      32 | device message          |
|   33-64 | reserved MMIO           |
|  64-... | ...                     |

# 5. Memory Mapped I/O

| range                   | significance          |
| ----------------------- | --------------------- |
| 0x0f0000000-0x0f0001000 | information directory |
| 0x0f0001000-0x100000000 | mapped device memory  |

## 5.1. MMIO Information Directory

| range         | significance                            |
| ------------- | --------------------------------------- |
| 0x0000-0x0004 | signaling device's mapped memory start  |
| 0x0004-0x0008 | signaling device's mapped memory extent |
| 0x0008-0x000c | signaling device's vendor identifier    |
| 0x000c-0x0010 | signaling device's category identifier  |
| 0x0010-0x0014 | signaling device's instance identifier  |
| 0x0014-0x0018 | signaling device's version              |
| 0x0018-0x001c | signaling device flags                  |

## 5.2. MMIO Category Identifiers

| id     | category        |
| ------ | --------------- |
| 0x0000 | unknown         |
| 0x0001 | HID device      |
| 0x0002 | video device    |
| 0x0003 | disk device     |
| 0x0004 | UART device     |

## 5.3. MMIO Device Flags

| bits  | significance     |
| ----- | ---------------- |
|     0 | device present   |
|  1-32 | ...              |
