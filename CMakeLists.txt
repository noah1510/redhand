#specify cmake version and flags
cmake_minimum_required(VERSION 3.16)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_policy(SET CMP0067 NEW)

cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0069 NEW)
cmake_policy(VERSION 3.16)

#the project name and version
project(redhand VERSION 0.1.1 LANGUAGES C CXX DESCRIPTION "a small 2D game engine")

#enable link time optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

#the available build options
option(AUTOSETUP "automatically call setup.sh if needed" ON)

#set the reporoot var
message("Repo base path is ${CMAKE_SOURCE_DIR}")

#execute setup.sh if needed
if(AUTOSETUP)
    if(EXISTS ${CMAKE_SOURCE_DIR}/src/glad.c)
        message("glad.c found; not calling setup")
    else()
        message("glad.c not found; executing setup")
        execute_process(COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/setup.sh RESULT_VARIABLE setupOutput WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

        if(setupOutput MATCHES 0)
            message("setup sucessful")
        else()
            message( FATAL_ERROR "error during setup" )
        endif()
    endif()
endif()

#add glfw to build if needed
find_package(glfw3)
if(glfw3_FOUND)
    message("using installed version of glfw")
else()
    message("Manually building glfw")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory("dependencies/glfw/")
endif()

#set c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#include and link project folders
include_directories(${CMAKE_SOURCE_DIR}/include/)
link_directories(${CMAKE_SOURCE_DIR}/lib/)

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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 11.0)
        SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpch-instantiate-templates")
        SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fpch-instantiate-templates")
    endif()
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

#add executable name
add_library(redhand SHARED
    src/glad.c
    src/helper.cpp
    src/types.cpp
    src/shader.cpp
    src/texture.cpp
    src/game_object.cpp
    src/complex_world.cpp
    src/actor.cpp
    src/input.cpp
    src/engine.cpp)

#include glib and link libraries
#target_precompile_headers(redhand PRIVATE ${CMAKE_SOURCE_DIR}/include/redhand/helper.hpp)
target_include_directories(redhand PRIVATE ${CMAKE_SOURCE_DIR}/include/ ${CMAKE_SOURCE_DIR}/include/redhand)
target_include_directories(redhand PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(redhand ${GLIB_LDFLAGS})
target_link_libraries(redhand OpenGL::GL)
target_link_libraries(redhand glfw)
target_include_directories(redhand PRIVATE ${VIPS_INCLUDE_DIRS})
target_link_libraries(redhand ${VIPS_LDFLAGS})

if (UNIX)
    target_link_libraries(redhand dl)
endif (UNIX)

if (WIN32)
    #target_link_libraries(redhand ${CMAKE_SOURCE_DIR}/lib/glfw3.dll)
endif (WIN32)

install(TARGETS redhand
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_SOURCE_DIR}/include/redhand)

file(GLOB HEADERS src/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_SOURCE_DIR}/include/redhand)
