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

if(WIN32)
    SET(LibMariadb
        debug "${CMAKE_CURRENT_SOURCE_DIR}/Mysql/libmariadb.lib"
        optimized "${CMAKE_CURRENT_SOURCE_DIR}/Mysql/libmariadb.lib"
    )
endif()

add_executable(MysqlTemplateGenerater
    ${Api}
    ${Common}
    ${Interface}
    ${Geometry}
    ${Utils}
    ${MysqlTemplateGenerater}
)

target_link_libraries(MysqlTemplateGenerater ${XTOOLS_PLATFORM_LIBS})

if(WIN32)
    target_link_libraries(MysqlTemplateGenerater crypt32)
    target_link_libraries(MysqlTemplateGenerater ${LibMariadb})
else()
    find_library(MARIADB_CLIENT_LIBRARY
        NAMES mariadb mysqlclient
        PATHS
            /usr/local/lib
            /opt/homebrew/lib
            /usr/lib
            /usr/lib64
    )

    if(NOT MARIADB_CLIENT_LIBRARY)
        message(FATAL_ERROR "MariaDB/MySQL client library not found. Install mariadb-connector-c or mysqlclient.")
    endif()

    target_link_libraries(MysqlTemplateGenerater ${MARIADB_CLIENT_LIBRARY})
endif()

set_property(TARGET MysqlTemplateGenerater PROPERTY FOLDER "Tools")
