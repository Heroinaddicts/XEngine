#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB GameEntitySync "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${GameEntitySync})

add_library (GameEntitySync SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${GameEntitySync}
)

set_property(TARGET GameEntitySync PROPERTY FOLDER "Game")

target_link_libraries(GameEntitySync "-Xlinker \"-(\"")
target_link_libraries(GameEntitySync ${LibProtobuf})
target_link_libraries(GameEntitySync "-Xlinker \"-)\"")
