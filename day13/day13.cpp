#include "utils.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>

namespace day13 {

int countDifferentChars(const QString &s1, const QString &s2)
{
    auto data = utils::zip(s1, s2);
    return std::accumulate(data.begin(), data.end(), 0, [](int sum, const auto &entry) {
        return sum + (std::get<0>(entry) != std::get<1>(entry));
    });
}

int findMirror(const QVector<QString> &grid, bool findSmudges)
{
    for (int i = 1; i < grid.size(); i++) {
        auto above = grid.mid(0, i);
        std::reverse(above.begin(), above.end());
        auto below = grid.mid(i);
        auto data  = utils::zip(above, below);

        if (findSmudges) {
            if (std::accumulate(data.begin(),
                                data.end(),
                                0,
                                [](int sum, const auto &entry) {
                                    return sum + countDifferentChars(std::get<0>(entry), std::get<1>(entry));
                                })
                == 1)
                return i;
        } else {
            if (std::all_of(data.begin(), data.end(), [](const auto &entry) {
                    return std::get<0>(entry) == std::get<1>(entry);
                }))
                return i;
        }
    }
    return 0;
}

struct Map
{
    QVector<QString> rows;
    QVector<QString> columns;

    static Map parse(QTextStream &stream)
    {
        Map map;
        while (!stream.atEnd()) {
            const QString line = stream.readLine().trimmed();
            if (line.isEmpty())
                break;
            map.rows.append(line);
            for (int i = 0; i < line.size(); ++i) {
                if (map.columns.size() <= i)
                    map.columns.append("");
                map.columns[i].append(line[i]);
            }
        }
        return map;
    }
};

QVector<Map> parse(const QString &fileName)
{
    QVector<Map> maps;
    QFile        file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return maps;
    QTextStream stream(&file);
    while (!stream.atEnd())
        maps.append(Map::parse(stream));
    return maps;
}

QString part1(const QString &fileName)
{
    const auto maps = parse(fileName);
    qint64     sum  = 0;

    for (const auto &map : maps) {
        sum += findMirror(map.rows, false) * 100;
        sum += findMirror(map.columns, false);
    }

    return QString::number(sum);
}

QString part2(const QString &fileName)
{
    const auto maps = parse(fileName);
    qint64     sum  = 0;

    for (const auto &map : maps) {
        sum += findMirror(map.rows, true) * 100;
        sum += findMirror(map.columns, true);
    }

    return QString::number(sum);
}

} // namespace day13

int main()
{
    qDebug() << "Part 1: " << day13::part1(":/input.txt");
    qDebug() << "Part 2: " << day13::part2(":/input.txt");
}
