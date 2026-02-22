#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB CharacterManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${CharacterManager})

add_library (CharacterManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${CharacterManager}
)

set_property(TARGET CharacterManager PROPERTY FOLDER "Game")

target_link_libraries(CharacterManager "-Xlinker \"-(\"")
target_link_libraries(CharacterManager ${LibProtobuf})
target_link_libraries(CharacterManager "-Xlinker \"-)\"")
