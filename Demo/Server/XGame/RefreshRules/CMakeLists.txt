#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB RefreshRules "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${RefreshRules})

add_library (RefreshRules SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${RefreshRules}
)

set_property(TARGET RefreshRules PROPERTY FOLDER "Game")

target_link_libraries(RefreshRules "-Xlinker \"-(\"")
target_link_libraries(RefreshRules ${LibProtobuf})
target_link_libraries(RefreshRules "-Xlinker \"-)\"")
