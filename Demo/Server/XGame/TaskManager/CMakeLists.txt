#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB TaskManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${TaskManager})

add_library (TaskManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${TaskManager}
)

set_property(TARGET TaskManager PROPERTY FOLDER "Game")

target_link_libraries(TaskManager "-Xlinker \"-(\"")
target_link_libraries(TaskManager ${LibProtobuf})
target_link_libraries(TaskManager "-Xlinker \"-)\"")
