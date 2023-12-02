#include <QDebug>
#include <QFile>
#include <QMetaEnum>
#include <QString>
#include <QVariant>

namespace Day02 {

Q_NAMESPACE

enum class Color { red, green, blue };
Q_ENUM_NS(Color)

QMap<Color, int> maxScoresFromGame(const QString &game)
{
    QMap<Color, int> scores;
    for (const auto &set : game.split(";")) {
        for (const auto &element : set.split(",")) {
            const auto sep   = element.trimmed().split(" ");
            const auto score = sep[0].toInt();
            const auto color = QVariant::fromValue(sep[1]).value<Color>();
            scores[color]    = qMax(scores[color], score);
        }
    }
    return scores;
}

template<typename Fn>
QString process(Fn function, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileName;
        return {};
    }

    quint32 sum = 0;

    while (!file.atEnd()) {
        const QString line         = file.readLine();
        const auto    posOfGameSep = line.indexOf(": ");
        const auto    gameId       = line.left(posOfGameSep).replace("Game ", "").toInt();
        const auto    game         = line.mid(posOfGameSep + 2);
        sum += function(gameId, game);
    }

    return QString::number(sum);
}

QString part1(const QString &fileName)
{
    const QMap<Color, int> maxScores{
        {Color::red,   12},
        {Color::green, 13},
        {Color::blue,  14}
    };

    return process(
        [&maxScores](quint32 gameId, const QString &game) {
            if (const auto scores = maxScoresFromGame(game);
                scores[Color::red] <= maxScores[Color::red] && scores[Color::green] <= maxScores[Color::green]
                && scores[Color::blue] <= maxScores[Color::blue]) {
                return gameId;
            }
            return static_cast<quint32>(0);
        },
        fileName);
}

QString part2(const QString &fileName)
{
    return process(
        [](quint32, const QString &game) {
            const auto scores = maxScoresFromGame(game);
            return scores[Color::red] * scores[Color::green] * scores[Color::blue];
        },
        fileName);
}

} // namespace Day02

int main()
{
    qInfo() << "Part 1:" << Day02::part1(":/input.txt");
    qInfo() << "Part 2:" << Day02::part2(":/input.txt");
    return 0;
}

#include "day02.moc"
