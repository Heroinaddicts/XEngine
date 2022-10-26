#cmake file for project core
#author : max
#created : 2022-10-24 21:17:32

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(Hello
    "${CMAKE_CURRENT_SOURCE_DIR}/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Hello.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Hello.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DllExport.cpp"
)

source_group(Api FILES ${Api})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${Hello})

add_library(Hello SHARED
    ${Api}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Hello}
)

set_property(TARGET Hello PROPERTY FOLDER "Test")
