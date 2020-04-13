cmake_minimum_required(VERSION 3.10)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

if(CMAKE_VERSION VERSION_GREATER "3.11.0")
    cmake_policy(SET CMP0072 NEW)
    cmake_policy(VERSION 3.11)
endif()

project("redhand-testgame")

option(OUTPUTFILE "Name of file to output")
option(REPOROOT "Path of the root of the repo")
option(REDHAND_LOCATION "The exact locataion of the libredhand file relative to reporoot")

if (REPOROOT)
    SET(CMAKE_PREFIX_PATH ${REPOROOT})
else (REPOROOT)
    SET(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/..)
endif (REPOROOT)

include_directories(${CMAKE_PREFIX_PATH}/include/)
link_directories(${CMAKE_PREFIX_PATH}/lib/)

if (UNIX)
    if (OUTPUTFILE)
        SET(OUTPUT ${OUTPUTFILE})
    else (OUTPUTFILE)
        SET(OUTPUT "main.out")
    endif (OUTPUTFILE)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread -ldl -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11")
endif (UNIX)

if (WIN32)
    if (OUTPUTFILE)
        SET(OUTPUT ${OUTPUTFILE})
    else (OUTPUTFILE)
        SET(OUTPUT "main")
    endif (OUTPUTFILE)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread")
endif (WIN32)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -g -O0 -Wall -Wextra")
SET(GC_COVERAGE_COMPILE_FLAGS "-g -O0")



SET(GCC_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(GCC_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

find_package(OpenGL REQUIRED)


add_executable(${OUTPUT} 
${CMAKE_PREFIX_PATH}/testgame/src/game.cpp
${CMAKE_PREFIX_PATH}/testgame/src/main.cpp)

if (UNIX)
    find_library(GLFW
        NAMES libglfw.so.3 libglfw.so glfw libglfw.so.3.2
        HINTS /usr/lib/x86_64-linux-gnu/ ${CMAKE_PREFIX_PATH}/deploy/)
    
    target_link_libraries(${OUTPUT} ${GLFW})
    target_link_libraries(${OUTPUT} libopenal.so libopenal.so.1.20.1)
    target_link_libraries(${OUTPUT} dl)
    
    if(REDHAND_LOCATION)
        message("using custom REDHAND_LOCATION")
        target_link_libraries(${OUTPUT} ${CMAKE_PREFIX_PATH}/${REDHAND_LOCATION})
    else()
        find_library(REDHAND
            NAMES libredhand.so redhand
            HINTS "${CMAKE_PREFIX_PATH}/lib/")   

        target_link_libraries(${OUTPUT} ${REDHAND})
    endif(REDHAND_LOCATION)
    #target_link_libraries(${OUTPUT} sfml-graphics sfml-audio sfml-window sfml-system)
endif (UNIX)

if (WIN32)
    target_link_libraries(${OUTPUT} glfw3.dll)
    target_link_libraries(${OUTPUT} libredhand.dll)
    #target_link_libraries(${OUTPUT} sfml-graphics-2.dll sfml-audio-2.dll sfml-window-2.dll sfml-system-2.dll)
endif (WIN32)
