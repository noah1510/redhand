cmake_minimum_required(VERSION 3.10)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_policy(SET CMP0067 NEW)

if(CMAKE_VERSION VERSION_GREATER "3.11.0")
    cmake_policy(SET CMP0072 NEW)
    cmake_policy(VERSION 3.11)
endif()

project(redhand VERSION "0.0.11" DESCRIPTION "a small 2D game engine")

set(CMAKE_CXX_STANDARD 17)

option(OUTPUTFILE "Name of file to output")
option(REPOROOT "Path of the root of the repo" "${CMAKE_CURRENT_BINARY_DIR}/..")
option(BUILDGLFW "allow manual compiling of glfw" "1")

SET(CMAKE_PREFIX_PATH ${REPOROOT})

if (${BUILDGLFW} EQUAL  "0")
    find_package(glfw3 REQUIRED)
else()
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory("dependencies/glfw/")
endif()


include_directories(${CMAKE_PREFIX_PATH}/include/)
link_directories(${CMAKE_PREFIX_PATH}/lib/)

set(CMAKE_REQUIRED_FLAGS "-std=c++11")

find_package(OpenGL REQUIRED)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("filesystem" HAVE_FILESYSTEM)

if(!HAVE_FILESYSTEM)
    message( FATAL_ERROR "compiler does not have c++ <filesystem> support please install a newer version." )
endif()

if (UNIX)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread -ldl -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11")
endif (UNIX)

if (WIN32)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread")
endif (WIN32)

SET(GCC_COVERAGE_COMPILE_FLAGS " -std=c++17 -g -O0 -Wall -Wextra")
SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -g -O0")

SET(GCC_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_library(redhand SHARED
                        src/glad.c
                        src/math.cpp
                        src/shader.cpp
                        src/texture.cpp
                        src/game_object.cpp
                        src/complex_world.cpp
                        src/input.cpp
                        src/engine.cpp)

target_link_libraries(redhand ${OpenCV_LIBRARIES})
target_link_libraries(redhand OpenGL::GL)
target_link_libraries(redhand glfw)


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
