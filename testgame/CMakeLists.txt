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

include_directories(${CMAKE_PREFIX_PATH}/include/ /usr/include/GraphicsMagick)
link_directories(${CMAKE_PREFIX_PATH}/lib/)

if (UNIX)
    if (OUTPUTFILE)
        SET(OUTPUT ${OUTPUTFILE})
    else (OUTPUTFILE)
        SET(OUTPUT "main.out")
    endif (OUTPUTFILE)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread -ldl -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11")
    SET(GCC_COVERAGE_LINK_FLAGS "${GCC_COVERAGE_LINK_FLAGS} -I/usr/include/GraphicsMagick -L/usr/lib -L/usr/lib/X11 -lGraphicsMagick++ -lGraphicsMagick -ljbig -lwebp -lwebpmux -llcms2 -ltiff -lfreetype -ljpeg -lpng16 -lwmflite -lXext -lSM -lICE -lX11 -llzma -lbz2 -lxml2 -lz -lm -lgomp -lpthread")
endif (UNIX)

if (WIN32)
    if (OUTPUTFILE)
        SET(OUTPUT ${OUTPUTFILE})
    else (OUTPUTFILE)
        SET(OUTPUT "main")
    endif (OUTPUTFILE)
    SET(GCC_COVERAGE_LINK_FLAGS "-pthread")
endif (WIN32)

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
${CMAKE_PREFIX_PATH}/testgame/src/test_world.cpp
${CMAKE_PREFIX_PATH}/testgame/src/test_objects.cpp
${CMAKE_PREFIX_PATH}/testgame/src/main.cpp)

if (UNIX)
    find_library(GLFW
        NAMES libglfw.so.3 libglfw.so glfw libglfw.so.3.2
        HINTS /usr/lib/x86_64-linux-gnu/ ${CMAKE_PREFIX_PATH}/deploy/)
    
    target_link_libraries(${OUTPUT} ${GLFW})
    target_link_libraries(${OUTPUT} dl)
    
    if(REDHAND_LOCATION)
        message("using custom REDHAND_LOCATION")
        target_link_libraries(${OUTPUT} ${CMAKE_PREFIX_PATH}/${REDHAND_LOCATION})
    else()
        find_library(REDHAND
            NAMES libredhand.so redhand
            HINTS "${CMAKE_PREFIX_PATH}/lib/" "${CMAKE_PREFIX_PATH}/deploy/" "${CMAKE_PREFIX_PATH}")   

        target_link_libraries(${OUTPUT} ${REDHAND})
    endif(REDHAND_LOCATION)
endif (UNIX)

if (WIN32)
    target_link_libraries(${OUTPUT} "${CMAKE_PREFIX_PATH}/lib/glfw3.dll") 

    target_link_libraries(${OUTPUT} "${CMAKE_PREFIX_PATH}/lib/libredhand.dll")
endif (WIN32)
