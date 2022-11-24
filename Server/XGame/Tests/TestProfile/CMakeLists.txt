#cmake file for project core
#author : max
#created : 2022-11-24 12:43:38

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB TestProfile "*.*")
source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${TestProfile})

add_library(TestProfile SHARED
    ${Api}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${TestProfile}
)

set_property(TARGET TestProfile PROPERTY FOLDER "Tests")
