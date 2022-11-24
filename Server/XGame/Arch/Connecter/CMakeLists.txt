#cmake file for project core
#author : max
#created : 2022-11-17 12:30:58

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Connecter "*.*")
source_group(Api FILES ${Api})
source_group(Common FILES ${Common})
source_group(General FILES ${General})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${Connecter})

add_library(Connecter SHARED
    ${Api}
    ${Common}
    ${General}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Connecter}
)

TARGET_LINK_LIBRARIES(Connecter
    ${LibProtobuf}
)

set_property(TARGET Connecter PROPERTY FOLDER "Arch/Components")
