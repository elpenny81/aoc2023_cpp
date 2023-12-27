
qt_add_executable(
        day04
        day04.cpp
        resources.qrc
)

target_link_libraries(
        day04
        PRIVATE
        Qt6::Core
)
