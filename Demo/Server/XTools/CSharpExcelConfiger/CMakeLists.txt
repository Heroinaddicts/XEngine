#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Libxl "${CMAKE_CURRENT_SOURCE_DIR}/Libxl/*.h")
file(GLOB CSharpExcelConfiger "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(\\ FILES ${CSharpExcelConfiger})

add_executable(CSharpExcelConfiger
    ${Api}
    ${Common}
    ${Interface}
    ${Excel}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${CSharpExcelConfiger}
)

target_link_libraries(CSharpExcelConfiger ${XTOOLS_PLATFORM_LIBS})

set_property(TARGET CSharpExcelConfiger PROPERTY FOLDER "Tools")
