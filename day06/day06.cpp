#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

namespace Day06 {

struct Race
{
    qint64 time     = 0;
    qint64 distance = 0;

    [[nodiscard]] qint64 distanceAtTime(qint64 t) const { return t * (time - t); }

    [[nodiscard]] qint64 betterDistances() const
    {
        int numBetter = 0;

        for (qint64 i = 0; i < time; i++) {
            if (distanceAtTime(i) > distance)
                numBetter++;
            else if (numBetter > 0)
                break; // all future distances will be worse
        }

        return numBetter;
    }
};

QPair<QStringList, QStringList> parseTimesAndDistances(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileName;
        return {};
    }

    QStringList times;
    QStringList distances;

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        if (line.startsWith("Time:"))
            times = line.mid(line.indexOf(':') + 1).split(' ', Qt::SkipEmptyParts);
        else if (line.startsWith("Distance:"))
            distances = line.mid(line.indexOf(':') + 1).split(' ', Qt::SkipEmptyParts);
    }

    return {times, distances};
}

QVector<Race> parseRaces(const QString &fileName)
{
    QVector<Race> ret;

    auto [times, distances] = parseTimesAndDistances(fileName);
    Q_ASSERT(times.size() == distances.size());
    for (int i = 0; i < times.size(); i++)
        ret.append({times[i].toLongLong(), distances[i].toLongLong()});

    return ret;
}

Race parseRacePart2(const QString &fileName)
{
    auto [times, distances] = parseTimesAndDistances(fileName);
    Q_ASSERT(times.size() == distances.size());
    return {times.join("").toLongLong(), distances.join("").toLongLong()};
}

QString part1(const QString &fileName)
{
    auto races = parseRaces(fileName);
    return QString::number(
        std::accumulate(races.begin(), races.end(), static_cast<qint64>(1), [](qint64 i, const Race &race) {
            return i * race.betterDistances();
        }));
}

QString part2(const QString &fileName)
{
    return QString::number(parseRacePart2(fileName).betterDistances());
}

} // namespace Day06

int main()
{
    qInfo() << "Part 1:" << Day06::part1(":/input.txt");
    qInfo() << "Part 2:" << Day06::part2(":/input.txt");
    return 0;
}
