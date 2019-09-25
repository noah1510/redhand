cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project("Greenfoot++")

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../include/)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -g -O0")
SET(GC_COVERAGE_COMPILE_FLAGS "-g -O0")
SET(GCC_COVERAGE_LINK_FLAGS "-lm -lGL -lglfw -lpthread -lX11 -lXcursor -ldl -lXi -lXxf86vm")


SET(GCC_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(main.out src/glad.c
                        src/shader.cpp
                        src/texture.cpp
                        src/objects.cpp
                        src/game.cpp
                        src/world.cpp
                        src/main.cpp)

target_link_libraries(main.out glfw)
