add_executable(
        day17
        day17.cpp
        resources.qrc
)

target_include_directories(
        day17
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day17
        PRIVATE
        Qt6::Core
)
