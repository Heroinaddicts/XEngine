#cmake file for project core
#author : max
#created : 2022-10-24 21:36:41

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(HelloWorld
    "${CMAKE_CURRENT_SOURCE_DIR}/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/HelloWorld.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/HelloWorld.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DllExport.cpp"
)

source_group(Api FILES ${Api})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${HelloWorld})

add_library(HelloWorld SHARED
    ${Api}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${HelloWorld}
)

set_property(TARGET HelloWorld PROPERTY FOLDER "Tests")
