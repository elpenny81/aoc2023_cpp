#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

#include <unordered_set>
#include <queue>

namespace day17 {

struct Map
{
    QVector<QVector<int>> grid;

    static Map parse(const QString &fileName)
    {
        Map   map;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return map;
        while (!file.atEnd()) {
            const QString line = file.readLine().trimmed();
            if (line.isEmpty())
                continue;

            QVector<int> row;
            for (const auto &c : line)
                row.append(static_cast<int>(c.unicode() - '0'));
            map.grid.append(row);
        }
        return map;
    }
};

enum class Direction { None, Left, Right, Up, Down };

struct Position
{
    qsizetype x = 0, y = 0;
    Direction direction = Direction::None;
    qsizetype straights = 1;

    bool operator==(const Position &other) const { return x == other.x && y == other.y; }

    // hash function for Position
    std::size_t operator()(const Position &pos) const
    {
        return (pos.x << 24) | (pos.y << 16) | (pos.straights << 8) | static_cast<int>(pos.direction);
    }
};

struct Cell
{
    Position  position{};
    qsizetype dist = 0;

    // Comparison operator for priority_queue
    bool operator>(const Cell &other) const { return this->dist > other.dist; }
};

QVector<Direction> validDirections(Direction dir)
{
    switch (dir) {
    case Direction::Left:
        return {Direction::Up, Direction::Down, Direction::Left};
    case Direction::Right:
        return {Direction::Up, Direction::Down, Direction::Right};
    case Direction::Up:
        return {Direction::Left, Direction::Right, Direction::Up};
    case Direction::Down:
        return {Direction::Left, Direction::Right, Direction::Down};
    default:
        break;
    }
    return {Direction::Left, Direction::Right, Direction::Up, Direction::Down};
}

QVector<Cell> validNeighbors(const Position              &pos,
                             const QVector<QVector<int>> &grid,
                             qsizetype                    minStraights,
                             qsizetype                    maxStraights)
{
    QVector<Cell> neighbors;
    for (const auto &dir : validDirections(pos.direction)) {
        if (pos.direction == dir && pos.straights >= maxStraights)
            continue;
        if (pos.direction != Direction::None && pos.direction != dir && pos.straights < minStraights)
            continue;
        const auto newX = pos.x + (dir == Direction::Left ? -1 : dir == Direction::Right ? 1 : 0);
        const auto newY = pos.y + (dir == Direction::Up ? -1 : dir == Direction::Down ? 1 : 0);
        if (newX < 0 || newY < 0 || newY >= grid.size() || newX >= grid[0].size())
            continue;
        neighbors.append(Cell{
            Position{newX, newY, dir, dir == pos.direction ? pos.straights + 1 : 1},
            grid[newY][newX],
        });
    }
    return neighbors;
}

qsizetype dijkstra(
    QVector<QVector<int>> &grid, Position source, Position destination, qsizetype minStraights, qsizetype maxStraights)
{
    std::priority_queue<Cell, QVector<Cell>, std::greater<>> pq;
    pq.push({source, 0});
    std::unordered_set<Position, Position> visited;

    while (!pq.empty()) {
        const auto current = pq.top();
        pq.pop();

        if (visited.contains(current.position))
            continue;
        if (current.position == destination && current.position.straights >= minStraights)
            return current.dist;

        visited.insert(current.position);

        for (auto &neighbor : validNeighbors(current.position, grid, minStraights, maxStraights)) {
            neighbor.dist += current.dist;
            pq.push(neighbor);
        }
    }

    return std::numeric_limits<qsizetype>::max();
}

QString part1(const QString &fileName)
{
    auto map = Map::parse(fileName);
    return QString::number(dijkstra(map.grid, {0, 0}, {map.grid[0].size() - 1, map.grid.size() - 1}, 1, 3));
}

QString part2(const QString &fileName)
{
    auto map = Map::parse(fileName);
    return QString::number(dijkstra(map.grid, {0, 0}, {map.grid[0].size() - 1, map.grid.size() - 1}, 4, 10));
}

} // namespace day17

int main()
{
    qDebug() << "Part 1: " << day17::part1(":/input.txt");
    qDebug() << "Part 2: " << day17::part2(":/input.txt");
}
