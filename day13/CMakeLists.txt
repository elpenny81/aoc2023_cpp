qt_add_executable(
        day13
        day13.cpp
        resources.qrc
)

target_include_directories(
        day13
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day13
        PRIVATE
        Qt6::Core
)
