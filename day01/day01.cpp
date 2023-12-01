#include <QDebug>
#include <QFile>
#include <QMap>
#include <QHash>
#include <QString>

namespace {

QString sum(const QString &fileName, const QHash<QString, QString> &numbersMap) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileName;
        return {};
    }

    quint32 sum = 0;
    while (!file.atEnd()) {
        const QString line = file.readLine();
        QMap<int, QString> searchResults;

        for (QHashIterator i(numbersMap); i.hasNext();) {
            i.next();
            int index = line.indexOf(i.key());
            while (index != -1) {
                searchResults.insert(index, i.key());
                index = line.indexOf(i.key(), index + 1);
            }
        }
        sum += (numbersMap.value(searchResults.first()) + numbersMap.value(searchResults.last())).toUInt();
    }

    return QString::number(sum);
}

QString part1(const QString &inputFile) {
    return sum(inputFile, {
        {"0", "0"},
        {"1", "1"},
        {"2", "2"},
        {"3", "3"},
        {"4", "4"},
        {"5", "5"},
        {"6", "6"},
        {"7", "7"},
        {"8", "8"},
        {"9", "9"},
    });
}

QString part2(const QString &inputFile) {
    return sum(inputFile, {
        {"zero", "0"},
        {"one", "1"},
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"},
        {"0", "0"},
        {"1", "1"},
        {"2", "2"},
        {"3", "3"},
        {"4", "4"},
        {"5", "5"},
        {"6", "6"},
        {"7", "7"},
        {"8", "8"},
        {"9", "9"},
    });
}

}


int main(int argc, char *argv[]) {
    qInfo() << "Part 1:" << part1(":/input.txt");
    qInfo() << "Part 2:" << part2(":/input.txt");
    return 0;
}
