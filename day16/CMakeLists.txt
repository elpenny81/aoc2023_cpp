qt_add_executable(
        day16
        day16.cpp
        resources.qrc
)

target_include_directories(
        day16
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day16
        PRIVATE
        Qt6::Core
)
