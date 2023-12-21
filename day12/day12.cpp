#include <QDebug>
#include <QFile>
#include <QMap>
#include <QString>
#include <QVector>

#include "utils.h"

namespace day12 {

struct TestLine
{
    QString      record;
    QVector<int> expected;
};

QVector<TestLine> parse(const QString &fileName)
{
    QVector<TestLine> result;
    QFile             file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return result;

    while (!file.atEnd()) {
        QString     line  = file.readLine().trimmed();
        QStringList parts = line.split(" ");
        TestLine    testLine;
        testLine.record = parts[0];
        for (auto &numString : parts[1].split(","))
            testLine.expected.append(numString.toInt());
        result.append(testLine);
    }
    return result;
}

qint64 calculateWays(const QString &record, const QVector<int> &expected)
{
    using Positions = QMap<qint64, qint64>;
    Positions positions{
        {0, 1}
    };
    for (auto const &[index, contiguous] : utils::enumerate(expected)) {
        Positions newPositions;
        for (QMapIterator it(positions); it.hasNext();) {
            it.next();
            auto nextExpected = expected.mid(index + 1);
            for (int n = it.key(); n <= (record.length() - utils::sum(nextExpected) + nextExpected.length()); n++) {
                if ((n + contiguous - 1) < record.length() && !record.mid(n, contiguous).contains('.')) {
                    if ((index == (expected.length() - 1) && !record.mid(n + contiguous).contains('#'))
                        || (index < (expected.length() - 1) && (n + contiguous) < record.length()
                            && record.at(n + contiguous) != '#'))
                        newPositions[n + contiguous + 1] = newPositions[n + contiguous + 1] + it.value();
                }
                if (record.at(n) == '#')
                    break;
            }
        }
        positions = newPositions;
    }
    return utils::sum(positions);
}

QString part1(const QString &fileName)
{
    auto   testData = parse(fileName);
    qint64 ways     = 0;
    for (const auto &[record, expected] : testData)
        ways += calculateWays(record, expected);
    return QString::number(ways);
}

QString part2(const QString &fileName)
{
    auto   testData = parse(fileName);
    qint64 ways     = 0;
    for (const auto &[_record, _expected] : testData) {
        auto record   = _record;
        auto expected = _expected;
        for (int i = 0; i < 4; i++) {
            record += "?" + _record;
            expected.append(_expected);
        }
        ways += calculateWays(record, expected);
    }
    return QString::number(ways);
}

} // namespace day12

int main()
{
    qDebug() << "Part 1: " << day12::part1(":/input.txt");
    qDebug() << "Part 2: " << day12::part2(":/input.txt");
}
