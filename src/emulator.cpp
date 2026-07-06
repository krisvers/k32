#include "k32/cpu.hpp"
#include "k32/gpu.hpp"
#include "k32/memory.hpp"

using namespace k32::memory_literals;

int main(int argc, char** argv) {
    k32::CPU cpu = k32::CPU(4_M);
    k32::GPU gpu = k32::GPU(1_M, 256_K);
    cpu.addChildDevice(&gpu);

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
