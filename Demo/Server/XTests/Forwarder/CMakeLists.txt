#cmake file for project core

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Forwarder "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(\\ FILES ${Forwarder})

add_library (Forwarder SHARED
    ${Api}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Forwarder}
)

set_property(TARGET Forwarder PROPERTY FOLDER "PerfTest")
