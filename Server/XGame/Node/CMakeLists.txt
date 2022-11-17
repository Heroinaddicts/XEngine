
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/General"
    "${CMAKE_CURRENT_SOURCE_DIR}/Connecter"
)

file(GLOB General "General/*.*")

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Connecter)