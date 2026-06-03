#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Libxl "${CMAKE_CURRENT_SOURCE_DIR}/Libxl/*.h")
file(GLOB UnitGenerater "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(\\ FILES ${UnitGenerater})

add_executable(UnitGenerater
    ${Api}
    ${Common}
    ${Interface}
    ${Excel}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${UnitGenerater}
)

set_property(TARGET UnitGenerater PROPERTY FOLDER "Tools")
