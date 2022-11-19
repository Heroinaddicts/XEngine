
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/General"
)

file(GLOB General "General/*.*")

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Redis)