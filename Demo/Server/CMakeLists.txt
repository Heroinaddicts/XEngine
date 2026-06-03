cmake_minimum_required(VERSION 3.24)
if(CMAKE_GENERATOR STREQUAL "Xcode" AND APPLE)
    execute_process(
        COMMAND xcrun --find clang
        OUTPUT_VARIABLE XGAME_XCODE_C_COMPILER
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    execute_process(
        COMMAND xcrun --find clang++
        OUTPUT_VARIABLE XGAME_XCODE_CXX_COMPILER
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(XGAME_XCODE_C_COMPILER)
        set(CMAKE_C_COMPILER "${XGAME_XCODE_C_COMPILER}" CACHE FILEPATH "C compiler for Xcode generator" FORCE)
    endif()

    if(XGAME_XCODE_CXX_COMPILER)
        set(CMAKE_CXX_COMPILER "${XGAME_XCODE_CXX_COMPILER}" CACHE FILEPATH "CXX compiler for Xcode generator" FORCE)
    endif()

    set(CMAKE_C_COMPILER_ID "AppleClang" CACHE INTERNAL "C compiler ID for Xcode generator")
    set(CMAKE_CXX_COMPILER_ID "AppleClang" CACHE INTERNAL "CXX compiler ID for Xcode generator")
    set(CMAKE_C_COMPILER_FORCED TRUE CACHE INTERNAL "Skip C compiler try-compile for Xcode generator")
    set(CMAKE_CXX_COMPILER_FORCED TRUE CACHE INTERNAL "Skip CXX compiler try-compile for Xcode generator")
endif()

project(XGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_GENERATOR STREQUAL "Xcode" AND APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++20")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS On)
option(USE_BUILD_TYPE_DIRS "是否使用构建类型区分输出目录" OFF)

function(set_server_output_dirs platform_name)
    set(OUTPUT_BIN_DIR "${PROJECT_SOURCE_DIR}/Bin/${platform_name}")
    set(OUTPUT_TEMP_DIR "${PROJECT_SOURCE_DIR}/Build/Temp/${platform_name}")

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}" PARENT_SCOPE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}" PARENT_SCOPE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_TEMP_DIR}" PARENT_SCOPE)

    if (USE_BUILD_TYPE_DIRS)
        set(DEBUG_OUTPUT_BIN_DIR "${OUTPUT_BIN_DIR}/Debug")
        set(RELEASE_OUTPUT_BIN_DIR "${OUTPUT_BIN_DIR}/Release")
        set(RELWITHDEBINFO_OUTPUT_BIN_DIR "${OUTPUT_BIN_DIR}/RelWithDebInfo")
        set(MINSIZEREL_OUTPUT_BIN_DIR "${OUTPUT_BIN_DIR}/MinSizeRel")

        set(DEBUG_OUTPUT_TEMP_DIR "${OUTPUT_TEMP_DIR}/Debug")
        set(RELEASE_OUTPUT_TEMP_DIR "${OUTPUT_TEMP_DIR}/Release")
        set(RELWITHDEBINFO_OUTPUT_TEMP_DIR "${OUTPUT_TEMP_DIR}/RelWithDebInfo")
        set(MINSIZEREL_OUTPUT_TEMP_DIR "${OUTPUT_TEMP_DIR}/MinSizeRel")
    else()
        set(DEBUG_OUTPUT_BIN_DIR "${OUTPUT_BIN_DIR}")
        set(RELEASE_OUTPUT_BIN_DIR "${OUTPUT_BIN_DIR}")
        set(RELWITHDEBINFO_OUTPUT_BIN_DIR "${OUTPUT_BIN_DIR}")
        set(MINSIZEREL_OUTPUT_BIN_DIR "${OUTPUT_BIN_DIR}")

        set(DEBUG_OUTPUT_TEMP_DIR "${OUTPUT_TEMP_DIR}")
        set(RELEASE_OUTPUT_TEMP_DIR "${OUTPUT_TEMP_DIR}")
        set(RELWITHDEBINFO_OUTPUT_TEMP_DIR "${OUTPUT_TEMP_DIR}")
        set(MINSIZEREL_OUTPUT_TEMP_DIR "${OUTPUT_TEMP_DIR}")
    endif()

    foreach(build_type DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${build_type} "${${build_type}_OUTPUT_BIN_DIR}" PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${build_type} "${${build_type}_OUTPUT_BIN_DIR}" PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${build_type} "${${build_type}_OUTPUT_TEMP_DIR}" PARENT_SCOPE)
    endforeach()

    set(EXECUTABLE_OUTPUT_PATH "${OUTPUT_BIN_DIR}/" PARENT_SCOPE)
    set(LIBRARY_OUTPUT_PATH "${OUTPUT_BIN_DIR}/" PARENT_SCOPE)
endfunction()

message(STATUS "OS : ${CMAKE_SYSTEM_NAME}")
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /GR")

    set_server_output_dirs(Windows)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    option(USE_32BITS "Compile for 32bits processors(linux only)" OFF)
    set(CMAKE_CXX_FLAGS "-g -fuse-ld=gold -Wall -lpthread -rdynamic -ldl -DLinux")
    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -fsanitize=leak")
    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-o2 -D_DEBUG -fsanitize=leak")
    set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG")
    set_server_output_dirs(Linux)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    add_compile_definitions(MacOS)
    set_server_output_dirs(MacOS)
endif()

option(BUILD_TOOLS "BUILD TOOLS" OFF)
option(BUILD_TOOLS "BUILD ROBOT" OFF)

add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)

SET(XENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../XEngine)

file(GLOB Api "${XENGINE_DIR}/Api/*.*")
file(GLOB_RECURSE PhysX "${XENGINE_DIR}/Api/PhysX/*.*")
file(GLOB Geometry "${XENGINE_DIR}/Utils/Geometry/*.*")
file(GLOB TinyXML "${XENGINE_DIR}/Utils/TinyXML/*.*")
file(GLOB JsonCpp "${XENGINE_DIR}/Utils/JsonCpp/*.*")
file(GLOB Excel "${XENGINE_DIR}/Utils/Excel/*.*")
file(GLOB Utils "${XENGINE_DIR}/Utils/*.*")

#Engine
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/XGame)

add_subdirectory(
    ${XENGINE_DIR}
    ${CMAKE_BINARY_DIR}/XEngine
)
