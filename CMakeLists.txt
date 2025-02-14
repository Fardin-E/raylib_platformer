cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# MSVC specific settings
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/arch:AVX2)
endif()

# Set the project name
project(raylib_platformer)

# Add Raylib as a subdirectory
add_subdirectory(thirdparty/raylib)

# Find all source files in the src directory
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Add the executable target with all found source files
add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES})

# Copy assets folder to the build directory
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"              # Path to assets folder in your source directory
    "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"  # Destination in the build directory
)


# Link the executable with Raylib
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE raylib)


# Include directories
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)