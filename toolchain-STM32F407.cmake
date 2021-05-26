# set CMAKE_SYSTEM_NAME to define build as CMAKE_CROSSCOMPILING
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION Cortex-M4-STM32F407)
set(CMAKE_SYSTEM_PROCESSOR arm)

# settinge either of the compilers builds the absolute paths for the other tools:
#   ar, nm, objcopy, objdump, ranlib, readelf -- but not as, ld, size
# if the compiler cannot be found the try_compile() function will fail the build
# set(CMAKE_C_COMPILER arm-none-eabi-gcc)
# set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# we need to get compiler path itself, for the toolchain
# but still rely on try_compile to check we have a valid compiler

find_program(CROSS_GCC_PATH arm-none-eabi-gcc)
get_filename_component(TOOLCHAIN ${CROSS_GCC_PATH} PATH)

set(CMAKE_C_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc)
set(CMAKE_Cxx_COMPILER ${TOOLCHAIN}/arm-none-eabi-g++)
set(TOOLCHAIN_as ${TOOLCHAIN}/arm-none-eabi-as CACHE STRING "arm-none-eabi-as")
set(TOOLCHAIN_LD ${TOOLCHAIN}/arm-none-eabi-ld CACHE STRING "arm-none-eabi-ld")
set(TOOLCHAIN_SIZE ${TOOLCHAIN}/arm-none-eabi-size CACHE STRING "arm-none-eabi-size")

# --specs=nano.specs is both a compiler and linker option
set(ARM_OPTIONS -mcpu=cortex-m4 -mfloat-abi=soft --specs=nano.specs)

add_compile_options(
  ${ARM_OPTIONS}
  -fmessage-length=0
  -funsigned-char
  -ffunction-sections
  -fdata-sections
  -MMD
  -MP)

add_compile_definitions(
  STM32F407xx
  USE_FULL_ASSERT
  OS_USE_TRACE_SEMIHOSTING_STDOUT
  OS_USE_SEMIHOSTING
)

# use this to avoid running the linker during test compilation
# set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# use these options to verify the linker can create an ELF file
# when not doing a static link

add_link_options(
  ${ARM_OPTIONS}
  --specs=rdimon.specs
  -u_printf_float
  -u_scanf_float
  -nostartfiles
  LINKER:--gc-sections
  LINKER:--build-id)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

