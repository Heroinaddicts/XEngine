#cmake file for project core
#author : max
#created : 2022-11-19 13:11:33

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Gate "*.*")
source_group(Api FILES ${Api})
source_group(Common FILES ${Common})
source_group(General FILES ${General})
source_group(Proto FILES ${Proto})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${Gate})

add_library(Gate SHARED
    ${Api}
    ${Common}
    ${General}
    ${Proto}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Gate}
)

TARGET_LINK_LIBRARIES(Gate
    ${LibProtobuf}
)

set_property(TARGET Gate PROPERTY FOLDER "Node")

