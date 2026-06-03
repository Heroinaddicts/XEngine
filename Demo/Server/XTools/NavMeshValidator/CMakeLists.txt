#cmake file for NavMeshValidator

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${XENGINE_DIR}/Engine/Interface"
    "${XENGINE_DIR}/Engine/Component"
    "${XENGINE_DIR}/Engine/Component/Navigation"
    "${XENGINE_DIR}/Utils/JsonCpp"
    "${PROJECT_SOURCE_DIR}/../recastnavigation/Detour/Include"
)

file(GLOB NavMeshValidator "*.*")
file(GLOB NavigationRuntime
    "${XENGINE_DIR}/Engine/Component/Navigation/Header.h"
    "${XENGINE_DIR}/Engine/Component/Navigation/NavMesh.h"
    "${XENGINE_DIR}/Engine/Component/Navigation/NavMesh.cpp"
)
file(GLOB DetourInclude "${PROJECT_SOURCE_DIR}/../recastnavigation/Detour/Include/*.*")
file(GLOB DetourSource "${PROJECT_SOURCE_DIR}/../recastnavigation/Detour/Source/*.*")

include("${XENGINE_DIR}/EngineSourceGroup.cmake")

source_group(Interface FILES ${Interface})
source_group(Common FILES ${Common})
source_group(NavigationRuntime FILES ${NavigationRuntime})
source_group(ThirdParty\\Detour\\Include FILES ${DetourInclude})
source_group(ThirdParty\\Detour\\Source FILES ${DetourSource})
source_group(\\ FILES ${NavMeshValidator})

add_executable(NavMeshValidator
    ${Api}
    ${Common}
    ${Interface}
    ${TinyXML}
    ${JsonCpp}
    ${Geometry}
    ${Utils}
    ${NavigationRuntime}
    ${DetourInclude}
    ${DetourSource}
    ${NavMeshValidator}
)

target_link_libraries(NavMeshValidator ${XTOOLS_PLATFORM_LIBS})

set_property(TARGET NavMeshValidator PROPERTY FOLDER "Tools")
