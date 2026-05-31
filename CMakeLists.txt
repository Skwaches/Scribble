cmake_minimum_required(VERSION 4.2)
project(scribble)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../.Vendor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(VENDORED "Use personal libraries" ON)

#Dependencies
if(VENDORED)
	add_subdirectory(${VENDOR_DIR}/SDL SDL)
else()
	find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

set(executable_Name ${PROJECT_NAME})
add_executable(${executable_Name} 
	src/grid.cpp
	src/init.cpp
	src/input.cpp
	src/machine.cpp
	src/matrix.cpp
	src/read.cpp
	src/render.cpp
	src/window.cpp

)

target_include_directories(${executable_Name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${executable_Name} PRIVATE SDL3::SDL3) 
