#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB GameWorld "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${GameWorld})

add_library (GameWorld SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${GameWorld}
)

set_property(TARGET GameWorld PROPERTY FOLDER "Game")

target_link_libraries(GameWorld "-Xlinker \"-(\"")
target_link_libraries(GameWorld ${LibProtobuf})
target_link_libraries(GameWorld "-Xlinker \"-)\"")
