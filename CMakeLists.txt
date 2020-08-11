#specify cmake version and flags
cmake_minimum_required(VERSION 3.16)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_policy(SET CMP0067 NEW)

cmake_policy(SET CMP0072 NEW)
cmake_policy(VERSION 3.16)

#the project name and version
project(redhand VERSION "0.0.12" DESCRIPTION "a small 2D game engine")

#the available build options
option(OUTPUTFILE "Name of file to output")
option(REPOROOT "Path of the root of the repo" "${CMAKE_CURRENT_BINARY_DIR}/..")
option(BUILDGLFW "allow manual compiling of glfw" "1")

#set the reporoot var
SET(CMAKE_PREFIX_PATH ${REPOROOT})
message("Repo base path is ${CMAKE_PREFIX_PATH}")

#add glfw to build if needed
if (${BUILDGLFW} EQUAL  "0")
    find_package(glfw3 REQUIRED)
else()
    message("Manually building glfw")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory("dependencies/glfw/")
endif()

add_subdirectory("dependencies/openal/")

#set c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#include and link project folders
include_directories(${CMAKE_PREFIX_PATH}/include/)
link_directories(${CMAKE_PREFIX_PATH}/lib/)

#search for opengl
find_package(OpenGL REQUIRED)

#search for glib and vips
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(VIPS vips-cpp REQUIRED)

#Search for filesystem header
include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("filesystem" HAVE_FILESYSTEM)

if(NOT HAVE_FILESYSTEM)
    message( FATAL_ERROR "compiler does not have c++ <filesystem> support please install a newer version." )
endif()

# Set output compiler flags
if (UNIX)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread -ldl -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11")
endif (UNIX)

if (WIN32)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread")
endif (WIN32)

SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -Wall -Wextra")
SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -g -O0")

SET(GCC_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

#add executable name
add_library(redhand SHARED
    src/glad.c
    src/math.cpp
    src/shader.cpp
    src/texture.cpp
    src/game_object.cpp
    src/complex_world.cpp
    src/actor.cpp
    src/input.cpp
    src/engine.cpp
    src/audio.cpp)

#include glib and link libraries
#target_precompile_headers(redhand PRIVATE ${CMAKE_PREFIX_PATH}/include/redhand/types.hpp)
target_include_directories(redhand PRIVATE ${CMAKE_PREFIX_PATH}/include/ ${CMAKE_PREFIX_PATH}/include/redhand ${CMAKE_PREFIX_PATH}/include/AL)
target_include_directories(redhand PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(redhand ${GLIB_LDFLAGS})
target_link_libraries(redhand OpenGL::GL)
target_link_libraries(redhand glfw)
#target_link_libraries(redhand lib/dependencies/openal/libopenal.so.1)
target_include_directories(redhand PRIVATE ${VIPS_INCLUDE_DIRS})
target_link_libraries(redhand ${VIPS_LDFLAGS})

if (UNIX)
    target_link_libraries(redhand dl)
endif (UNIX)

if (WIN32)
    #target_link_libraries(redhand ${CMAKE_PREFIX_PATH}/lib/glfw3.dll)
endif (WIN32)

install(TARGETS redhand
        LIBRARY DESTINATION .
        PUBLIC_HEADER DESTINATION ${CMAKE_PREFIX_PATH}/include/redhand)

file(GLOB HEADERS src/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_PREFIX_PATH}/include/redhand)
