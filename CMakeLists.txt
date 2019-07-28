cmake_minimum_required(VERSION 3.14)
project(sxt_reader C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER afl-gcc)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -fsanitize=address")
#set(CMAKE_CXX_COMPILER afl-clang-fast++)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")


file(MAKE_DIRECTORY cmake-build-debug/fuzz_in)
configure_file(input_examples/admin.sxt fuzz_in/admin.sxt COPYONLY)
configure_file(input_examples/user.sxt fuzz_in/user.sxt COPYONLY)

file(MAKE_DIRECTORY cmake-build-debug/fuzz_out)

add_executable(sxt_reader src/main.c src/sxtFileLoader.c src/sxtFileLoader.h src/sxtParser.c src/sxtParser.h)