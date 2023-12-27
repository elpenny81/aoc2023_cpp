qt_add_executable(
        day08
        day08.cpp
        resources.qrc
)

target_include_directories(
        day08
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day08
        PRIVATE
        Qt6::Core
)
