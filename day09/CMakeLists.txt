qt_add_executable(
        day09
        day09.cpp
        resources.qrc
)

target_include_directories(
        day09
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day09
        PRIVATE
        Qt6::Core
)
