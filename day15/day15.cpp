#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

#include <utility>

#include "utils.h"

namespace day15 {

int checksum(const QString &input)
{
    int result = 0;
    for (const auto c : input)
        result = ((result + c.unicode()) * 17) % 256;
    return result;
}

struct HashMapData
{
    QString label;
    int     focusPower = 0;
};

using HashMap = QVector<QVector<HashMapData>>;

int findLabel(HashMap &map, const QString &label)
{
    const int c = checksum(label);
    for (int i = 0; i < map[c].size(); i++) {
        if (map[c][i].label == label)
            return i;
    }
    return -1;
}

void insert(HashMap &map, const HashMapData &data)
{
    const int c = checksum(data.label);
    if (const auto index = findLabel(map, data.label); index == -1)
        map[c].append(data);
    else
        map[c][index] = data;
}

void remove(HashMap &map, const QString &label)
{
    const int c = checksum(label);
    if (const int index = findLabel(map, label); index != -1)
        map[c].remove(index);
}

QStringList parse(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};
    const QString line = file.readLine().trimmed();
    return line.split(',');
}

QString part1(const QString &fileName)
{
    auto input = parse(fileName);
    return QString::number(std::accumulate(input.begin(), input.end(), 0, [](const int sum, const QString &word) {
        return sum + checksum(word);
    }));
}

QString part2(const QString &fileName)
{
    auto    input = parse(fileName);
    HashMap map;
    map.resize(256);
    for (auto &word : input) {
        if (word.contains('=')) {
            if (const auto parts = word.split('='); parts.size() == 2) {
                const auto label = parts[0].trimmed();
                const auto value = parts[1].trimmed().toInt();
                insert(map, {label, value});
            }
        } else if (word.endsWith('-')) {
            remove(map, word.left(word.size() - 1));
        }
    }

    std::size_t sum = 0;
    for (const auto &[hash, data] : utils::enumerate(map)) {
        for (auto const &[index, data] : utils::enumerate(data)) {
            sum += (hash + 1) * (index + 1) * data.focusPower;
        }
    }

    return QString::number(sum);
}
} // namespace day15

int main()
{
    qDebug() << "Part 1: " << day15::part1(":/input.txt");
    qDebug() << "Part 2: " << day15::part2(":/input.txt");
}
