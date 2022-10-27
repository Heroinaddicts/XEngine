#cmake file for project core
#author : max
#created : 2021-04-27 10:40:33

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(RemoteServer
    "${CMAKE_CURRENT_SOURCE_DIR}/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/AgentConnection.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/AgentConnection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UserConnection.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/UserConnection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RemoteServer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/RemoteServer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DllExport.cpp"
)

source_group(Api FILES ${Api})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${RemoteServer})

add_library(RemoteServer SHARED
    ${Api}
    ${Geometry}
    ${Utils}
    ${RemoteServer}
)

set_property(TARGET RemoteServer PROPERTY FOLDER "Tools")
