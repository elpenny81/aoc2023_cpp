qt_add_executable(
        day15
        day15.cpp
        resources.qrc
)

target_include_directories(
        day15
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day15
        PRIVATE
        Qt6::Core
)
