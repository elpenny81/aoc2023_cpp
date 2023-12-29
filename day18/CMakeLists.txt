add_executable(
        day18
        day18.cpp
        resources.qrc
)

target_include_directories(
        day18
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day18
        PRIVATE
        Qt6::Core
)
