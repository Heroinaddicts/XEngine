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
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Contrib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourTileCache"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/Physx4"
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
endif()

set(iEngineComponent
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iEngineComponent.h"
)

set(Interface
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iLogic.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iNet.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iTimeWheel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iNavigation.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iPhysics.h"
)

set(Logic
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Logic.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic/Logic.cpp"
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
set(Net
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Net.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Net.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Tcper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Tcper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Accepter.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Windows/Accepter.cpp"
)
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
set(Net
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Linux/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Linux/Net.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Linux/Net.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Linux/Tcper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Linux/Tcper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Linux/Accepter.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net/Linux/Accepter.cpp"
)
endif()

set(TimeWheel
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/TimeBase.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/TimeBase.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/TimeBaseList.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/TimeBaseList.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/TimeGear.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/TimeGear.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/TimeWheel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel/TimeWheel.cpp"
)

set(Recast
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Contrib/stb_truetype.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Contrib/fastlz/fastlz.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Contrib/fastlz/fastlz.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourAlloc.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourAlloc.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourAssert.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourAssert.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourCommon.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourCommon.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourMath.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourNavMesh.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourNavMesh.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourNavMeshBuilder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourNavMeshBuilder.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourNavMeshQuery.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourNavMeshQuery.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourNode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourNode.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Detour/DetourStatus.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourCrowd.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourCrowd.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourLocalBoundary.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourLocalBoundary.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourObstacleAvoidance.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourObstacleAvoidance.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourPathCorridor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourPathCorridor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourPathQueue.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourPathQueue.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourProximityGrid.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourCrowd/DetourProximityGrid.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourTileCache/DetourTileCache.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourTileCache/DetourTileCache.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourTileCache/DetourTileCacheBuilder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/DetourTileCache/DetourTileCacheBuilder.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/Recast.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/Recast.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastAlloc.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastAlloc.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastArea.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastAssert.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastAssert.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastContour.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastFilter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastLayers.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastMesh.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastMeshDetail.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastRasterization.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Recast/RecastRegion.cpp"
)

set(Navigation
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Navmesh.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Navmesh.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Navigation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Navigation/Navigation.h"
)

set(Physics
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/Header.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/PhysxBase.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/PhysxBase.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/PhysxScene.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/PhysxScene.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/Physics.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics/Physics.cpp"
)

set(Engine
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp"
)

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
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/FbxSDK/libfbxsdk.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/FbxSDK/libfbxsdk-md.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/FbxSDK/libxml2-md.lib"
    debug "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Windows/FbxSDK/zlib-md.lib"
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
    #debug "${PROJECT_SOURCE_DIR}/Bin/Linux/libPhysXGpu_64.so"
    
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXFoundation_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCharacterKinematic_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCommon_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXCooking_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXExtensions_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXVehicle_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysXPvdSDK_static_64.a"
    optimized "${PROJECT_SOURCE_DIR}/Engine/Library/Debug/Linux/Physx4/libPhysX_static_64.a"
    #optimized "${PROJECT_SOURCE_DIR}/Bin/Linux/libPhysXGpu_64.so"
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
