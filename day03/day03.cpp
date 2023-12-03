#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

namespace Day03 {

bool isInRange(const int x1, const int length, const int x2)
{
    return (x1 - 1) <= x2 && x2 <= x1 + length;
}

enum class Type { None, Number, Symbol, Asterisk };

struct Part
{
    Type    type = Type::None;
    QString value;
    int     pos = 0;

    void clear()
    {
        type = Type::None;
        value.clear();
        pos = 0;
    }

    [[nodiscard]] QVector<Part> adjacents(const Type adjType, const QVector<QVector<Part>> &parts) const
    {
        QVector<Part> result;
        for (auto &row : parts) {
            for (auto &part : row) {
                if (part.type == adjType
                    && (isInRange(pos, value.length(), part.pos) || isInRange(part.pos, part.value.length(), pos))) {
                    result.append(part);
                }
            }
        }
        return result;
    }
};

QVector<Part> parseLine(const QString &line, bool isPart2 = false)
{
    QVector<Part> parts;
    Part          part;
    int           pos = 0;
    for (const auto &c : line) {
        if (c.isDigit()) {
            if (part.type == Type::None)
                part.pos = pos;
            part.type = Type::Number;
            part.value += c;
        } else {
            if (part.type != Type::None) {
                parts.append(part);
                part.clear();
            }

            if (isPart2 && c == '*')
                parts.append({.type = Type::Asterisk, .value = c, .pos = pos});
            else if (c != '.')
                parts.append({.type = Type::Symbol, .value = c, .pos = pos});
        }
        pos++;
    }

    if (part.type != Type::None)
        parts.append(part);

    return parts;
}

QString part1(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileName;
        return {};
    }

    QVector<QVector<Part>> parts;
    while (!file.atEnd()) {
        const QString line = file.readLine();
        parts.append(parseLine(line.trimmed(), false));
    }

    int sum = 0;

    for (int row = 0; row < parts.size(); row++) {
        for (auto const &part : parts[row]) {
            if (part.type == Type::Number) {
                QVector<QVector<Part>> adjacentRows;
                if (row > 0)
                    adjacentRows.append(parts[row - 1]);
                if (row < parts.size() - 1)
                    adjacentRows.append(parts[row + 1]);
                adjacentRows.append(parts[row]);

                if (!part.adjacents(Type::Symbol, adjacentRows).empty())
                    sum += part.value.toInt();
            }
        }
    }

    return QString::number(sum);
}

QString part2(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileName;
        return {};
    }

    QVector<QVector<Part>> parts;
    while (!file.atEnd()) {
        const QString line = file.readLine();
        parts.append(parseLine(line.trimmed(), true));
    }

    int sum = 0;
    for (int row = 0; row < parts.size(); row++) {
        for (auto const &part : parts[row]) {
            if (part.type == Type::Asterisk) {
                QVector<QVector<Part>> adjacentRows;
                if (row > 0)
                    adjacentRows.append(parts[row - 1]);
                if (row < parts.size() - 1)
                    adjacentRows.append(parts[row + 1]);
                adjacentRows.append(parts[row]);

                if (auto adjacentParts = part.adjacents(Type::Number, adjacentRows); adjacentParts.size() == 2) {
                    sum += adjacentParts[0].value.toInt() * adjacentParts[1].value.toInt();
                }
            }
        }
    }
    return QString::number(sum);
}

} // namespace Day03

int main()
{
    qInfo() << "Part 1:" << Day03::part1(":/input.txt");
    qInfo() << "Part 2:" << Day03::part2(":/input.txt");
    return 0;
}
