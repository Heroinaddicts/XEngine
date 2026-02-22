#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB CacheClient "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${CacheClient})

add_library (CacheClient SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${CacheClient}
)

set_property(TARGET CacheClient PROPERTY FOLDER "Components")

target_link_libraries(CacheClient "-Xlinker \"-(\"")
target_link_libraries(CacheClient ${LibProtobuf})
target_link_libraries(CacheClient "-Xlinker \"-)\"")
