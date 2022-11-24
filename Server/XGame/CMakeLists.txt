include_directories(
    "${PROJECT_SOURCE_DIR}/XEngine/Api"
    "${PROJECT_SOURCE_DIR}/XEngine/Utils"
    "${PROJECT_SOURCE_DIR}/XEngine/Utils/TinyXML"
    "${PROJECT_SOURCE_DIR}/XEngine/Utils/Geometry"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/Public"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/Arch/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Components/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Game/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tests/Interface"
)

#aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/Interface" Interface) 
#set(Interface
#    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iObjLoader.h"
#    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iGameObjectManager.h"
#)

file(GLOB Interface "Arch/Interface/*.*" "Components/Interface/*.*" "Game/Interface/*.*" "Tests/Interface/*.*")
file(GLOB Proto "Public/Proto/*.*")

if(CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
    SET(LibProtobuf
        debug "${CMAKE_CURRENT_SOURCE_DIR}/ThirdLibrary/Debug/libprotobufd.lib"
        optimized "${CMAKE_CURRENT_SOURCE_DIR}/ThirdLibrary/Release/libprotobuf.lib"
    )
endif()

if(CMAKE_HOST_SYSTEM_NAME MATCHES  "Linux")
    SET(LibProtobuf
    )
endif()

#Components
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Arch)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Components)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Game)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tests)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tools)
