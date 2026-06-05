#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB RobotAi "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(XRobotInterface FILES ${XRobotInterface})
source_group(XGameInterface FILES ${XGameInterface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${RobotAi})

add_library (RobotAi SHARED
    ${Api}
    ${Common}
    ${Proto}
    ${XRobotInterface}
    ${XGameInterface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${RobotAi}
)

set_property(TARGET RobotAi PROPERTY FOLDER "GameRobot")

target_link_libraries(RobotAi ${XROBOT_LINK_GROUP_START})
target_link_libraries(RobotAi ${LibProtobuf})
target_link_libraries(RobotAi ${XROBOT_PLATFORM_LIBS})
target_link_libraries(RobotAi ${XROBOT_LINK_GROUP_END})

