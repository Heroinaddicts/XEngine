#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:59

message(STATUS "current cmake path : ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Racast/Contrib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Racast/Detour"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Racast/DetourCrowd"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Racast/DetourTileCache"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Racast/Recast"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/Physx4"
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
endif()

set(iEngineComponent
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iEngineComponent.h"
)

file(GLOB Interface "Interface/*.*")
file(GLOB Logic "Component/Logic/*.*")

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
file(GLOB Net "Component/Net/Windows/*.*")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
file(GLOB Net "Component/Net/Linux/*.*")
endif()

file(GLOB TimeWheel "Component/TimeWheel/*.*")
file(GLOB_RECURSE Recast "Component/Navigation/Racast/*.*")
file(GLOB Navigation "Component/Navigation/*.*")
file(GLOB Physics "Component/Physics/*.*")
file(GLOB Engine "*.*")

source_group(Api FILES ${Api})
source_group(Interface FILES ${iEngineComponent})
source_group(Interface FILES ${Interface})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils FILES ${Utils})
source_group(Component\\Logic FILES ${Logic})
source_group(Component\\Net FILES ${Net})
source_group(Component\\TimeWheel FILES ${TimeWheel})
source_group(Component\\Navigation\\Recast FILES ${Recast})
source_group(Component\\Navigation FILES ${Navigation})
source_group(Component\\Physics FILES ${Physics})
source_group(\\ FILES ${Engine})

if(CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
set(LibFbxSDK

)

set(LibPhysx4
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysXCooking_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysXExtensions_static_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysXFoundation_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysXPvdSDK_static_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysXTask_static_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysXVehicle_static_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/LowLevel_static_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/LowLevelAABB_static_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/LowLevelDynamics_static_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysX_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysXCharacterKinematic_static_64.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/Physx4/PhysXCommon_64.lib"
    
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysXCooking_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysXExtensions_static_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysXFoundation_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysXPvdSDK_static_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysXTask_static_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysXVehicle_static_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/LowLevel_static_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/LowLevelAABB_static_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/LowLevelDynamics_static_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysX_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysXCharacterKinematic_static_64.lib"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Release/Windows/Physx4/PhysXCommon_64.lib"
)
endif()

if(CMAKE_HOST_SYSTEM_NAME MATCHES  "Linux")
set(LibFbxSDK

)

set(LibPhysx4
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXFoundation_static_64.a"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCharacterKinematic_static_64.a"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCommon_static_64.a"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCooking_static_64.a"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXExtensions_static_64.a"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXVehicle_static_64.a"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXPvdSDK_static_64.a"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysX_static_64.a"
    
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXFoundation_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCharacterKinematic_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCommon_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCooking_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXExtensions_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXVehicle_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXPvdSDK_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysX_static_64.a"
)
endif()

add_executable(XEngine
    ${Api}
    ${iEngineComponent}
    ${Interface}
    ${Logic}
    ${Net}
    ${TimeWheel}
    ${Recast}
    ${Navigation}
    ${Physics}
    ${Geometry}
    ${Utils}
    ${Engine}
)

target_link_libraries(XEngine "-Xlinker \"-(\"")
target_link_libraries(XEngine ${LibPhysx4})
target_link_libraries(XEngine "-Xlinker \"-)\"")
