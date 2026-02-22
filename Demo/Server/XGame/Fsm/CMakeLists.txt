#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Fsm "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Fsm})

add_library (Fsm SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Fsm}
)

set_property(TARGET Fsm PROPERTY FOLDER "Components")

target_link_libraries(Fsm "-Xlinker \"-(\"")
target_link_libraries(Fsm ${LibProtobuf})
target_link_libraries(Fsm "-Xlinker \"-)\"")
