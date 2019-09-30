cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_policy(SET CMP0072 NEW)
cmake_policy(VERSION 3.12)

project("Greenfoot++")

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../include/)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../lib/)

if (UNIX)
    SET(OUTPUT "main.out")
endif (UNIX)

if (WIN32)
    SET(OUTPUT "main.exe")
endif (WIN32)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -g -O0")
SET(GC_COVERAGE_COMPILE_FLAGS "-g -O0")
SET(GCC_COVERAGE_LINK_FLAGS "-pthread -ldl -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11")


SET(GCC_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

find_package(OpenGL REQUIRED)

add_executable(${OUTPUT} src/glad.c
                        src/shader.cpp
                        src/texture.cpp
                        src/objects.cpp
                        src/game.cpp
                        src/world.cpp
                        src/main.cpp)

if (UNIX)
    target_link_libraries(${OUTPUT} libglfw.so)
endif (UNIX)

if (WIN32)
    target_link_libraries(${OUTPUT} glfw3.dll)
endif (WIN32)

target_link_libraries(${OUTPUT} dl)
