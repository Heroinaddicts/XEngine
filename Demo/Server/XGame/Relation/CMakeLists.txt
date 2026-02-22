#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Relation "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Relation})

add_library (Relation SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Relation}
)

set_property(TARGET Relation PROPERTY FOLDER "Game")

target_link_libraries(Relation "-Xlinker \"-(\"")
target_link_libraries(Relation ${LibProtobuf})
target_link_libraries(Relation "-Xlinker \"-)\"")
