cmake_minimum_required (VERSION 3.5)

option(EXECNAME "execname" "defaultName")
set(executableName ${EXECNAME})

set (PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

set (SOURCE 
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/gpio.c
    ${PROJECT_SOURCE_DIR}/test.c
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
    set(executableName "${executableName}_local")
endif()
add_executable(${executableName} ${SOURCE} ${HEADERS})
include_directories(${PROJECT_BINARY_DIR})