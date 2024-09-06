cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC specific settings
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/arch:AVX2)
endif()

# Set the project name
project(raylib_platformer)

# Add Raylib as a subdirectory
add_subdirectory(thirdparty/raylib-5.0)
add_subdirectory(thirdparty/box2d)

# Find all source files in the src directory
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Add the executable target with all found source files
add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES})

# Link the executable with Raylib
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE raylib)

#Link the box2d library
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE box2d::box2d)

# Include directories
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)