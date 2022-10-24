#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:45

cmake_minimum_required(VERSION 3.2)
project(XEngine)
set_property(GLOBAL PROPERTY USE_FOLDERS On)

message(STATUS "os : ${CMAKE_HOST_SYSTEM_NAME}")
if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Windows/")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Windows/")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    option(USE_32BITS "Compile for 32bits processors(linux only)" OFF)
    set(CMAKE_CXX_FLAGS "-g -fuse-ld=gold -std=c++11 -Wall -lpthread -rdynamic -ldl -DLinux")
    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-o2 -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Linux/")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Linux/")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${PROJECT_SOURCE_DIR}/Utils"
    "${PROJECT_SOURCE_DIR}/Utils/TinyXML"
    "${PROJECT_SOURCE_DIR}/Utils/Geometry"
)

set(Api
    "${PROJECT_SOURCE_DIR}/Api/iGameObject.h"
    "${PROJECT_SOURCE_DIR}/Api/iComponent.h"
    "${PROJECT_SOURCE_DIR}/Api/iNetApi.h"
    "${PROJECT_SOURCE_DIR}/Api/iTimerApi.h"
    "${PROJECT_SOURCE_DIR}/Api/iNavigationApi.h"
    "${PROJECT_SOURCE_DIR}/Api/iPhysicsApi.h"
    "${PROJECT_SOURCE_DIR}/Api/iEngine.h"
    "${PROJECT_SOURCE_DIR}/Api/MultiSys.h"
    "${PROJECT_SOURCE_DIR}/Api/MultiSys.cpp"
)

set(Geometry
    "${PROJECT_SOURCE_DIR}/Utils/Geometry/Geometry.h"
    "${PROJECT_SOURCE_DIR}/Utils/Geometry/Vector.h"
    "${PROJECT_SOURCE_DIR}/Utils/Geometry/Vector.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/Geometry/Quaternion.h"
    "${PROJECT_SOURCE_DIR}/Utils/Geometry/Quaternion.cpp"
)

set(TinyXML
    "${PROJECT_SOURCE_DIR}/Utils/TinyXML/tinyxml2.h"
    "${PROJECT_SOURCE_DIR}/Utils/TinyXML/tinyxml2.cpp"
)

set(Utils
    "${PROJECT_SOURCE_DIR}/Utils/SafeString.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeString.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/SafeMemory.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeMemory.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/SafeThread.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeThread.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/SafeSystem.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeSystem.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/SafeTools.h"
    "${PROJECT_SOURCE_DIR}/Utils/SafeTools.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/SafeQueue.h"
    "${PROJECT_SOURCE_DIR}/Utils/XBuffer.h"
    "${PROJECT_SOURCE_DIR}/Utils/XPool.h"
    "${PROJECT_SOURCE_DIR}/Utils/XLock.h"
    "${PROJECT_SOURCE_DIR}/Utils/X3DObj.h"
    "${PROJECT_SOURCE_DIR}/Utils/X3DObj.cpp"
    "${PROJECT_SOURCE_DIR}/Utils/XFile.h"
    "${PROJECT_SOURCE_DIR}/Utils/XFile.cpp"
)

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine)
add_subdirectory(${PROJECT_SOURCE_DIR}/Demo)
