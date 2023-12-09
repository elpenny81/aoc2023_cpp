#include "literals.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

using namespace utils::literals::integer;

namespace Day05 {

struct RangeMap
{
    qint64 start  = 0;
    qint64 length = 0;
    qint64 target = 0;

    struct MapResult
    {
        bool   mapped = false;
        qint64 value  = 0;
    };

    [[nodiscard]] MapResult mappedValue(qint64 in) const
    {
        if (in >= start && in < (start + length)) {
            return {true, target + (in - start)};
        }
        return {false, in};
    }

    [[nodiscard]] RangeMap mappedRange(const RangeMap &in) const
    {
        if (in.start >= start && in.start < (start + length)) {
            return {target + (in.start - start), in.length, in.target};
        }
        return in;
    }

    [[nodiscard]] bool overlaps(const RangeMap &in) const
    {
        return (in.start < end() && in.end() > start) || (in.end() > start && in.start < end());
    }

    [[nodiscard]] qint64 end() const
    {
        return start + length;
    }

    [[nodiscard]] RangeMap filteredRange(const RangeMap &in) const
    {
        auto st = qMax(in.start, start);
        auto en = qMin(in.end(), end());
        return {st, qMax(en - st, 0_i64), 0};
    }

    [[nodiscard]] QVector<RangeMap> nonMappedRanges(const RangeMap &in) const
    {
        QVector<RangeMap> ret;
        if (!overlaps(in))
            return {in};

        if (in.start < start && in.end() > start)
            ret.append({in.start, qMax(start - in.start, 0_i64), in.target});

        if (in.end() > end() && in.start < end())
            ret.append({end(), qMax(in.end() - end(), 0_i64), in.target});

        return ret;
    }

    [[nodiscard]] bool operator<(const RangeMap &other) const
    {
        return start < other.start;
    }
};

struct Map
{
    QVector<RangeMap> map;

    [[nodiscard]] qint64 mapped(qint64 in) const
    {
        for (const auto &m : map) {
            auto res = m.mappedValue(in);
            if (res.mapped)
                return res.value;
        }

        return in;
    }
};

RangeMap parseRangeMap(const QString &line)
{
    auto parts = line.split(' ', Qt::SkipEmptyParts);
    assert(parts.size() == 3);
    return {parts.at(1).toLongLong(), parts.at(2).toLongLong(), parts.at(0).toLongLong()};
}

QVector<qint64> parseSeed(const QString &line)
{
    auto            seedsString = line.mid(line.indexOf(":") + 1).trimmed();
    auto            seeds       = seedsString.split(' ', Qt::SkipEmptyParts);
    QVector<qint64> ret;
    for (auto const &s : seeds)
        ret.append(s.toLongLong());
    return ret;
}

struct Data
{
    QVector<qint64> seeds;
    QVector<Map>    maps;

    static Data parse(const QString &fileName)
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open file" << fileName;
            return {};
        }

        Data ret;
        while (!file.atEnd()) {
            const QString line = file.readLine().trimmed();
            if (line.startsWith("seeds:"))
                ret.seeds = parseSeed(line);
            else if (line.contains("-to-"))
                ret.maps.append(Map{});
            else if (line.isEmpty())
                continue;
            else
                ret.maps.last().map.append(parseRangeMap(line));
        }
        return ret;
    }

    [[nodiscard]] QVector<RangeMap> seedToRanges() const
    {
        QVector<RangeMap> ret;
        for (int i = 0; i < seeds.size(); i+=2) {
            ret.append(RangeMap{seeds.at(i), seeds.at(i+1), 0});
        }
        return ret;
    }
};

QString part1(const QString &fileName)
{
    auto data = Data::parse(fileName);
    qint64 result = std::numeric_limits<qint64>::max();
    for (const auto &seed: data.seeds) {
        qint64 location = seed;
        for (auto &map: data.maps)
            location = map.mapped(location);
        result = qMin(result, location);
    }
    return QString::number(result);
}

QString part2(const QString &fileName)
{
    auto data = Data::parse(fileName);
    QVector<RangeMap> sources = data.seedToRanges();

    for (auto &map: data.maps) {
        QVector<RangeMap> results;
        for (auto &source : sources) {
            QVector<RangeMap> current {source};
            for (auto &mapRanges : map.map) {
                QVector<RangeMap> remaining;
                for (auto &currentRange : current) {
                    remaining.append(mapRanges.nonMappedRanges(currentRange));
                    auto filtered = mapRanges.filteredRange(currentRange);
                    if (filtered.length == 0)
                        continue;
                    results.append(mapRanges.mappedRange(filtered));
                }
                current = remaining;
            }
            results.append(current);
        }
        sources = results;
    }

    return QString::number(std::min_element(sources.begin(), sources.end())->start);
}

} // namespace Day05

int main()
{
    qInfo() << "Part 1:" << Day05::part1(":/input.txt");
    qInfo() << "Part 2:" << Day05::part2(":/input.txt");
    return 0;
}
