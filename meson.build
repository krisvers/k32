project('k32', 'cpp', default_options: ['cpp_std=c++20'])

executable('k32',
    sources: [
        'src/k32/memory.hpp',
        'src/k32/uuid.hpp',
        'src/k32/dev/device.hpp',
        'src/k32/dev/cpu.hpp',
        'src/k32/dev/gpu.hpp',
        'src/emulator.cpp',
    ],
    include_directories: include_directories('src'),
    override_options: ['cpp_std=c++20'],
)
