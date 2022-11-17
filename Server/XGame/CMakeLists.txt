include_directories(
    "${PROJECT_SOURCE_DIR}/XEngine/Api"
    "${PROJECT_SOURCE_DIR}/XEngine/Utils"
    "${PROJECT_SOURCE_DIR}/XEngine/Utils/TinyXML"
    "${PROJECT_SOURCE_DIR}/XEngine/Utils/Geometry"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/Common"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Proto"
)

#aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/Interface" Interface) 
#set(Interface
#    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iObjLoader.h"
#    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iGameObjectManager.h"
#)

file(GLOB Interface "Interface/*.*")
file(GLOB Common "Common/*.*")
file(GLOB Proto "Proto/*.*")

#Components
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tools)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Node)