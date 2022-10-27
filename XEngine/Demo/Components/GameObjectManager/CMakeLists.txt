#cmake file for project core
#author : max
#created : 2022-10-27 14:17:14

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB GameObjectManager "*.*")

source_group(Api FILES ${Api})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(Common FILES ${Common})
source_group(Interface FILES ${Interface})
source_group(\\ FILES ${GameObjectManager})

add_library(GameObjectManager SHARED
    ${Api}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Common}
    ${Interface}
    ${GameObjectManager}
)

set_property(TARGET GameObjectManager PROPERTY FOLDER "Components")
