#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Movement "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Movement})

add_library (Movement SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Movement}
)

set_property(TARGET Movement PROPERTY FOLDER "Game")

target_link_libraries(Movement "-Xlinker \"-(\"")
target_link_libraries(Movement ${LibProtobuf})
target_link_libraries(Movement "-Xlinker \"-)\"")
