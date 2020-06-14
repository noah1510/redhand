cmake_minimum_required(VERSION 3.10)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

if(CMAKE_VERSION VERSION_GREATER "3.11.0")
    cmake_policy(SET CMP0072 NEW)
    cmake_policy(VERSION 3.11)
endif()

project(redhand VERSION "0.0.8" DESCRIPTION "a small 2D game engine")

option(OUTPUTFILE "Name of file to output")
option(REPOROOT "Path of the root of the repo")

if (REPOROOT)
    SET(CMAKE_PREFIX_PATH ${REPOROOT})
else (REPOROOT)
    SET(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/..)
endif (REPOROOT)

find_package(OpenGL REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

include_directories(${CMAKE_PREFIX_PATH}/include/ ${ImageMagick_INCLUDE_DIRS})
link_directories(${CMAKE_PREFIX_PATH}/lib/)

if (UNIX)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread -ldl -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11")
    SET(GCC_COVERAGE_LINK_FLAGS "${GCC_COVERAGE_LINK_FLAGS} -lGraphicsMagick++ -lGraphicsMagick -ljbig -lwebp -lwebpmux -llcms2 -ltiff -lfreetype -ljpeg -lpng16 -lwmflite -lXext -lSM -lICE -lX11 -llzma -lbz2 -lxml2 -lz -lm -lgomp -lpthread")
endif (UNIX)

if (WIN32)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread")
endif (WIN32)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -g -O0 -Wall -Wextra")
SET(GC_COVERAGE_COMPILE_FLAGS "-g -O0 -rdynamic")

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

target_link_libraries(redhand ${ImageMagick_LIBRARIES})

if (UNIX)
    find_library(GLFW
        NAMES libglfw.so.3 libglfw.so glfw libglfw.so.3.2
        HINTS /usr/lib/x86_64-linux-gnu/ )
    target_link_libraries(redhand ${GLFW})

    #find_library(MAGICK
    #    NAMES "GraphicsMagick++" "libGraphicsMagick++.so" "Magick++" "libMagick++.so"
    #    HINTS /usr/lib/ /usr/lib/x86_64-linux-gnu/ )
    #target_link_libraries(redhand ${MAGICK}) 
    target_link_libraries(redhand dl)
endif (UNIX)

if (WIN32)
    target_link_libraries(redhand ${CMAKE_PREFIX_PATH}/lib/glfw3.dll)
endif (WIN32)

install(TARGETS redhand
        LIBRARY DESTINATION .
        PUBLIC_HEADER DESTINATION ${CMAKE_PREFIX_PATH}/include/redhand)

file(GLOB HEADERS src/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_PREFIX_PATH}/include/redhand)
