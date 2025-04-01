cmake_minimum_required(VERSION 3.20.0...4.0.0)
project(TND004-Lec-1 VERSION 1.0.0 DESCRIPTION "TND004 Lecture 1" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(enable_warnings target)
    target_compile_options(${target} PUBLIC 
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4                 # Enable the highest warning level
            /w44388             # Enable 'signed/unsigned mismatch' '(off by default)
            /we4715             # Turn 'not all control paths return a value' into a compile error
            /permissive-        # Stick to the standard
			/fsanitize=address  # Enable the Address Sanatizer, helps finding bugs at runtime
            >
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra>
    )
endfunction()

add_executable(Factorial factorial.cpp)
add_executable(Fibonacci fibonacci.cpp)
add_executable(Frequency findFrequency.cpp)

enable_warnings(Factorial)
enable_warnings(Fibonacci)
enable_warnings(Frequency)