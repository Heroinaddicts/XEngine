#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB MysqlTemplateGenerater "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(\\ FILES ${MysqlTemplateGenerater})

    SET(LibMariadb
        debug "${CMAKE_CURRENT_SOURCE_DIR}/Mysql/libmariadb.lib"
        optimized "${CMAKE_CURRENT_SOURCE_DIR}/Mysql/libmariadb.lib"
    )

add_executable(MysqlTemplateGenerater
    ${Api}
    ${Common}
    ${Interface}
    ${Geometry}
    ${Utils}
    ${MysqlTemplateGenerater}
)

target_link_libraries(MysqlTemplateGenerater "-Xlinker \"-(\"")
target_link_libraries(MysqlTemplateGenerater crypt32)
target_link_libraries(MysqlTemplateGenerater ${LibMariadb})
target_link_libraries(MysqlTemplateGenerater "-Xlinker \"-)\"")

set_property(TARGET MysqlTemplateGenerater PROPERTY FOLDER "Tools")
