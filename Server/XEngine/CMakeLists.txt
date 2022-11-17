#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:45

set_property(GLOBAL PROPERTY USE_FOLDERS On)

##message(STATUS "os : ${CMAKE_HOST_SYSTEM_NAME}")
##if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
##    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
##    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /GR")
##    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Windows/")
##    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Windows/")
##elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
##    option(USE_32BITS "Compile for 32bits processors(linux only)" OFF)
##    set(CMAKE_CXX_FLAGS "-g -fuse-ld=gold -std=c++11 -Wall -lpthread -rdynamic -ldl -DLinux")
##    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -fsanitize=leak")
##    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-o2 -D_DEBUG -fsanitize=leak")
##    set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG")
##    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Linux/")
##    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin/Linux/")
##endif()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}/Utils"
    "${CMAKE_CURRENT_SOURCE_DIR}/Utils/TinyXML"
    "${CMAKE_CURRENT_SOURCE_DIR}/Utils/Geometry"
)

file(GLOB Api "Api/*.*")
file(GLOB Geometry "Utils/Geometry/*.*")
file(GLOB TinyXML "Utils/TinyXML/*.*")
file(GLOB Utils "Utils/*.*")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine)
