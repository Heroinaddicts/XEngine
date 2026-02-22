#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB BossManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${BossManager})

add_library (BossManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${BossManager}
)

set_property(TARGET BossManager PROPERTY FOLDER "Game")

target_link_libraries(BossManager "-Xlinker \"-(\"")
target_link_libraries(BossManager ${LibProtobuf})
target_link_libraries(BossManager "-Xlinker \"-)\"")
