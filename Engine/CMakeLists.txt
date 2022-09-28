#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:59

message(STATUS "current cmake path : ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net"
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
endif()

set(iEngineComponent
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iEngineComponent.h"
)

set(Interface
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iLogic.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iNet.h"
)

set(Logic
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Logic.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Logic.cpp"
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
set(Net
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Net.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Net.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Tcper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Tcper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Accepter.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Accepter.cpp"
)
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")

endif()


set(Engine
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp"
)

source_group(Api FILES ${Api})
source_group(Interface FILES ${iEngineComponent})
source_group(Interface FILES ${Interface})
source_group(\\Utils FILES ${Utils})
source_group(\\Component\\Logic FILES ${Logic})
source_group(\\Component\\Net FILES ${Net})
source_group(\\ FILES ${Engine})

add_executable(XEngine
    ${Api}
    ${iEngineComponent}
    ${Interface}
    ${Logic}
    ${Net}
    ${Utils}
    ${Engine}
)
