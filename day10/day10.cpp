#include <QDebug>
#include <QFile>
#include <QHash>
#include <QMap>
#include <QObject>
#include <QString>
#include <QVector>

#include <algorithm>

namespace day10 {

Q_NAMESPACE

enum class Tile {
    VerticalPipe,
    HorizontalPipe,
    BendNorthEast,
    BendNorthWest,
    BendSouthEast,
    BendSouthWest,
    Ground,
    Start,
};

Q_ENUM_NS(Tile)

enum class Direction {
    North,
    East,
    South,
    West,
};

Q_ENUM_NS(Direction)

struct Point
{
    int x = 0;
    int y = 0;

    bool operator<(const Point &other) const { return x < other.x || (x == other.x && y < other.y); }
};

QMap<QChar, Tile> parserMap{
    {'|', Tile::VerticalPipe  },
    {'-', Tile::HorizontalPipe},
    {'L', Tile::BendNorthEast },
    {'J', Tile::BendNorthWest },
    {'F', Tile::BendSouthEast },
    {'7', Tile::BendSouthWest },
    {'.', Tile::Ground        },
    {'S', Tile::Start         },
};

using Map = QMap<Point, Tile>;

struct ParserResult
{
    Map   map;
    Point start;
};

std::optional<Tile> convertStart(const Map &map, const Point &startPoint);

ParserResult parseFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file: " << fileName;
        return {};
    }

    ParserResult result;
    Point        point;
    while (!file.atEnd()) {
        const QString line = file.readLine().trimmed();
        for (const QChar c : line) {
            result.map.insert(point, parserMap[c]);
            if (c == 'S')
                result.start = point;
            point.x++;
        }
        point.x = 0;
        point.y++;
    }

    result.map[result.start] = *convertStart(result.map, result.start);

    return result;
}

Point move(const Point &point, Direction direction)
{
    switch (direction) {
    case Direction::North:
        return {point.x, point.y - 1};
    case Direction::East:
        return {point.x + 1, point.y};
    case Direction::South:
        return {point.x, point.y + 1};
    case Direction::West:
        return {point.x - 1, point.y};
    }
    return {};
}

QVector<Direction> directions(Tile tile)
{
    switch (tile) {
    case Tile::VerticalPipe:
        return {Direction::North, Direction::South};
    case Tile::HorizontalPipe:
        return {Direction::East, Direction::West};
    case Tile::BendNorthEast:
        return {Direction::North, Direction::East};
    case Tile::BendNorthWest:
        return {Direction::North, Direction::West};
    case Tile::BendSouthEast:
        return {Direction::South, Direction::East};
    case Tile::BendSouthWest:
        return {Direction::South, Direction::West};
    default:
        return {};
    }
}

Direction invertedDirection(Direction in)
{
    switch (in) {
    case Direction::North:
        return Direction::South;
    case Direction::East:
        return Direction::West;
    case Direction::South:
        return Direction::North;
    case Direction::West:
        return Direction::East;
    }
    return in;
}

std::optional<Point> canMove(const Map &map, const Point &point, Direction to)
{
    const auto pointTo = move(point, to);
    auto       res     = map.contains(pointTo) && directions(map.value(pointTo)).contains(invertedDirection(to));
    return res ? std::optional<Point>(pointTo) : std::nullopt;
}

std::optional<Tile> convertStart(const Map &map, const Point &startPoint)
{
    if (canMove(map, startPoint, Direction::North) && canMove(map, startPoint, Direction::South))
        return Tile::VerticalPipe;
    if (canMove(map, startPoint, Direction::East) && canMove(map, startPoint, Direction::West))
        return Tile::HorizontalPipe;
    if (canMove(map, startPoint, Direction::East) && canMove(map, startPoint, Direction::North))
        return Tile::BendNorthEast;
    if (canMove(map, startPoint, Direction::West) && canMove(map, startPoint, Direction::North))
        return Tile::BendNorthWest;
    if (canMove(map, startPoint, Direction::East) && canMove(map, startPoint, Direction::South))
        return Tile::BendSouthEast;
    if (canMove(map, startPoint, Direction::West) && canMove(map, startPoint, Direction::South))
        return Tile::BendSouthWest;
    return {};
}

QMap<Point, int> createVisitedMap(const Map &map, const Point &start)
{
    QMap<Point, int> visited;

    QVector<Point> pointsToVisit = {start};
    while (!pointsToVisit.isEmpty()) {
        const Point point         = pointsToVisit.takeFirst();
        const int   pointDistance = visited.value(point, 0);
        for (const Direction direction : directions(map.value(point))) {
            if (auto pointTo = canMove(map, point, direction)) {
                if (!visited.contains(*pointTo)) {
                    visited.insert(*pointTo, pointDistance + 1);
                    pointsToVisit.append(*pointTo);
                }
            }
        }
    }

    return visited;
}

QString part1(const QString &fileName)
{
    const auto [map, start] = parseFile(fileName);
    const auto visited      = createVisitedMap(map, start);
    return QString::number(*std::ranges::max_element(visited, [](const auto &a, const auto &b) { return a < b; }));
}

QString part2(const QString &fileName)
{
    const auto [map, start] = parseFile(fileName);
    const auto visited      = createVisitedMap(map, start);
    // ray-casting algorithm
    const auto cntInv = [&](const Point &point) {
        int count = 0;
        for (int i = 0; i < point.x; ++i) {
            const Point p = {i, point.y};
            if (!visited.contains(p))
                continue;
            auto tile = map.value(p);
            if (tile == Tile::VerticalPipe || tile == Tile::BendNorthEast || tile == Tile::BendNorthWest)
                count++;
        }
        return count;
    };
    const auto is_odd = [](int x) { return x % 2 == 1; };

    return QString::number(std::ranges::count_if(map.keys(), [&](const auto &point) {
        return !visited.contains(point) && is_odd(cntInv(point));
    }));
}

} // namespace day10

int main()
{
    qDebug() << "Part 1: " << day10::part1(":/input.txt");
    qDebug() << "Part 2: " << day10::part2(":/input.txt");
}

#include "day10.moc"
