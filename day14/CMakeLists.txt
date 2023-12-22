add_executable(
        day14
        day14.cpp
        resources.qrc
)

target_include_directories(
        day14
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day14
        PRIVATE
        Qt5::Core
)
