#include "literals.h"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QRegularExpression>
#include <QString>
#include <QVector>

using namespace utils::literals::integer;

namespace day08 {

enum class Instruction { Left, Right };

struct Element
{
    QString                    name;
    QMap<Instruction, QString> next;
};

struct Program
{
    QVector<Instruction>    instructions;
    QHash<QString, Element> elements;
    int                     currentInstruction = 0;

    Instruction update()
    {
        auto ret           = instructions[currentInstruction];
        currentInstruction = (currentInstruction + 1) % instructions.size();
        return ret;
    }

    static Program parse(const QString &fileName)
    {
        QFile file(fileName);
        Q_ASSERT(file.open(QIODevice::ReadOnly | QIODevice::Text));
        Program ret;

        enum class State { Instruction, Elements };
        State state = State::Instruction;

        while (!file.atEnd()) {
            const QString line = file.readLine().trimmed();
            if (line.isEmpty()) {
                state = State::Elements;
                continue;
            }

            if (state == State::Instruction) {
                for (const auto &c : line) {
                    if (c == 'L')
                        ret.instructions.append(Instruction::Left);
                    else if (c == 'R')
                        ret.instructions.append(Instruction::Right);
                    else
                        Q_ASSERT(false);
                }
            } else {
                QRegularExpression re(R"((\w+)\s*=\s*\((\w+),\s*(\w+)\))");
                auto               m = re.match(line);
                ret.elements.insert(m.captured(1),
                                    {
                                        m.captured(1),
                                        {{Instruction::Left, m.captured(2)}, {Instruction::Right, m.captured(3)}}
                });
            }
        }
        return ret;
    }
};

QVector<Element> elementsEndingWith(QChar c, const Program &program)
{
    QVector<Element> ret;
    for (const auto &e : program.elements) {
        if (e.name.endsWith(c))
            ret.append(e);
    }
    return ret;
}

QString part1(const QString &fileName)
{
    auto program        = Program::parse(fileName);
    auto currentElement = program.elements["AAA"];
    int  steps          = 0;

    while (currentElement.name != "ZZZ") {
        auto instruction = program.update();
        currentElement   = program.elements[currentElement.next[instruction]];
        steps++;
    }

    return QString::number(steps);
}

QString part2(const QString &fileName)
{
    auto             program         = Program::parse(fileName);
    quint64          steps           = 0;
    auto             currentElements = elementsEndingWith('A', program);
    QVector<quint64> counts(currentElements.size());

    while (std::ranges::any_of(counts, [](const auto &c) { return c == 0; })) {
        auto instruction = program.update();
        for (int i = 0; i < currentElements.size(); ++i) {
            if (currentElements[i].name.endsWith('Z') && counts[i] == 0) {
                counts[i] = steps;
                continue;
            }
            currentElements[i] = program.elements[currentElements[i].next[instruction]];
        }
        steps++;
    }

    return QString::number(std::accumulate(counts.begin(), counts.end(), 1_u64, [](quint64 sum, const auto &c) {
        return std::lcm(sum, c);
    }));
}

} // namespace day08

int main()
{
    qDebug() << "Part 1: " << day08::part1(":/input.txt");
    qDebug() << "Part 2: " << day08::part2(":/input.txt");
}
