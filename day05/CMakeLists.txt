
qt_add_executable(
        day05
        day05.cpp
        resources.qrc
)

target_include_directories(
        day05
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day05
        PRIVATE
        Qt6::Core
)
