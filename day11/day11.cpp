#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

namespace day11 {

struct Coordinate 
{
    qint64 x = 0;
    qint64 y = 0;

    bool operator==(const Coordinate& other) const 
    {
        return x == other.x && y == other.y;
    }

    qint64 distanceTo(const Coordinate& other) const 
    {
        return qAbs(x - other.x) + qAbs(y - other.y);
    }
};

QVector<Coordinate> parse(const QString& fileName, qint64 extra = 1) 
{
    QVector<Coordinate> result;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for reading";
        return result;
    }

    qint64 y = 0;
    qint64 max_x = 0;
    QSet<qint64> usedRows;

    while (!file.atEnd()) {
        const QString line = file.readLine().trimmed();
        bool empty = true;
        qint64 x = 0;
        for (const QChar c : line) {
            if (c == '#') {
                result.append({x, y});
                empty = false;
                max_x = qMax(max_x, x);
                usedRows.insert(x);
            }
            x++;
        }
        y += 1 + (empty ? extra : 0);
    }

    for (qint64 x = max_x; x >= 0; x--) {
        if (!usedRows.contains(x)) {
            for (auto &c : result) {
                if (c.x > x)
                    c.x += extra;
            }
        }
    }

    return result;
}

QString part1(const QString& fileName) 
{
    auto coordinates = parse(fileName);
    qint64 result = 0;
    for (int i = 0; i < coordinates.size(); i++)
        for (int j = i + 1; j < coordinates.size(); j++)
            result += coordinates[i].distanceTo(coordinates[j]);
    return QString::number(result);
}

QString part2(const QString& fileName) 
{
    auto coordinates = parse(fileName, 999999);
    qint64 result = 0;
    for (int i = 0; i < coordinates.size(); i++)
        for (int j = i + 1; j < coordinates.size(); j++)
            result += coordinates[i].distanceTo(coordinates[j]);
    return QString::number(result);
}

}

int main()
{
    qDebug() << "Part 1: " << day11::part1(":/input.txt");
    qDebug() << "Part 2: " << day11::part2(":/input.txt");
}

