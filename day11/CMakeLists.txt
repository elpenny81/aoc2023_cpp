qt_add_executable(
        day11
        day11.cpp
        resources.qrc
)

target_include_directories(
        day11
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day11
        PRIVATE
        Qt6::Core
)
