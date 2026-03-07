cmake_minimum_required(VERSION 3.24)
project(XGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS On)
option(USE_BUILD_TYPE_DIRS "是否使用构建类型区分输出目录" OFF)

message(STATUS "OS : ${CMAKE_HOST_SYSTEM_NAME}")
if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /GR")

    # 分别设置 Debug 和 Release 构建类型下的静态库输出目录
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Bin/Windows)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Bin/Windows)
    set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Bin/Windows)

    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Windows/")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Windows/")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/Temp)
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    option(USE_32BITS "Compile for 32bits processors(linux only)" OFF)
    set(CMAKE_CXX_FLAGS "-g -fuse-ld=gold -Wall -lpthread -rdynamic -ldl -DLinux")
    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -fsanitize=leak")
    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-o2 -D_DEBUG -fsanitize=leak")
    set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Linux/")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Linux/")
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
    ${CMAKE_BINARY_DIR}/Build
)