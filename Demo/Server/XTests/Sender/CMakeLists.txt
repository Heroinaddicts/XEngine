#cmake file for project core

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Sender "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(\\ FILES ${Sender})

add_library (Sender SHARED
    ${Api}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Sender}
)

set_property(TARGET Sender PROPERTY FOLDER "PerfTest")
