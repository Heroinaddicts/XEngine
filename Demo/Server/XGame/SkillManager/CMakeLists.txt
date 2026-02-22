#cmake file for project core
#author : {auther}
#created : {date}

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

file(GLOB SkillManager "*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(Proto FILES ${Proto})
source_group(\\ FILES ${SkillManager})

add_library (SkillManager SHARED
    ${Api}
    ${Proto}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${SkillManager}
)

set_property(TARGET SkillManager PROPERTY FOLDER "Game")

target_link_libraries(SkillManager "-Xlinker \"-(\"")
target_link_libraries(SkillManager ${LibProtobuf})
target_link_libraries(SkillManager "-Xlinker \"-)\"")
