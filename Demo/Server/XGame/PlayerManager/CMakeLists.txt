#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB PlayerManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${PlayerManager})

add_library (PlayerManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${PlayerManager}
)

set_property(TARGET PlayerManager PROPERTY FOLDER "Game")

target_link_libraries(PlayerManager "-Xlinker \"-(\"")
target_link_libraries(PlayerManager ${LibProtobuf})
target_link_libraries(PlayerManager "-Xlinker \"-)\"")
