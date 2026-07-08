#include "memory.hpp"
#include "dev/cpu.hpp"
#include "dev/gpu.hpp"

using namespace kemu::memory_literals;

int main(int argc, char** argv) {
    kemu::dev::CPU cpu = kemu::dev::CPU(4_MB);
    kemu::dev::GPU gpu = kemu::dev::GPU(1_MB, 256_KB);
    cpu.addChildDevice(&gpu);

    kemu::IMemoryDevice* cpuMMU = cpu.getExternalMemoryDevice();


    /* user-level code */
    cpuMMU->word(0x00001000, 0x000601e0);
    cpuMMU->word(0x00001004, 0x00103de0);
    cpuMMU->word(0x00001008, 0xfffc3fe0);
    cpuMMU->word(0x0000100c, 0x000081f6);
    cpuMMU->word(0x00001010, 0x000001f6);
    cpuMMU->word(0x00001014, 0x000001e6);
    cpuMMU->word(0x00001018, 0x00003c15);
    cpuMMU->word(0x0000101c, 0x001001e8);
    cpuMMU->word(0x00001020, 0x0000c036);
    cpuMMU->word(0x00001024, 0x001001e6);
    cpuMMU->word(0x00001028, 0x00007df5);
    cpuMMU->word(0x0000102c, 0x008fffe8);
    cpuMMU->word(0x00001030, 0x0087fde3);
    cpuMMU->word(0x00001034, 0x0047c1e3);
    cpuMMU->word(0x00001038, 0x00403de0);
    cpuMMU->word(0x0000103c, 0xfffda1f6);
    cpuMMU->word(0x00001040, 0x000003e6);
    cpuMMU->word(0x00001044, 0x0047c1e1);
    cpuMMU->word(0x00001048, 0x0047c3e3);
    cpuMMU->word(0x0000104c, 0x001001e8);
    cpuMMU->word(0x00001050, 0x00583de0);
    cpuMMU->word(0x00001054, 0xfffc21f6);
    cpuMMU->word(0x00001058, 0x0047c3e1);
    cpuMMU->word(0x0000105c, 0x010001e5);
    cpuMMU->word(0x00001060, 0x0087fde1);
    cpuMMU->word(0x00001064, 0x0087ffe6);
    cpuMMU->word(0x00001068, 0x00003df5);

    /* page directory */
    cpuMMU->word(0x00002000, 0x00003000);

    /* page table */
    cpuMMU->word(0x00003000, 0x0000001f);
    cpuMMU->word(0x00003ffc, 0x0001001f);

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
