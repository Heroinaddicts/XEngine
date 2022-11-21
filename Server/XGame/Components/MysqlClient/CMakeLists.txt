#cmake file for project core
#author : max
#created : 2022-11-21 20:59:51

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB MysqlClient "*.*")
file(GLOB LibMysql "lib/*.lib")

source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${MysqlClient})

add_library(MysqlClient SHARED
    ${Api}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${MysqlClient}
)

TARGET_LINK_LIBRARIES(MysqlClient
    ${LibMysql}
)

set_property(TARGET MysqlClient PROPERTY FOLDER "Components")
