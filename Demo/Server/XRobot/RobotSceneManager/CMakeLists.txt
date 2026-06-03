#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB RobotSceneManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(XRobotInterface FILES ${XRobotInterface})
source_group(XGameInterface FILES ${XGameInterface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${RobotSceneManager})

add_library (RobotSceneManager SHARED
    ${Api}
    ${Common}
    ${Proto}
    ${XRobotInterface}
    ${XGameInterface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${RobotSceneManager}
)

set_property(TARGET RobotSceneManager PROPERTY FOLDER "GameRobot")

target_link_libraries(RobotSceneManager "-Xlinker \"-(\"")
target_link_libraries(RobotSceneManager ${LibProtobuf})
target_link_libraries(RobotSceneManager "-Xlinker \"-)\"")
