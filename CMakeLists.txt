cmake_minimum_required (VERSION 3.5)
project (autochess)
option(EXECNAME "execname" "defaultName")
set(executableName ${EXECNAME})

set (PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

add_subdirectory(board)
add_subdirectory(gpio)
add_subdirectory(motor)

set (SOURCE 
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/gpio.c
)
set (HEADERS 
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/gpio.h
)

#call cmake -D PIBUILD:bool=true to build for pi
option(PIBUILD "pibuild" off)

option(DEFINE_DEBUG "preprocess with #define debug" off)
if(${DEFINE_DEBUG})
    message("- building debug")
    ADD_DEFINITIONS(-Ddebug)
endif()
if(${PIBUILD})
    message("- Building for rpi")
    include($ENV{HOME}/pi/pi.cmake)
else()
    message("- Building for local enviroment")
endif()
add_executable(${executableName} ${SOURCE} ${HEADERS})
include_directories(${PROJECT_BINARY_DIR})