#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:45

cmake_minimum_required(VERSION 3.20)
project(XEngine)
set_property(GLOBAL PROPERTY USE_FOLDERS On)

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
    message(STATUS "os : windows")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/windows/")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/windows/")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "os : linux")
    option(USE_32BITS "Compile for 32bits processors(linux only)" OFF)
    set(CMAKE_CXX_FLAGS "-g -fuse-ld=gold -std=c++11 -Wall -lpthread -rdynamic -ldl -Dlinux")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -fuse-ld=gold -std=c++11 -lpthread -rdynamic -ldl -Dlinux")
    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-g -o2 -fuse-ld=gold -std=c++11 -lpthread -rdynamic -ldl -Dlinux")
    set(CMAKE_CXX_FLAGS_RELEASE "-g -o3 -fuse-ld=gold -std=c++11 -lpthread -rdynamic -ldl -Dlinux")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/linux/")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/linux/")

    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        add_definitions(
            -D_DEBUG
        )
    endif ()
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${PROJECT_SOURCE_DIR}/Utils"
)

set(Api
    "${PROJECT_SOURCE_DIR}/Api/iComponent.h"
    "${PROJECT_SOURCE_DIR}/Api/iEngine.h"
    "${PROJECT_SOURCE_DIR}/Api/MultiSys.h"
    "${PROJECT_SOURCE_DIR}/Api/MultiSys.cpp"
)

set(Utils
    "${PROJECT_SOURCE_DIR}/Utils/SafeString.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeString.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/SafeMemory.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeMemory.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/SafeFile.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeFile.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/SafeTime.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeTime.cpp"
)

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine)
add_subdirectory(${PROJECT_SOURCE_DIR}/Demo)