#cmake file for project core

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Reciver "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(\\ FILES ${Reciver})

add_library (Reciver SHARED
    ${Api}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Reciver}
)

set_property(TARGET Reciver PROPERTY FOLDER "PerfTest")
