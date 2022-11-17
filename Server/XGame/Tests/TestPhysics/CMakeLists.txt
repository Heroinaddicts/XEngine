#cmake file for project core
#author : max
#created : 2021-04-27 10:40:33

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB TestPhysics "*.*")

source_group(Api FILES ${Api})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${TestPhysics})

add_library(TestPhysics SHARED
    ${Api}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${TestPhysics}
)

set_property(TARGET TestPhysics PROPERTY FOLDER "Tests")
