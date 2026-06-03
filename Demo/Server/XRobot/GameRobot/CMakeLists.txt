#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB GameRobot "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(XRobotInterface FILES ${XRobotInterface})
source_group(XGameInterface FILES ${XGameInterface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${GameRobot})

add_library (GameRobot SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${XRobotInterface}
    ${XGameInterface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${GameRobot}
)

set_property(TARGET GameRobot PROPERTY FOLDER "GameRobot")

target_link_libraries(GameRobot "-Xlinker \"-(\"")
target_link_libraries(GameRobot ${LibProtobuf})
target_link_libraries(GameRobot "-Xlinker \"-)\"")
