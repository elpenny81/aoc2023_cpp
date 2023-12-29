add_executable(
        day19
        day19.cpp
        resources.qrc
)

target_include_directories(
        day19
        PRIVATE
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(
        day19
        PRIVATE
        Qt6::Core
)
