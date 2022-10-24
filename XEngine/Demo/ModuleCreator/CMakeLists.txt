#cmake file for project core
#author : max
#created : 2021-04-27 10:40:33

include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(ModuleCreator
    "${CMAKE_CURRENT_SOURCE_DIR}/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Template.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

source_group(Api FILES ${Api})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(\\ FILES ${ModuleCreator})

add_executable(ModuleCreator
    ${Api}
    ${Utils}
    ${ModuleCreator}
)

set_property(TARGET ModuleCreator PROPERTY FOLDER "Tools")
