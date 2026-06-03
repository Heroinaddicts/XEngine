include_directories(
    "${PROJECT_SOURCE_DIR}/XEngine/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Common"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto"
)


file(GLOB Common "${CMAKE_CURRENT_SOURCE_DIR}/Common/*.*")
file(GLOB XRobotInterface "${CMAKE_CURRENT_SOURCE_DIR}/Interface/*.*")
file(GLOB XGameInterface "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Interface/*.*")

file(GLOB LibProtobufLinux "${CMAKE_CURRENT_SOURCE_DIR}/Library/Protobuf/Linux/*.a")

file(GLOB Proto
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.cpp"
)

if(WIN32)
    set(LibProtobuf
        debug "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Library/Protobuf/Windows/Debug/*.lib"
        optimized "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Library/Protobuf/Windows/Release/*.lib"
    )
elseif(UNIX)
    set(LibProtobuf
        ${LibProtobufLinux}
    )
endif()

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/GameRobot)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotSceneManager)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotNpcManager)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotGameObjectSync)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotMovement)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotAi)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotSkill)