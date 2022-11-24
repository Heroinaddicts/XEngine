#cmake file for project core
#author : max
#created : 2022-11-24 12:58:36

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Location "*.*")
source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${Location})

add_library(Location SHARED
    ${Api}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Location}
)

TARGET_LINK_LIBRARIES(Location
    ${LibProtobuf}
)


set_property(TARGET Location PROPERTY FOLDER "Arch")
