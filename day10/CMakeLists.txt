add_executable(
        day10
        day10.cpp
        resources.qrc
)

target_include_directories(
        day10
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day10
        PRIVATE
        Qt5::Core
)
