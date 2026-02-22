#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Combat "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Combat})

add_library (Combat SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Combat}
)

set_property(TARGET Combat PROPERTY FOLDER "Game")

target_link_libraries(Combat "-Xlinker \"-(\"")
target_link_libraries(Combat ${LibProtobuf})
target_link_libraries(Combat "-Xlinker \"-)\"")
