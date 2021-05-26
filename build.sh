cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=toolchain-STM32F407.cmake
cmake --build build -- VERBOSE=1
