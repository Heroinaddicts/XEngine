#cmake file for project core
#author : max
#created : 2022-11-14 18:16:00

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB TestNet "*.*")
source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${TestNet})

add_library(TestNet SHARED
    ${Api}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${TestNet}
)

set_property(TARGET TestNet PROPERTY FOLDER "Tests")
