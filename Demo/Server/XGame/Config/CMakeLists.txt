#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Config "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Config})

add_library (Config SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Config}
)

set_property(TARGET Config PROPERTY FOLDER "Components")

if (MSVC)
    target_compile_options(Config PRIVATE /utf-8)
endif()
target_link_libraries(Config "-Xlinker \"-(\"")
target_link_libraries(Config ${LibProtobuf})
target_link_libraries(Config "-Xlinker \"-)\"")
