cmake_minimum_required (VERSION 2.8.12)

project (can_bootloader C ASM)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib/")

set(
    CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} \
    -g \
    -fno-PIC \
    -no-pie \
    -Wall \
    -Wextra \
    -std=c99 \
    -pedantic \
    -Wmissing-prototypes \
    -Wstrict-prototypes \
    -Wold-style-definition"
)

include_directories(
    ./include
)

add_executable(
    traceback_test
    examples/traceback_test.c
)

add_executable(
    division_by_zero
    examples/division_by_zero.c
)

add_custom_target(
    sclean
    COMMAND rm
        "-rf"
        "bin"
        "cmake_install.cmake"
        "CMakeFiles"
        "CMakeCache.txt"
        "lib"
        "Makefile"
)
