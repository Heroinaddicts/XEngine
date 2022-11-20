#cmake file for project core
#author : max
#created : 2022-11-19 17:50:21

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/HiRedis"
)

SET(HiRedis
    "${CMAKE_CURRENT_SOURCE_DIR}/hiredis/sslio.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hiredis/sockcompat.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hiredis/sds.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hiredis/read.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hiredis/net.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hiredis/hiredis.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hiredis/dict.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/hiredis/async.c"
)


file(GLOB Redis "*.*")
source_group(Api FILES ${Api})
source_group(Interface FILES ${Interface})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(HiRedis FILES ${HiRedis})
source_group(\\ FILES ${Redis})

add_library(Redis SHARED
    ${Api}
    ${Interface}
    ${TinyXML}
    ${Geometry}
    ${Utils}
    ${HiRedis}
    ${Redis}
)

set_property(TARGET Redis PROPERTY FOLDER "Components")
