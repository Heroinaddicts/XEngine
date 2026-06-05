include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/Common"
    "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
)

file(GLOB Common "${CMAKE_CURRENT_SOURCE_DIR}/Common/*.*")
file(GLOB Interface "${CMAKE_CURRENT_SOURCE_DIR}/Interface/*.*")

if (APPLE)
    find_package(Iconv)
    if (Iconv_FOUND)
        link_libraries(${Iconv_LIBRARIES})
    endif()
endif()

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Sender)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Forwarder)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Reciver)
