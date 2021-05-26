cmake_minimum_required(VERSION 3.16)
project(simple-host LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Wall
    -Wextra
    -Wconversion
    -Wsign-conversion
    -g3
    -Og
)

add_compile_definitions(
    DEBUG
)

add_executable(Application 
    src/main.cpp
    system/src/cmsis/vectors_stm32f4xx.c
    system/src/cmsis/system_stm32f4xx.c
    system/src/cortexm/_initialize_hardware.c
    system/src/cortexm/_reset_hardware.c
    system/src/cortexm/exception_handlers.c
    system/src/diag/Trace.c
    system/src/diag/trace_impl.c
    system/src/newlib/__dso_handle.c
    system/src/newlib/_exit.c
    system/src/newlib/_sbrk.c
    system/src/newlib/_startup.c
    system/src/newlib/_syscalls.c
    system/src/newlib/_write.c
    system/src/newlib/assert.c
  )

set_target_properties(Application PROPERTIES
    SUFFIX .elf
)

target_link_options(Application PRIVATE
  -T${CMAKE_SOURCE_DIR}/ldscripts/mem.ld
  -T${CMAKE_SOURCE_DIR}/ldscripts/sections.ld
  LINKER:--print-memory-usage
  LINKER:-Map,${CMAKE_CURRENT_BINARY_DIR}/Application.map
)

target_include_directories(Application PRIVATE
  src
  system/include
  system/include/cmsis
)
