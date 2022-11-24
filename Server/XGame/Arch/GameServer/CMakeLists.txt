#cmake file for project core
#author : max
#created : 2022-11-19 13:48:27

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB GameServer "*.*")
source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${GameServer})

add_library(GameServer SHARED
    ${Api}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${GameServer}
)

TARGET_LINK_LIBRARIES(GameServer
    ${LibProtobuf}
)


set_property(TARGET GameServer PROPERTY FOLDER "Arch")
