#include <QDebug>
#include <QFile>
#include <QObject>
#include <QString>
#include <QVector>

#include <ranges>

namespace day16 {

Q_NAMESPACE

enum class Direction { Up, Down, Left, Right };

Q_ENUM_NS(Direction)

struct Beam
{
    int       x = 0;
    int       y = 0;
    Direction direction{Direction::Right};

    void move()
    {
        switch (direction) {
        case Direction::Up:
            y--;
            break;
        case Direction::Down:
            y++;
            break;
        case Direction::Left:
            x--;
            break;
        case Direction::Right:
            x++;
            break;
        }
    }
};

struct Field
{
    QChar              c;
    int                visited = 0;
    QVector<Direction> handledDirections;
};

std::optional<Beam> handleFieldAction(QChar c, Beam &beam)
{
    struct Key
    {
        QChar     c;
        Direction d;

        bool operator<(const Key &other) const { return std::tie(c, d) < std::tie(other.c, other.d); }
    };

    struct Value
    {
        Direction           d = Direction::Right;
        std::optional<Beam> b;
    };

    static const QMap<Key, Value> map{
        {{'|', Direction::Right},  {Direction::Up, Beam{.direction = Direction::Down}}   },
        {{'|', Direction::Left},   {Direction::Up, Beam{.direction = Direction::Down}}   },
        {{'|', Direction::Up},     {Direction::Up, {}}                                   },
        {{'|', Direction::Down},   {Direction::Down, {}}                                 },
        {{'-', Direction::Up},     {Direction::Left, Beam{.direction = Direction::Right}}},
        {{'-', Direction::Down},   {Direction::Left, Beam{.direction = Direction::Right}}},
        {{'-', Direction::Left},   {Direction::Left, {}}                                 },
        {{'-', Direction::Right},  {Direction::Right, {}}                                },
        {{'/', Direction::Up},     {Direction::Right, {}}                                },
        {{'/', Direction::Down},   {Direction::Left, {}}                                 },
        {{'/', Direction::Left},   {Direction::Down, {}}                                 },
        {{'/', Direction::Right},  {Direction::Up, {}}                                   },
        {{'\\', Direction::Up},    {Direction::Left, {}}                                 },
        {{'\\', Direction::Down},  {Direction::Right, {}}                                },
        {{'\\', Direction::Left},  {Direction::Up, {}}                                   },
        {{'\\', Direction::Right}, {Direction::Down, {}}                                 },
        {{'.', Direction::Up},     {Direction::Up, {}}                                   },
        {{'.', Direction::Down},   {Direction::Down, {}}                                 },
        {{'.', Direction::Left},   {Direction::Left, {}}                                 },
        {{'.', Direction::Right},  {Direction::Right, {}}                                },
    };

    auto res = map.value({c, beam.direction});
    if (res.b) {
        res.b->x = beam.x;
        res.b->y = beam.y;
    }
    beam.direction = res.d;
    return res.b;
}

struct Map
{
    QVector<QVector<Field>> grid;
    QVector<Beam>           beams;

    static Map parse(const QString &fileName)
    {
        Map   map;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return map;
        while (!file.atEnd()) {
            const QString  line = file.readLine().trimmed();
            QVector<Field> row;
            for (const QChar c : line)
                row.append({c});
            map.grid.append(row);
        }
        return map;
    }

    [[nodiscard]] bool isOnMap(int x, int y) const { return x >= 0 && y >= 0 && x < grid[0].size() && y < grid.size(); }

    void moveUntilCollision()
    {
        for (auto &b : beams) {
            while (isOnMap(b.x, b.y) && grid[b.y][b.x].c == '.') {
                grid[b.y][b.x].visited++;
                b.move();
            }
        }
    }

    void checkCurrentField()
    {
        // remove beams that are out of bounds
        beams.erase(std::ranges::remove_if(beams, [this](const Beam &b) { return !isOnMap(b.x, b.y); }).begin(),
                    beams.end());

        QVector<Beam> beamsToAdd;
        for (auto &b : beams) {
            grid[b.y][b.x].visited++;
            if (auto res = handleFieldAction(grid[b.y][b.x].c, b))
                beamsToAdd.append(*res);
        }
        beams.append(beamsToAdd);

        // remove beams that were already handled
        beams.erase(std::ranges::remove_if(beams,
                                           [this](const Beam &b) {
                                               return grid[b.y][b.x].handledDirections.contains(b.direction);
                                           })
                        .begin(),
                    beams.end());

        std::ranges::for_each(beams, [this](Beam &b) {
            grid[b.y][b.x].handledDirections.append(b.direction);
            b.move();
        });
    }

    [[nodiscard]] int visitedTiles() const
    {
        int sum = 0;
        for (const auto &row : grid)
            for (const auto &field : row)
                if (field.visited > 0)
                    sum++;
        return sum;
    }

    void exec()
    {
        while (!beams.isEmpty()) {
            moveUntilCollision();
            checkCurrentField();
        }
    }
};

QString part1(const QString &fileName)
{
    auto map = Map::parse(fileName);
    map.beams.append({0, 0, Direction::Right});
    map.exec();
    return QString::number(map.visitedTiles());
}

QString part2(const QString &fileName)
{
    const auto initMap = Map::parse(fileName);
    const int  rows    = initMap.grid.size();
    const int  cols    = initMap.grid[0].size();

    QVector<Beam> startBeams;
    for (int y = 0; y < rows; y++)
        startBeams.append({0, y, Direction::Right});
    for (int y = 0; y < rows; y++)
        startBeams.append({cols - 1, y, Direction::Left});
    for (int x = 0; x < cols; x++)
        startBeams.append({x, 0, Direction::Down});
    for (int x = 0; x < cols; x++)
        startBeams.append({x, rows - 1, Direction::Up});

    int result = 0;
    for (auto startBeam : startBeams) {
        auto map = initMap;
        map.beams.append(startBeam);
        map.exec();
        result = std::max(result, map.visitedTiles());
    }

    return QString::number(result);
}

} // namespace day16

int main()
{
    qDebug() << "Part 1: " << day16::part1(":/input.txt");
    qDebug() << "Part 2: " << day16::part2(":/input.txt");
}

#include "day16.moc"