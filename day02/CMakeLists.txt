
qt_add_executable(
        day02
        day02.cpp
        resources.qrc
)

target_link_libraries(
        day02
        PRIVATE
        Qt6::Core
)
