#cmake file for project core
#author : max
#created : 2022-10-28 01:16:58

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB TestNewCmakeLists "*.*")
source_group(Api FILES ${Api})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${TestNewCmakeLists})

add_library(TestNewCmakeLists SHARED
    ${Api}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${TestNewCmakeLists}
)

set_property(TARGET TestNewCmakeLists PROPERTY FOLDER "Tests")
