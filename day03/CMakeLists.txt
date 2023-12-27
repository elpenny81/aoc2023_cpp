
qt_add_executable(
        day03
        day03.cpp
        resources.qrc
)

target_link_libraries(
        day03
        PRIVATE
        Qt6::Core
)
