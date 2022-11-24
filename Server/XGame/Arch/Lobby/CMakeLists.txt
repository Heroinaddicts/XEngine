#cmake file for project core
#author : max
#created : 2022-11-19 13:47:38

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Lobby "*.*")
source_group(Api FILES ${Api})
source_group(General FILES ${General})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${Lobby})

add_library(Lobby SHARED
    ${Api}
    ${Interface}
    ${General}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Lobby}
)

TARGET_LINK_LIBRARIES(Lobby
    ${LibProtobuf}
)


set_property(TARGET Lobby PROPERTY FOLDER "Arch")
