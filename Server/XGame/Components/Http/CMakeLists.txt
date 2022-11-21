#cmake file for project core
#author : max
#created : 2022-11-21 20:59:04

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Http "*.*")
source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${Http})

add_library(Http SHARED
    ${Api}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Http}
)

set_property(TARGET Http PROPERTY FOLDER "Components")
