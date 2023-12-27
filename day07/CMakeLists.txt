qt_add_executable(
        day07
        day07.cpp
        resources.qrc
)

target_include_directories(
        day07
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day07
        PRIVATE
        Qt6::Core
)
