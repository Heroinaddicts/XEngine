#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB Backpack "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${Backpack})

add_library (Backpack SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${Backpack}
)

set_property(TARGET Backpack PROPERTY FOLDER "Game")

target_link_libraries(Backpack "-Xlinker \"-(\"")
target_link_libraries(Backpack ${LibProtobuf})
target_link_libraries(Backpack "-Xlinker \"-)\"")
