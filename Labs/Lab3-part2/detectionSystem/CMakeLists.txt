cmake_minimum_required(VERSION 3.21.0...4.0.0)
project(TND004-Lab-3-part2 VERSION 1.0.0 DESCRIPTION "TND004 Lab 3: part2" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Rendering	
	renderingSystem/include/readfiles.h 
    renderingSystem/include/window.h  
	renderingSystem/src/readfiles.cpp 
    renderingSystem/src/window.cpp
	renderingSystem/src/lab3-part2.cpp	# main   
)

add_executable(LinesDiscovery	
    #include/file1.h	 # ADD other header files, if needed
	linesdiscoverysystem/include/find-patterns.hpp
    #src/file1.cpp		#ADD other source files, if needed
	linesdiscoverysystem/src/find-patterns.cpp	# main   
)

target_include_directories(Rendering PUBLIC "renderingSystem/include")
target_include_directories(LinesDiscovery PUBLIC "linesdiscoverysystem/include")

target_compile_options(Rendering PUBLIC 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra>
)

target_compile_options(LinesDiscovery PUBLIC 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra>
)

# External libraries
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(Rendering PUBLIC glm::glm fmt::fmt glad::glad glfw)

target_compile_definitions(Rendering PRIVATE DATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/data\")
target_compile_definitions(LinesDiscovery PRIVATE DATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/data\")