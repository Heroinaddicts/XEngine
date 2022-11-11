#cmake file for project core
#author : max
#created : 2022-11-03 16:54:29

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Master "*.*")
source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Proto FILES ${Proto})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${Master})

add_library(Master SHARED
    ${Api}
    ${Interface}
    ${Proto}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Master}
)

set_property(TARGET Master PROPERTY FOLDER "ServerArchitecture")
