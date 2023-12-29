#include <QDebug>
#include <QFile>
#include <QString>

namespace day18 {

enum class Direction { Right, Down, Left, Up };

struct Point
{
    qint64 x = 0;
    qint64 y = 0;

    bool operator==(const Point &other) const { return x == other.x && y == other.y; }

    Point operator+(const Point &other) const { return Point{x + other.x, y + other.y}; }

    Point operator*(const qint64 &m) const { return Point{x * m, y * m}; }
};

static QMap<Direction, Point> motions{
    {Direction::Right, {1, 0} },
    {Direction::Down,  {0, 1} },
    {Direction::Left,  {-1, 0}},
    {Direction::Up,    {0, -1}},
};

Direction parseDirection(const QChar &c)
{
    switch (c.unicode()) {
    case 'U':
        return Direction::Up;
    case 'D':
        return Direction::Down;
    case 'L':
        return Direction::Left;
    case 'R':
        return Direction::Right;
    default:
        throw std::runtime_error("Invalid direction");
    }
}

struct Rule
{
    Direction direction{Direction::Right};
    qint64    steps = 0;
};

using Rules = QVector<Rule>;

Rules parseRules(const QString &fileName, bool part2 = false)
{
    Rules rules;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};
    while (!file.atEnd()) {
        const QString line = file.readLine().trimmed();
        if (line.isEmpty())
            continue;
        const auto parts = line.split(' ');

        if (part2) {
            const auto color     = parts.value(2).removeLast().remove(0, 2).toUInt(nullptr, 16);
            const auto direction = static_cast<Direction>(color & 0xf);
            const auto steps     = (color >> 4) & 0xfffff;
            rules.append({direction, static_cast<qint64>(steps)});
        } else {
            rules.append({parseDirection(parts.at(0).at(0)), parts.at(1).toInt()});
        }
    }
    return rules;
}

QString process(const QString &fileName, bool part2)
{
    // Green's theorem
    const auto rules = parseRules(fileName, part2);

    qint64 perimeter = 0;
    qint64 area      = 0;
    Point  p{0, 0};

    for (const auto &[direction, steps] : rules) {
        const auto &motion = motions[direction];
        auto        dp     = motion * steps;
        p                  = p + dp;
        perimeter += steps;
        area += p.x * dp.y;
    }

    return QString::number(area + perimeter / 2 + 1);
}

QString part1(const QString &fileName)
{
    return process(fileName, false);
}

QString part2(const QString &fileName)
{
    return process(fileName, true);
}

} // namespace day18

int main()
{
    qDebug() << "Part 1: " << day18::part1(":/input.txt");
    qDebug() << "Part 2: " << day18::part2(":/input.txt");
}
