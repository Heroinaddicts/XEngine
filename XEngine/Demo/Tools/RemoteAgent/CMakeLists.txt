#cmake file for project core
#author : max
#created : 2021-04-27 10:40:33

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB RemoteAgent "*.*")

source_group(Api FILES ${Api})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${RemoteAgent})

add_library(RemoteAgent SHARED
    ${Api}
    ${Geometry}
    ${Utils}
    ${RemoteAgent}
)

set_property(TARGET RemoteAgent PROPERTY FOLDER "Tools")
