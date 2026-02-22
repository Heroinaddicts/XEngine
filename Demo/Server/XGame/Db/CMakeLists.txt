#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Db "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Db})

add_library (Db SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Db}
)

set_property(TARGET Db PROPERTY FOLDER "Components")

target_link_libraries(Db "-Xlinker \"-(\"")
target_link_libraries(Db ${LibProtobuf})
target_link_libraries(Db "-Xlinker \"-)\"")
