#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB TeamManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${TeamManager})

add_library (TeamManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${TeamManager}
)

set_property(TARGET TeamManager PROPERTY FOLDER "Game")

target_link_libraries(TeamManager "-Xlinker \"-(\"")
target_link_libraries(TeamManager ${LibProtobuf})
target_link_libraries(TeamManager "-Xlinker \"-)\"")
