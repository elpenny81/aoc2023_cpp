
add_executable(
        day06
        day06.cpp
        resources.qrc
)

target_include_directories(
        day06
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day06
        PRIVATE
        Qt5::Core
)
