
include_directories(
    "${PROJECT_SOURCE_DIR}/Api"
    "${CMAKE_CURRENT_SOURCE_DIR}/Common"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
)

set(XTOOLS_PLATFORM_LIBS)

if(NOT WIN32)
    find_package(Threads REQUIRED)
    find_package(Iconv)

    list(APPEND XTOOLS_PLATFORM_LIBS Threads::Threads)

    if(CMAKE_DL_LIBS)
        list(APPEND XTOOLS_PLATFORM_LIBS ${CMAKE_DL_LIBS})
    endif()

    if(Iconv_FOUND)
        if(Iconv_LIBRARIES)
            list(APPEND XTOOLS_PLATFORM_LIBS ${Iconv_LIBRARIES})
        elseif(Iconv_LIBRARY)
            list(APPEND XTOOLS_PLATFORM_LIBS ${Iconv_LIBRARY})
        endif()
    endif()
endif()

file(GLOB Interface "${CMAKE_CURRENT_SOURCE_DIR}/Interface/*.*")
file(GLOB Common "${CMAKE_CURRENT_SOURCE_DIR}/Common/*.*")

#Components
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/CSharpExcelConfiger)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/CppExcelConfiger)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/UnitGenerater)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/MysqlTemplateGenerater)

if(EXISTS "${XENGINE_DIR}/Engine/Component/Navigation/NavMesh.h"
    AND EXISTS "${XENGINE_DIR}/Engine/Component/Navigation/NavMesh.cpp"
    AND EXISTS "${PROJECT_SOURCE_DIR}/../recastnavigation/Detour/Include"
    AND EXISTS "${PROJECT_SOURCE_DIR}/../recastnavigation/Detour/Source")
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/NavMeshValidator)
else()
    message(WARNING "Skip NavMeshValidator: Navigation runtime or recastnavigation/Detour sources not found.")
endif()
