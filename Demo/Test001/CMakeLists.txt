#cmake file for project core
#author : max
#created : 2021-04-27 10:40:33

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(Test001
    "${CMAKE_CURRENT_SOURCE_DIR}/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Test001.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Test001.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DllExport.cpp"
)

source_group(Api FILES ${Api})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${Test001})

add_library(Test001 SHARED
    ${Api}
    ${Geometry}
    ${Utils}
    ${Test001}
)

set_property(TARGET Test001 PROPERTY FOLDER "Test")
