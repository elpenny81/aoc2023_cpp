#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

#include "utils.h"

namespace day14 {

enum class Direction { North, East, South, West };

struct Position
{
    int x = 0, y = 0;
};

void sortByDirection(Direction direction, QVector<Position> &positions)
{
    switch (direction) {
    case Direction::North:
        std::sort(positions.begin(), positions.end(), [](const auto &a, const auto &b) { return a.y < b.y; });
        break;
    case Direction::East:
        std::sort(positions.begin(), positions.end(), [](const auto &a, const auto &b) { return a.x > b.x; });
        break;
    case Direction::South:
        std::sort(positions.begin(), positions.end(), [](const auto &a, const auto &b) { return a.y > b.y; });
        break;
    case Direction::West:
        std::sort(positions.begin(), positions.end(), [](const auto &a, const auto &b) { return a.x < b.x; });
        break;
    }
}

struct Platform
{
    QVector<QVector<QChar>> grid;
    QVector<Position>       roundedRocks;

    static Platform parse(const QString &fileName)
    {
        Platform platform;
        QFile    file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return platform;
        for (std::size_t y = 0; !file.atEnd(); y++) {
            const QString line = file.readLine().trimmed();
            if (line.isEmpty())
                continue;

            QVector<QChar> row;
            for (const auto [x, c] : utils::enumerate(line)) {
                if (c == 'O')
                    platform.roundedRocks.append({static_cast<int>(x), static_cast<int>(y)});
                row.append(c);
            }
            platform.grid.append(row);
        }
        return platform;
    }

    void tilt(Direction direction)
    {
        sortByDirection(direction, roundedRocks);

        for (auto &roundedRock : roundedRocks) {
            int x = roundedRock.x, y = roundedRock.y;
            switch (direction) {
            case Direction::North:
                while (y >= 1 && grid[y - 1][x] == '.')
                    y--;
                break;
            case Direction::South:
                while ((y + 1) < grid.size() && grid[y + 1][x] == '.')
                    y++;
                break;
            case Direction::East:
                while ((x + 1) < grid[y].size() && grid[y][x + 1] == '.')
                    x++;
                break;
            case Direction::West:
                while (x >= 1 && grid[y][x - 1] == '.')
                    x--;
                break;
            }

            grid[roundedRock.y][roundedRock.x] = '.';
            grid[y][x]                         = 'O';
            roundedRock.x                      = x;
            roundedRock.y                      = y;
        }
    }

    bool operator==(const Platform &other) const { return grid == other.grid; }
};

QString part1(const QString &fileName)
{
    auto platform = Platform::parse(fileName);
    platform.tilt(Direction::North);
    const int sum = std::accumulate(platform.roundedRocks.begin(),
                                    platform.roundedRocks.end(),
                                    0,
                                    [&platform](int sum, const auto &roundedRock) {
                                        return sum + (platform.grid.size() - roundedRock.y);
                                    });
    return QString::number(sum);
}

QString part2(const QString &fileName)
{
    auto              platform = Platform::parse(fileName);
    QVector<Platform> visited;

    int steps = 0;
    while (!visited.contains(platform)) {
        visited.append(platform);
        platform.tilt(Direction::North);
        platform.tilt(Direction::West);
        platform.tilt(Direction::South);
        platform.tilt(Direction::East);
        steps++;
    }

    const int  beforeCycle = visited.indexOf(platform);
    const auto pos         = beforeCycle + ((1000000000 - beforeCycle) % (steps - beforeCycle));
    platform               = visited.at(pos);

    const int sum = std::accumulate(platform.roundedRocks.begin(),
                                    platform.roundedRocks.end(),
                                    0,
                                    [&platform](int sum, const auto &roundedRock) {
                                        return sum + (platform.grid.size() - roundedRock.y);
                                    });
    return QString::number(sum);
}

} // namespace day14

int main()
{
    qDebug() << "Part 1: " << day14::part1(":/input.txt");
    qDebug() << "Part 2: " << day14::part2(":/input.txt");
}
