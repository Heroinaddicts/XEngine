include_directories(
    "${XENGINE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Common"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto"
)


file(GLOB Common "${CMAKE_CURRENT_SOURCE_DIR}/Common/*.*")
file(GLOB XRobotInterface "${CMAKE_CURRENT_SOURCE_DIR}/Interface/*.*")
file(GLOB XGameInterface "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Interface/*.*")

file(GLOB Proto
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Proto/*.cpp"
)

if (WIN32)
    file(GLOB LibProtobuf_Debug "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Library/Protobuf/Windows/Debug/*.lib")
    file(GLOB LibProtobuf_Release "${CMAKE_CURRENT_SOURCE_DIR}/../XGame/Library/Protobuf/Windows/Release/*.lib")

    set(LibProtobuf
        $<$<CONFIG:Debug>:${LibProtobuf_Debug}>
        $<$<CONFIG:Release>:${LibProtobuf_Release}>
        $<$<CONFIG:RelWithDebInfo>:${LibProtobuf_Release}>
        $<$<CONFIG:MinSizeRel>:${LibProtobuf_Release}>
    )
elseif (UNIX)
    find_library(
        LibProtobuf
        NAMES protobuf
        PATHS /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib /opt/homebrew/lib
    )
    message(STATUS "Robot Protobuf Library: ${LibProtobuf}")
endif()

if (APPLE)
    set(XROBOT_LINK_GROUP_START "")
    set(XROBOT_LINK_GROUP_END "")
else()
    set(XROBOT_LINK_GROUP_START "-Xlinker \"-(\"")
    set(XROBOT_LINK_GROUP_END "-Xlinker \"-)\"")
endif()

set(XROBOT_PLATFORM_LIBS)

if(NOT WIN32)
    find_package(Threads REQUIRED)
    find_package(Iconv)

    list(APPEND XROBOT_PLATFORM_LIBS Threads::Threads)

    if(CMAKE_DL_LIBS)
        list(APPEND XROBOT_PLATFORM_LIBS ${CMAKE_DL_LIBS})
    endif()

    if(Iconv_FOUND)
        if(Iconv_LIBRARIES)
            list(APPEND XROBOT_PLATFORM_LIBS ${Iconv_LIBRARIES})
        elseif(Iconv_LIBRARY)
            list(APPEND XROBOT_PLATFORM_LIBS ${Iconv_LIBRARY})
        endif()
    endif()
endif()

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/GameRobot)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotSceneManager)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotNpcManager)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotGameObjectSync)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotMovement)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotAi)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/RobotSkill)
