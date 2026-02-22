#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Game "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Game})

add_library (Game SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Game}
)

set_property(TARGET Game PROPERTY FOLDER "Game")

target_link_libraries(Game "-Xlinker \"-(\"")
target_link_libraries(Game ${LibProtobuf})
target_link_libraries(Game "-Xlinker \"-)\"")
