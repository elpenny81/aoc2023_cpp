
add_executable(
    day02
    day02.cpp 
    resources.qrc
)

target_link_libraries(
    day02 
    PRIVATE
    Qt5::Core 
)
