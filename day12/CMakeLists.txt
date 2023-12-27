qt_add_executable(
        day12
        day12.cpp
        resources.qrc
)

target_include_directories(
        day12
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day12
        PRIVATE
        Qt6::Core
)
