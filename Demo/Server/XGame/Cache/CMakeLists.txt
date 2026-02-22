#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Cache "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Cache})

add_library (Cache SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Cache}
)

set_property(TARGET Cache PROPERTY FOLDER "Cache")

target_link_libraries(Cache "-Xlinker \"-(\"")
target_link_libraries(Cache ${LibProtobuf})
target_link_libraries(Cache "-Xlinker \"-)\"")
