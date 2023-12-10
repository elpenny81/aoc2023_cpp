#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

namespace day09 {

struct Sequence
{
    QVector<QVector<qint64>> numbers;

    static Sequence fromLine(const QString &line)
    {
        Sequence        result;
        QVector<qint64> inputLine;
        const auto      numbers = line.split(' ', Qt::SkipEmptyParts);
        for (const auto &number : numbers)
            inputLine.append(number.toLongLong());
        result.numbers.append(inputLine);
        return result;
    }

    [[nodiscard]] bool isFinished() const
    {
        return std::all_of(numbers.last().begin(), numbers.last().end(), [](qint64 number) { return number == 0; });
    }

    void analyze()
    {
        while (!isFinished()) {
            const auto     &lastLine = numbers.last();
            QVector<qint64> newLine;
            for (int i = 0; i < lastLine.size() - 1; ++i)
                newLine.append(lastLine[i + 1] - lastLine[i]);
            numbers.append(newLine);
        }
    }

    qint64 extrapolateRight()
    {
        for (int i = numbers.size() - 1; i > 0; --i)
            numbers[i - 1].append(numbers[i - 1].last() + numbers[i].last());
        return numbers[0].last();
    }

    qint64 extrapolateLeft()
    {
        for (int i = numbers.size() - 1; i > 0; --i)
            numbers[i - 1].prepend(numbers[i - 1].first() - numbers[i].first());
        return numbers[0].first();
    }
};

QVector<Sequence> fromFile(const QString &fileName)
{
    QVector<Sequence> result;
    QFile             file(fileName);
    Q_ASSERT(file.open(QIODevice::ReadOnly | QIODevice::Text));
    while (!file.atEnd())
        result.append(Sequence::fromLine(file.readLine().trimmed()));
    return result;
}

QString part1(const QString &fileName)
{
    qint64 sum = 0;
    for (auto &sequence : fromFile(fileName)) {
        sequence.analyze();
        sum += sequence.extrapolateRight();
    }
    return QString::number(sum);
}

QString part2(const QString &fileName)
{
    qint64 sum = 0;
    for (auto &sequence : fromFile(fileName)) {
        sequence.analyze();
        sum += sequence.extrapolateLeft();
    }
    return QString::number(sum);
}

} // namespace day09

int main()
{
    qDebug() << "Part 1: " << day09::part1(":/input.txt");
    qDebug() << "Part 2: " << day09::part2(":/input.txt");
}
