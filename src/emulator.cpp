#include "k32/memory.hpp"
#include "k32/dev/cpu.hpp"
#include "k32/dev/gpu.hpp"

using namespace k32::memory_literals;

int main(int argc, char** argv) {
    k32::dev::CPU cpu = k32::dev::CPU(4_MB);
    k32::dev::GPU gpu = k32::dev::GPU(1_MB, 256_KB);
    cpu.addChildDevice(&gpu);

    k32::IMemoryDevice* cpuMMU = cpu.getExternalMemoryDevice();
    cpuMMU->word(0x00000000, 0x000901e0);
    cpuMMU->word(0x00000004, 0x000c3de0);
    cpuMMU->word(0x00000008, 0x000081f6);
    cpuMMU->word(0x0000000c, 0x000001f6);
    cpuMMU->word(0x00000010, 0x00003c15);
    cpuMMU->word(0x00000014, 0x001001e4);
    cpuMMU->word(0x00000018, 0x0000c036);
    cpuMMU->word(0x0000001c, 0x001001e6);
    cpuMMU->word(0x00000020, 0x00007df5);
    cpuMMU->word(0x00000024, 0x0087ffe4);
    cpuMMU->word(0x00000028, 0x0087bfe3);
    cpuMMU->word(0x0000002c, 0x008781e3);
    cpuMMU->word(0x00000030, 0x00383de0);
    cpuMMU->word(0x00000034, 0xfffda1f6);
    cpuMMU->word(0x00000038, 0x000003e6);
    cpuMMU->word(0x0000003c, 0x0047c1e1);
    cpuMMU->word(0x00000040, 0x0047c3e3);
    cpuMMU->word(0x00000044, 0x001001e4);
    cpuMMU->word(0x00000048, 0x00503de0);
    cpuMMU->word(0x0000004c, 0xfffc21f6);
    cpuMMU->word(0x00000050, 0x0047c3e1);
    cpuMMU->word(0x00000054, 0x080001e5);
    cpuMMU->word(0x0000005c, 0x0087fde1);
    cpuMMU->word(0x00000060, 0x00803fe6);
    cpuMMU->word(0x00000064, 0x00003df5);

    bool running = true;
    while (running) {
        if (!cpu.execute()) {
            break;
        }

        if (!gpu.execute()) {
            break;
        }
    }

    return 0;
}
