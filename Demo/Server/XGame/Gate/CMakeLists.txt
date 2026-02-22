#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Gate "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Gate})

add_library (Gate SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Gate}
)

set_property(TARGET Gate PROPERTY FOLDER "Gate")

target_link_libraries(Gate "-Xlinker \"-(\"")
target_link_libraries(Gate ${LibProtobuf})
target_link_libraries(Gate "-Xlinker \"-)\"")
