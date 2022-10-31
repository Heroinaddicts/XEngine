#cmake file for project core
#author : max
#created : 2022-10-31 21:25:01

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB GameObjectTest "*.*")
source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${GameObjectTest})

add_library(GameObjectTest SHARED
    ${Api}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${GameObjectTest}
)

set_property(TARGET GameObjectTest PROPERTY FOLDER "Tests")
