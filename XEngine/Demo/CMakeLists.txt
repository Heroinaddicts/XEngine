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

#Tools
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tools/RemoteAgent)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tools/RemoteServer)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tools/ModuleCreator)

#Tests
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tests/Test001)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tests/TestPhysics)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tests/HelloWorld)

#Components
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Components/ObjLoader)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tests/TestNewCmakeLists)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Tests/GameObjectTest)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/ServerArchitecture/Master)