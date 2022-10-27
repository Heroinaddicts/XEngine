#cmake file for project core
#author : max
#created : 2022-10-27 14:17:14

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(GameObjectManager
    "${CMAKE_CURRENT_SOURCE_DIR}/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Value.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Attribute.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Column.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Row.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Row.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Table.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Table.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GameObject.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/GameObject.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GameObjectManager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/GameObjectManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DllExport.cpp"
)

source_group(Api FILES ${Api})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(Interface FILES ${Interface})
source_group(\\ FILES ${GameObjectManager})

add_library(GameObjectManager SHARED
    ${Api}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${Interface}
    ${GameObjectManager}
)

set_property(TARGET GameObjectManager PROPERTY FOLDER "Components")
