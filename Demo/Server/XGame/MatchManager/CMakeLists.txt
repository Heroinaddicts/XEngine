#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB MatchManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${MatchManager})

add_library (MatchManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${MatchManager}
)

set_property(TARGET MatchManager PROPERTY FOLDER "Game")

target_link_libraries(MatchManager "-Xlinker \"-(\"")
target_link_libraries(MatchManager ${LibProtobuf})
target_link_libraries(MatchManager "-Xlinker \"-)\"")
