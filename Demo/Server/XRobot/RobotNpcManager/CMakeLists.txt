#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB RobotNpcManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(XRobotInterface FILES ${XRobotInterface})
source_group(XGameInterface FILES ${XGameInterface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${RobotNpcManager})

add_library (RobotNpcManager SHARED
    ${Api}
    ${Common}
    ${Proto}
    ${XRobotInterface}
    ${XGameInterface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${RobotNpcManager}
)

set_property(TARGET RobotNpcManager PROPERTY FOLDER "GameRobot")

target_link_libraries(RobotNpcManager "-Xlinker \"-(\"")
target_link_libraries(RobotNpcManager ${LibProtobuf})
target_link_libraries(RobotNpcManager "-Xlinker \"-)\"")

