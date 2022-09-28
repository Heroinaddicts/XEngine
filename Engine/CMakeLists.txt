#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:59

message(STATUS "current cmake path : ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component"
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
endif()

set(iEngineComponent
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iEngineComponent.h"
)

set(Interface
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iLogic.h"
)

set(Logic
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Logic.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Logic.cpp"
)

set(Engine
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp"
)

source_group(Api FILES ${Api})
source_group(Interface FILES ${iEngineComponent})
source_group(Interface FILES ${Interface})
source_group(\\Utils FILES ${Utils})
source_group(\\Component\\Logic FILES ${Logic})
source_group(\\ FILES ${Engine})

add_executable(XEngine
    ${Api}
    ${iEngineComponent}
    ${Interface}
    ${Logic}
    ${Utils}
    ${Engine}
)
