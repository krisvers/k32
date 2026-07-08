project('k32', 'cpp', default_options: ['cpp_std=c++20'])

executable('kemu',
    sources: [
        'src/kemu/memory.hpp',
        'src/kemu/uuid.hpp',
        'src/kemu/dev/device.hpp',
        'src/kemu/dev/cpu.hpp',
        'src/kemu/dev/gpu.hpp',
        'src/kemu/emulator.cpp',
    ],
    include_directories: include_directories('src'),
    override_options: ['cpp_std=c++20'],
)

executable('kasm',
    sources: [

    ],
    include_directories: include_directories('src'),
    override_options: ['cpp_std=c++20'],
)
