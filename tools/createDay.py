#!/usr/bin/env python
import os


def create_day(day):
    dir_name = "day" + str(f"{day:02d}")
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)
        open(dir_name + "/input.txt", 'a').close()
        open(dir_name + "/input_example.txt", 'a').close()
        with open(dir_name + "/day" + str(f"{day:02d}") + ".cpp", 'w') as f:
            f.write(f"""#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

namespace day{day:02d} {{

QString part1(const QString& fileName) 
{{
    return {{}};
}}

QString part2(const QString& fileName) 
{{
    return {{}};
}}

}}

int main()
{{
    qDebug() << "Part 1: " << day{day:02d}::part1(":/input.txt");
    qDebug() << "Part 2: " << day{day:02d}::part2(":/input.txt");
}}

""")
        with open(dir_name + "/CMakeLists.txt", 'w') as f:
            f.write(f"""add_executable(
        day{day:02d}
        day{day:02d}.cpp
        resources.qrc
)

target_include_directories(
        day{day:02d}
        PRIVATE
        ${{CMAKE_SOURCE_DIR}}/utils
)

target_link_libraries(
        day{day:02d}
        PRIVATE
        Qt6::Core
)
""")
        with open(dir_name + "/resources.qrc", 'w') as f:
            f.write(f"""<!DOCTYPE RCC><RCC version="1.0">
<qresource>
    <file>input.txt</file>
    <file>input_example.txt</file>
</qresource>
</RCC>
""")
        print("Created directory: " + dir_name)


if __name__ == "__main__":
    in_day = int(input("Enter day number: "))
    create_day(in_day)
