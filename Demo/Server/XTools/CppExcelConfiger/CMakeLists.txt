#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Libxl "${CMAKE_CURRENT_SOURCE_DIR}/Libxl/*.h")
file(GLOB CppExcelConfiger "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(\\ FILES ${CppExcelConfiger})

add_executable(CppExcelConfiger
    ${Api}
    ${Common}
    ${Interface}
    ${Excel}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${CppExcelConfiger}
)

set_property(TARGET CppExcelConfiger PROPERTY FOLDER "Tools")