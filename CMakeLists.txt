cmake_minimum_required(VERSION 3.20)
project(pikolang)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


include_directories(
    ./include/
)


add_executable(
    pikoc
    src/main.cpp
    src/tokenizer.cpp
    src/files.cpp
    src/print_tokens.cpp
    src/errors.cpp
)
