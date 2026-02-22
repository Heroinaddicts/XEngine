#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB NpcManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${NpcManager})

add_library (NpcManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${NpcManager}
)

set_property(TARGET NpcManager PROPERTY FOLDER "Game")

target_link_libraries(NpcManager "-Xlinker \"-(\"")
target_link_libraries(NpcManager ${LibProtobuf})
target_link_libraries(NpcManager "-Xlinker \"-)\"")
