add_executable(
    day01
    day01.cpp 
    resources.qrc
)

target_link_libraries(
    day01 
    PRIVATE
    Qt5::Core 
)
