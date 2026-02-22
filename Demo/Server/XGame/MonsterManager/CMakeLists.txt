#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB MonsterManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${MonsterManager})

add_library (MonsterManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${MonsterManager}
)

set_property(TARGET MonsterManager PROPERTY FOLDER "Game")

target_link_libraries(MonsterManager "-Xlinker \"-(\"")
target_link_libraries(MonsterManager ${LibProtobuf})
target_link_libraries(MonsterManager "-Xlinker \"-)\"")
