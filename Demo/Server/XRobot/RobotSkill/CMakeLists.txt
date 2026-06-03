#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB RobotSkill "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(XRobotInterface FILES ${XRobotInterface})
source_group(XGameInterface FILES ${XGameInterface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${RobotSkill})

add_library (RobotSkill SHARED
    ${Api}
    ${Common}
    ${Proto}
    ${XRobotInterface}
    ${XGameInterface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${RobotSkill}
)

set_property(TARGET RobotSkill PROPERTY FOLDER "GameRobot")

target_link_libraries(RobotSkill "-Xlinker \"-(\"")
target_link_libraries(RobotSkill ${LibProtobuf})
target_link_libraries(RobotSkill "-Xlinker \"-)\"")

