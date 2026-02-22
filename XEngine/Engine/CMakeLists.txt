#cmake file for XEngine 
#author : max
#created : 2022-09-28 12:59

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include("${CMAKE_CURRENT_SOURCE_DIR}/../EngineSourceGroup.cmake")

find_package(OpenSSL REQUIRED)
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Physics"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Log"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Logic"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Net"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Task"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/TimeWheel"
#    "${CMAKE_CURRENT_SOURCE_DIR}/Component/WebSockets"
#    "${CMAKE_CURRENT_SOURCE_DIR}/Component/WebSockets/libwebsockets"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Mysql"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Redis"
    "${CMAKE_CURRENT_SOURCE_DIR}/Component/Redis/hiredis"
    "${OPENSSL_INCLUDE_DIR}"
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
file(GLOB Redis "Component/Redis/*.*")
file(GLOB HiRedis "Component/Redis/hiredis/*.*")

if (CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
file(GLOB Net "Component/Net/Windows/*.*")
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
file(GLOB Net "Component/Net/Linux/*.*")
endif()

file(GLOB Http "Component/Http/*.*")
file(GLOB CUrl "Component/Http/CUrl/*.*")
file(GLOB TimeWheel "Component/TimeWheel/*.*")
file(GLOB Task "Component/Task/*.*")
file(GLOB Mysql "Component/Mysql/*.*")
file(GLOB Physics "Component/Physics/*.*")
#file(GLOB WebSockets "Component/WebSockets/*.*")
file(GLOB Engine "*.*")

source_group(Interface FILES ${iEngineComponent})
source_group(Interface FILES ${Interface})
source_group(Component\\Logic FILES ${Logic})
source_group(Component\\Log FILES ${Log})
source_group(Component\\Net FILES ${Net})
source_group(Component\\Task FILES ${Task})
#source_group(Component\\WebSockets FILES ${WebSockets})
source_group(Component\\Http FILES ${Http})
source_group(Component\\Http\\CUrl FILES ${CUrl})
source_group(Component\\TimeWheel FILES ${TimeWheel})
source_group(Component\\Mysql FILES ${Mysql})
#source_group(Component\\Physics FILES ${Physics})
source_group(Component\\Redis FILES ${Redis})
source_group(Component\\Redis\\HiRedis FILES ${HiRedis})
source_group(\\ FILES ${Engine})

if(CMAKE_HOST_SYSTEM_NAME MATCHES  "Windows")
    set(LibCURL
        debug "${CMAKE_CURRENT_SOURCE_DIR}/Library/Windows/Debug/CUrl/libcurl-d_imp.lib"
        optimized "${CMAKE_CURRENT_SOURCE_DIR}/Library/Windows/Release/CUrl/libcurl_imp.lib"
    )

    SET(LibMariadb
        debug "${CMAKE_CURRENT_SOURCE_DIR}/Library/Windows/Debug/Mysql/libmariadb.lib"
        optimized "${CMAKE_CURRENT_SOURCE_DIR}/Library/Windows/Release/Mysql/libmariadb.lib"
    )

#    SET(LibWebsockets
#        debug "${CMAKE_CURRENT_SOURCE_DIR}/Library/Windows/Debug/LibWebSockets/websockets.lib"
#        optimized "${CMAKE_CURRENT_SOURCE_DIR}/Library/Windows/Release/LibWebSockets/websockets.lib"
#    )
endif()

if(CMAKE_HOST_SYSTEM_NAME MATCHES  "Linux")
# 先尝试官方/打包器提供的 CMake 包
    find_package(CURL CONFIG QUIET)
    if(CURL_FOUND)
        # 大多数发行版/包管理器会提供 CURL::libcurl
        set(LibCURL CURL::libcurl)
    else()
        # 回退到 pkg-config
        find_package(PkgConfig REQUIRED)

        # 如需静态链接，请取消下一行注释（会把依赖链都带上）
        # set(PKG_CONFIG_USE_STATIC_LIBS ON)

        pkg_check_modules(PC_CURL REQUIRED libcurl)

        # 用一个 INTERFACE 目标把 pkg-config 的结果封装起来
        add_library(CURL_IMPORTED INTERFACE)
        target_include_directories(CURL_IMPORTED INTERFACE ${PC_CURL_INCLUDE_DIRS})
        # CMake 3.13+ 可用 target_link_directories；旧版本可用 link_directories 全局设置
        if(COMMAND target_link_directories)
            target_link_directories(CURL_IMPORTED INTERFACE ${PC_CURL_LIBRARY_DIRS})
        endif()
        target_link_libraries(CURL_IMPORTED INTERFACE ${PC_CURL_LIBRARIES})
        # 起个与 find_package 一致的别名，后续统一写法
        add_library(CURL::libcurl ALIAS CURL_IMPORTED)

        set(LibCURL CURL::libcurl)
    endif()

    

    SET(LibMariadb
        "${CMAKE_CURRENT_SOURCE_DIR}/Library/Linux/libmariadb.a"
    )
endif()

add_executable(XEngine
    ${Api}
    ${PhysX}
    ${iEngineComponent}
    ${Interface}
    ${Logic}
    ${HiRedis}
    ${Redis}
    ${Log}
    ${Net}
    ${Boost}
    ${CUrl}
    ${Http}
#    ${WebSockets}
    ${Task}
    ${TimeWheel}
    ${Geometry}
    ${Utils}
    ${TinyXML}
    ${Mysql}
    #${Physics}
    #${PhysX}
    ${Engine}
)

target_link_libraries(XEngine "-Xlinker \"-(\"")
if (WIN32)
    target_link_libraries(XEngine crypt32)
endif()
target_link_libraries(XEngine ${LibCURL})
#target_link_libraries(XEngine ${LibPhysX})
target_link_libraries(XEngine ${LibMariadb})
#target_link_libraries(XEngine ${LibWebsockets})
target_link_libraries(XEngine ${OPENSSL_LIBRARIES})
target_link_libraries(XEngine "-Xlinker \"-)\"")
