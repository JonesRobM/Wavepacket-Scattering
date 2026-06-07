cmake_minimum_required(VERSION 3.10)
project(QuantumScattering VERSION 1.0.0 LANGUAGES CXX)

# Enforce C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings to help catch potential issues
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

# Define the executable target and specify the source files


add_executable(QuantumScattering
    src/main.cpp
    src/simulation.cpp
    src/simulation.hpp
)

# Specify the include directory for compilation
target_include_directories(QuantumScattering PRIVATE src)