#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:59

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Log"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/XZip"
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
endif()

set(iEngineComponent
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface/iEngineComponent.h"
)

file(GLOB Interface "Interface/*.*")
file(GLOB Logic "Component/Logic/*.*")
file(GLOB Log "Component/Log/*.*")

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
file(GLOB Net "Component/Net/Windows/*.*")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
file(GLOB Net "Component/Net/Linux/*.*")
endif()

file(GLOB Http "Component/Http/*.*")
file(GLOB CUrl "Component/Http/CUrl/*.*")
file(GLOB MiniZip "Component/XZip/MiniZip/*.*")
file(GLOB XZip "Component/XZip/*.*")
file(GLOB TimeWheel "Component/TimeWheel/*.*")
file(GLOB Engine "*.*")

source_group(Api FILES ${Api})
source_group(Interface FILES ${iEngineComponent})
source_group(Interface FILES ${Interface})
source_group(Utils\\Geometry FILES ${Geometry})
source_group(Utils\\TinyXML FILES ${TinyXML})
source_group(Utils\\Boost FILES ${Boost})
source_group(Utils FILES ${Utils})
source_group(Component\\Logic FILES ${Logic})
source_group(Component\\Log FILES ${Log})
source_group(Component\\Net FILES ${Net})
source_group(Component\\XZip FILES ${XZip})
source_group(Component\\XZip\\MiniZip FILES ${MiniZip})
source_group(Component\\Http FILES ${Http})
source_group(Component\\Http\\CUrl FILES ${CUrl})
source_group(Component\\TimeWheel FILES ${TimeWheel})
source_group(\\ FILES ${Engine})

if(CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
    set(LibCURL
        debug "${CMAKE_CURRENT_SOURCE_DIR}/Library/Debug/Windows/CUrl/libcurl-d_imp.lib"
        optimized "${CMAKE_CURRENT_SOURCE_DIR}/Library/Release/Windows/CUrl/libcurl_imp.lib"
    )
endif()

if(CMAKE_HOST_SYSTEM_NAME MATCHES  "Linux")
    set(LibCURL
        debug "${CMAKE_CURRENT_SOURCE_DIR}/Library/Debug/Windows/CUrl/libcurl-d_imp.lib"
        optimized "${CMAKE_CURRENT_SOURCE_DIR}/Library/Release/Windows/CUrl/libcurl_imp.lib"
    )
endif()

add_executable(XEngine
    ${Api}
    ${iEngineComponent}
    ${Interface}
    ${Logic}
    ${Log}
    ${Net}
    ${Boost}
    ${CUrl}
    ${Http}
    ${MiniZip}
    ${XZip}
    ${TimeWheel}
    ${Geometry}
    ${Utils}
    ${TinyXML}
    ${Engine}
)

target_link_libraries(XEngine "-Xlinker \"-(\"")
target_link_libraries(XEngine ${LibCURL})
target_link_libraries(XEngine "-Xlinker \"-)\"")
