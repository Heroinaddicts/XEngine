#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB BuffManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${BuffManager})

add_library (BuffManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${BuffManager}
)

set_property(TARGET BuffManager PROPERTY FOLDER "Game")

target_link_libraries(BuffManager "-Xlinker \"-(\"")
target_link_libraries(BuffManager ${LibProtobuf})
target_link_libraries(BuffManager "-Xlinker \"-)\"")
