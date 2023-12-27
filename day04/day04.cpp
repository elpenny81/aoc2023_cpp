#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

namespace Day04 {

QVector<int> intersect(const QVector<int> &a, const QVector<int> &b)
{
    QVector<int> result;
    for (const int &e : a) {
        if (b.contains(e))
            result.append(e);
    }
    return result;
}

QVector<int> parseNumbers(const QString &input)
{
    QVector<int> numbers;
    for (const auto numStrings = input.split(" ", Qt::SkipEmptyParts); const auto &s : numStrings)
        numbers.append(s.toInt());
    return numbers;
}

QPair<QVector<int>, QVector<int>> parseLine(const QString &line)
{
    const auto setStrings = line.mid(line.indexOf(":") + 1).split("|");
    return {parseNumbers(setStrings.at(0)), parseNumbers(setStrings.at(1))};
}

QString part1(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileName;
        return {};
    }

    int sum = 0;
    while (!file.atEnd()) {
        const auto &[winningNumbers, numbers] = parseLine(file.readLine().trimmed());
        const auto matchedNumbers             = intersect(winningNumbers, numbers);
        sum += matchedNumbers.isEmpty() ? 0 : (1 << (matchedNumbers.size() - 1));
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

    QStringList lines;
    while (!file.atEnd())
        lines.append(file.readLine().trimmed());

    QVector<int> instances(lines.size(), 1);
    for (int curLine = 0; curLine < lines.size(); curLine++) {
        const auto &[winningNumbers, numbers] = parseLine(lines.at(curLine));
        const auto matchedNumbers             = intersect(winningNumbers, numbers);
        for (int i = 0; i < matchedNumbers.size(); ++i)
            instances[curLine + i + 1] += instances[curLine];
    }

    return QString::number(std::accumulate(instances.begin(), instances.end(), 0));
}

} // namespace Day04

int main()
{
    qInfo() << "Part 1:" << Day04::part1(":/input.txt");
    qInfo() << "Part 2:" << Day04::part2(":/input.txt");
    return 0;
}
