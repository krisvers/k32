fib(9)
while true

fib(a)
    if a == 0 or a == 1
        return a

    return fib(a - 1) + fib(a - 2)

----

r0 = 9
fib()
while true

fib()
    if r0 == 0 or r0 == 1
        r0 = r0
        return

    r0 = r0 - 1
    push r0 - #A    // [r31] = r0
                    // r31 -= 4
    fib()

    push r0 - #B    // [r31] = r0
                    // r31 -= 4

    r0 = #A         // r0 = [r31 + 8]
    r0 = r0 - 1

    fib()

    r1 = #B         // r0 = [r31 + 4]
    r0 = r0 + r1

    pop             // r31 += 4
    pop             // r31 += 4

    return

----

    boot
// setup paging
    // r0 - page directory address (0x00200000-0x00200400)
0x0000000c: li r0, 0x0020                   00000 1111
0x00000010: sh r0, 0x10; left               00000 1111
    // r1 - page table address (0x00200400-0x00200800)
0x00000014: li r1, 0x0020                   00000 1111
0x00000000: sh r1, 0x10; left               00000 1111
0x00000000: li r1, 0x0400                   00000 1111
    // r2 - page table entry 256 configuration (enabled/read/write/exec/user for paging section [0x00100000:0x00100000])
0x00000000: li r2, 0x0010                   00000 1111
0x00000000: sh r2, 0x10; left               00000 1111
0x00000000: li r2, 0x001f                   00000 1111
    // r3 - user code entry 0 configuration (enabled/read/write/exec/user for user code section [0x00300000:0x00000000])
0x00000000: li r3, 0x0030                   00000 1111
0x00000000: sh r3, 0x10; left               00000 1111
0x00000000: li r3, 0x001f                   00000 1111
    // r4 - stack entry 255 configuration (enabled/read/write/exec/user for stack section [0x003ff000:0x000ff000])
0x00000000: li r4, 0x003f                   00000 1111
0x00000000: sh r4, 0x10; left               00000 1111
0x00000000: li r4, 0xf01f                   00000 1111
    // store page table and page table entries
0x00000000: smr [r0, +0x000], r1            00000 1111
0x00000000: smr [r1, +0x100], r2            00000 1111
0x00000000: smr [r1, +0x000], r3            00000 1111
0x00000000: smr [r1, +0x0ff], r4            00000 1111
    // setup stack
0x00000000: li r30, 0x000f                  00000 1111
0x00000000: sh r30, 0x10; left              00000 1111
0x00000000: li r30, 0xf000                  00000 1111
    // setup jump to user code
0x00000000: and r31, r31, 0x000             00000 1111
    // set mmtable
0x00000000: spi mmtable, r0; write          00000 1111
    // jump to user code
0x00000000: jr r31, +0x0000                 00000 1111


      main
0x00001000: li r0, 0x0009           00000 1111 00000 __ 0x0009              0x000901e0
0x00001004: li r30, 0xfffc          00000 1111 11111 __ 0xfffc              0xfffc3fe0
0x00001008: jip +0x08               10110 1111 ___ 0x008                    0x000081f6
0x0000100c: jip +0x00               10110 1111 ___ 0x000                    0x000001f6
       fib
0x00001010: uadd r0, r0, 0x00       00110 1111 00000 00000 _ 0x000          0x000001e6
0x00001014: ifz jr r31, +0x0000     10101 0000 11110 __ 0x0000              0x00003c15
0x00001018: usub r0, r0, 0x01       01000 1111 00000 00000 _ 0x001          0x001001e8
0x0000101c: ifnz jip +0x0c          10110 0001 ___ 0x0000c                  0x0000c036
0x00001020: uadd r0, r0, 0x01       00110 1111 00000 00000 _ 0x001          0x001001e6
0x00001024: jr r31, +0x0000         10101 1111 11110 __ 0x0000              0x00007df5
0x00001028: usub r30, r30, 0x08     01000 1111 11111 11111 _ 0x008          0x008fffe8
0x0000102c: smr [r30, +0x08], r31   00011 1111 11110 11111 _ 0x008          0x0087fde3
0x00001030: smr [r30, +0x04], r0    00011 1111 00000 11111 _ 0x004          0x0047c1e3
0x00001038: jip -0x24               10110 1111 ___ 0xfffda                  0xfffda1f6
0x0000103c: uadd r1, r0, 0x00       00110 1111 00001 00000 _ 0x000          0x000003e6
0x00001040: lmr r0, [r30, +0x04]    00001 1111 00000 11111 _ 0x004          0x0047c1e1
0x00001044: smr [r30, +0x04], r1    00011 1111 00001 11111 _ 0x004          0x0047c3e3
0x00001048: usub r0, r0, 0x01       01000 1111 00000 00000 _ 0x001          0x001001e8
0x0000104c: jip -0x3c               10110 1111 ___ 0xfffc2                  0xfffc21f6
0x00001050: lmr r1, [r30, +0x04]    00001 1111 00001 11111 _ 0x004          0x0047c3e1
0x00001054: add4 r0, r0, r0, r1     00101 1111 00000 00000 00000 00001 ___  0x010001e5
0x00001058: lmr r31, [r30, +0x08]   00001 1111 11110 11111 _ 0x008          0x0087fde1
0x0000105c: uadd r30, r30, 0x08     00110 1111 11111 11111 _ 0x008          0x0087ffe6
0x00001060: jr r31, +0x0000         10101 1111 11110 __ 0x0000              0x00003df5
