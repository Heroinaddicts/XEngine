#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB ServerManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(\\ FILES ${ServerManager})

add_library (ServerManager SHARED
    ${Api}
    ${Common}
    ${Interface}
    ${Utils}
    ${ServerManager}
)

set_property(TARGET ServerManager PROPERTY FOLDER "Components")
