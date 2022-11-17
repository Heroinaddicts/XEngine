project(XGame)
cmake_minimum_required(VERSION 3.24)

#aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/Interface" Interface) 
#set(Interface
#    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iObjLoader.h"
#    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iGameObjectManager.h"
#)

file(GLOB Api "XEngine/Api/*.*")
file(GLOB Geometry "XEngine/Utils/Geometry/*.*")
file(GLOB TinyXML "XEngine/Utils/TinyXML/*.*")
file(GLOB Utils "XEngine/Utils/*.*")


#Engine
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/XGame)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/XEngine)
