#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB SceneManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${SceneManager})

add_library (SceneManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${SceneManager}
)

set_property(TARGET SceneManager PROPERTY FOLDER "Game")

target_link_libraries(SceneManager "-Xlinker \"-(\"")
target_link_libraries(SceneManager ${LibProtobuf})
target_link_libraries(SceneManager "-Xlinker \"-)\"")
