#include "literals.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

using namespace utils::literals::integer;

namespace day07 {

Q_NAMESPACE

static const QMap<QChar, int> cardValues{
    {'2', 2 },
    {'3', 3 },
    {'4', 4 },
    {'5', 5 },
    {'6', 6 },
    {'7', 7 },
    {'8', 8 },
    {'9', 9 },
    {'T', 10},
    {'J', 11},
    {'Q', 12},
    {'K', 13},
    {'A', 14},
    {'X', 1 }
};

enum class Ranks {
    HighCard,
    OnePair,
    TwoPairs,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind,
};

Q_ENUM_NS(Ranks)

struct Hand
{
    QString cards;
    quint64 bid   = 0;
    bool    joker = false;

    static Hand fromString(const QString &line, bool joker = false)
    {
        Hand       result;
        const auto sep = line.split(" ");
        Q_ASSERT(sep.size() == 2);
        result.cards = sep[0];
        result.bid   = sep[1].toInt();
        result.joker = joker;
        return result;
    }

    [[nodiscard]] Ranks rank() const
    {
        QMap<QChar, int> cardCounts;
        for (const auto &card : cards)
            cardCounts[card]++;

        if (joker) {
            if (cardCounts.contains('X')) {
                const auto cnt = cardCounts['X'];
                cardCounts.remove('X');

                QMap<QChar, int> maxCards;
                int              maxCnt = 0;

                QMapIterator it(cardCounts);
                while (it.hasNext()) {
                    it.next();
                    if (it.value() > maxCnt) {
                        maxCnt = it.value();
                        maxCards.clear();
                        maxCards.insert(it.key(), it.value());
                    } else if (it.value() == maxCnt) {
                        maxCards.insert(it.key(), it.value());
                    }
                }

                auto keys = maxCards.keys();
                // convert to values
                auto maxIt = std::ranges::max_element(keys, {}, [&](const auto &key) { return cardValues[key]; });
                if (maxIt != keys.end()) {
                    cardCounts[*maxIt] += cnt;
                } else {
                    cardCounts['A'] += cnt;
                }
            }
        }
        auto values = cardCounts.values();
        std::sort(values.begin(), values.end());
        std::reverse(values.begin(), values.end());

        for (auto cnt : values) {
            if (cnt == 5)
                return Ranks::FiveOfAKind;
            if (cnt == 4)
                return Ranks::FourOfAKind;
            if (cnt == 3) {
                if (cardCounts.size() == 2)
                    return Ranks::FullHouse;
                return Ranks::ThreeOfAKind;
            }
            if (cnt == 2) {
                if (cardCounts.size() == 3)
                    return Ranks::TwoPairs;
                return Ranks::OnePair;
            }
        }
        return Ranks::HighCard;
    }

    [[nodiscard]] bool operator<(const Hand &other) const
    {
        if (rank() != other.rank())
            return rank() < other.rank();

        for (int i = 0; i < cards.size(); ++i) {
            if (cardValues[cards[i]] != cardValues[other.cards[i]])
                return cardValues[cards[i]] < cardValues[other.cards[i]];
        }
        return false;
    }
};

QVector<Hand> parse(const QString &fileName, bool joker = false)
{
    QVector<Hand> result;
    QFile         file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileName;
        return {};
    }
    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        if (joker)
            line.replace('J', 'X');
        result.append(Hand::fromString(line, joker));
    }
    return result;
}

QString part1(const QString &fileName)
{
    auto hands = parse(fileName);
    std::sort(hands.begin(), hands.end());
    quint64 rank = 1;
    return QString::number(std::accumulate(hands.begin(), hands.end(), 0_u64, [&rank](quint64 sum, const auto &hand) {
        return sum + rank++ * hand.bid;
    }));
}

QString part2(const QString &fileName)
{
    auto hands = parse(fileName, true);
    std::sort(hands.begin(), hands.end());
    quint64 rank = 1;
    return QString::number(std::accumulate(hands.begin(), hands.end(), 0_u64, [&rank](quint64 sum, const auto &hand) {
        return sum + rank++ * hand.bid;
    }));
}

} // namespace day07

int main()
{
    qDebug() << "Part 1: " << day07::part1(":/input.txt");
    qDebug() << "Part 2: " << day07::part2(":/input.txt");
}

#include "day07.moc"