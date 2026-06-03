cmake_minimum_required(VERSION 3.20)

if(CMAKE_GENERATOR STREQUAL "Xcode" AND APPLE)
    execute_process(
        COMMAND xcrun --find clang
        OUTPUT_VARIABLE XENGINE_XCODE_C_COMPILER
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    execute_process(
        COMMAND xcrun --find clang++
        OUTPUT_VARIABLE XENGINE_XCODE_CXX_COMPILER
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(XENGINE_XCODE_C_COMPILER)
        set(CMAKE_C_COMPILER "${XENGINE_XCODE_C_COMPILER}" CACHE FILEPATH "C compiler for Xcode generator" FORCE)
    endif()

    if(XENGINE_XCODE_CXX_COMPILER)
        set(CMAKE_CXX_COMPILER "${XENGINE_XCODE_CXX_COMPILER}" CACHE FILEPATH "CXX compiler for Xcode generator" FORCE)
    endif()

    set(CMAKE_C_COMPILER_ID "AppleClang" CACHE INTERNAL "C compiler ID for Xcode generator")
    set(CMAKE_CXX_COMPILER_ID "AppleClang" CACHE INTERNAL "CXX compiler ID for Xcode generator")
    set(CMAKE_C_COMPILER_FORCED TRUE CACHE INTERNAL "Skip C compiler try-compile for Xcode generator")
    set(CMAKE_CXX_COMPILER_FORCED TRUE CACHE INTERNAL "Skip CXX compiler try-compile for Xcode generator")
endif()

project(XEngineRoot LANGUAGES C CXX)

#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:45

##message(STATUS "os : ${CMAKE_HOST_SYSTEM_NAME}")
##if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
##    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
##    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /GR")
##    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Windows/")
##    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Windows/")
##elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
##    option(USE_32BITS "Compile for 32bits processors(linux only)" OFF)
##    set(CMAKE_CXX_FLAGS "-g -fuse-ld=gold -std=c++11 -Wall -lpthread -rdynamic -ldl -DLinux")
##    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -fsanitize=leak")
##    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-o2 -D_DEBUG -fsanitize=leak")
##    set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG")
##    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Linux/")
##    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Linux/")
##endif()

set(XENGINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB Api "${CMAKE_CURRENT_SOURCE_DIR}/Api/*.*")
file(GLOB_RECURSE PhysX "${CMAKE_CURRENT_SOURCE_DIR}/Api/PhysX/*.*")
file(GLOB Geometry "${CMAKE_CURRENT_SOURCE_DIR}/Utils/Geometry/*.*")
file(GLOB TinyXML "${CMAKE_CURRENT_SOURCE_DIR}/Utils/TinyXML/*.*")
file(GLOB JsonCpp "${CMAKE_CURRENT_SOURCE_DIR}/Utils/JsonCpp/*.*")
file(GLOB Utils "${CMAKE_CURRENT_SOURCE_DIR}/Utils/*.*")
file(GLOB Excel "${CMAKE_CURRENT_SOURCE_DIR}/Utils/Excel/*.*")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine)
