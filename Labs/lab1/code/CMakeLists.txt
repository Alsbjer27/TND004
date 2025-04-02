cmake_minimum_required(VERSION 3.20.0...4.0.0)
project(TND004-Lab-1 VERSION 1.0.0 DESCRIPTION "TND004 Lab 1" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(enable_warnings target)
    target_compile_options(${target} PUBLIC 
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4                 # Enable the highest warning level
            /w44388             # Enable 'signed/unsigned mismatch' (off by default)
            /we4715             # Turn 'not all control paths return a value' into a compile error
            /permissive-        # Stick to the standard
        >
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:
            -Wall -Wextra -fsanitize=address # ASan only for Clang/GCC
        >
    )
endfunction()

add_executable(Lab1 lab1.cpp test_data.txt test_result.txt)

enable_warnings(Lab1)
