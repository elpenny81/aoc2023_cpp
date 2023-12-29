#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

namespace day19 {

enum class ResultType {
    None,
    Accepted,
    Rejected,
    Jump,
};

struct Result
{
    ResultType type{ResultType::None};
    QString    target; // target for jump

    static Result parse(const QString &line)
    {
        Result result;
        if (line == "A") {
            result.type = ResultType::Accepted;
        } else if (line == "R") {
            result.type = ResultType::Rejected;
        } else {
            result.type   = ResultType::Jump;
            result.target = line;
        }
        return result;
    }
};

enum class ConditionType {
    None,
    LowerThan,
    GreaterThan,
    True,
};

struct Condition
{
    ConditionType type{ConditionType::None};
    QString       varName;
    qint64        number = 0;
    Result        result;

    static Condition parse(const QString &line)
    {
        Condition condition;
        if (line.contains('<')) {
            auto conRes         = line.split(':');
            condition.type      = ConditionType::LowerThan;
            const auto subParts = conRes.at(0).split('<');
            condition.varName   = subParts[0];
            condition.number    = subParts[1].toLongLong();
            condition.result    = Result::parse(conRes.at(1));
        } else if (line.contains('>')) {
            auto conRes         = line.split(':');
            condition.type      = ConditionType::GreaterThan;
            const auto subParts = conRes.at(0).split('>');
            condition.varName   = subParts[0];
            condition.number    = subParts[1].toLongLong();
            condition.result    = Result::parse(conRes.at(1));
        } else {
            condition.type   = ConditionType::True;
            condition.result = Result::parse(line);
        }
        return condition;
    }

    [[nodiscard]] Result evaluate(const QString &name, const qint64 value) const
    {
        if (type == ConditionType::LowerThan) {
            if (name == this->varName && value < this->number)
                return result;
        } else if (type == ConditionType::GreaterThan) {
            if (name == this->varName && value > this->number)
                return result;
        } else if (type == ConditionType::True) {
            return result;
        }
        return {};
    }
};

struct Rating
{
    QString name;
    qint64  value = 0;

    static Rating parse(const QString &line)
    {
        Rating rating;
        auto   parts = line.split('=');
        rating.name  = parts[0].trimmed();
        rating.value = parts[1].trimmed().toLongLong();
        return rating;
    }
};

struct RatingList
{
    QVector<Rating> ratings;

    static RatingList parse(const QString &line)
    {
        QString input = line;
        input.remove('{');
        input.remove('}');
        RatingList ratingList;
        for (const auto &rating : input.split(','))
            ratingList.ratings.append(Rating::parse(rating.trimmed()));
        return ratingList;
    }

    [[nodiscard]] qint64 sumOfRatings() const
    {
        return std::accumulate(ratings.begin(), ratings.end(), 0, [](const qint64 sum, const Rating &rating) {
            return sum + rating.value;
        });
    }
};

struct Workflow
{
    QString            name;
    QVector<Condition> conditions;
    Condition          def;

    static Workflow parse(const QString &line)
    {
        Workflow workflow;
        auto     parts = line.split("{");
        workflow.name  = parts[0].trimmed();
        for (const auto &condition : parts[1].removeLast().split(','))
            workflow.conditions.append(Condition::parse(condition.trimmed()));
        workflow.def = workflow.conditions.last();
        workflow.conditions.removeLast();
        return workflow;
    }

    [[nodiscard]] Result evaluate(const RatingList &ratings) const
    {
        for (const auto &condition : conditions) {
            for (const auto &rating : ratings.ratings) {
                if (rating.name == condition.varName) {
                    auto result = condition.evaluate(rating.name, rating.value);
                    if (result.type != ResultType::None)
                        return result;
                }
            }
        }
        return def.result;
    }
};

using Workflows = QHash<QString, Workflow>;

struct Process
{
    Workflows           workflows;
    QVector<RatingList> ratings;

    static Process parse(const QString &fileName)
    {
        Process process;
        QFile   file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return process;
        QTextStream in(&file);
        while (!in.atEnd()) {
            const QString line = in.readLine().trimmed();
            if (line.isEmpty())
                continue;
            if (line.startsWith("{")) {
                process.ratings.append(RatingList::parse(line));
            } else {
                auto workflow = Workflow::parse(line);
                process.workflows.insert(workflow.name, workflow);
            }
        }
        return process;
    }

    [[nodiscard]] qint64 evalRating(const RatingList &ratingList) const
    {
        auto workflow = workflows.value("in");
        while (true) {
            auto evalResult = workflow.evaluate(ratingList);
            if (evalResult.type == ResultType::Accepted) {
                return ratingList.sumOfRatings();
                break;
            } else if (evalResult.type == ResultType::Rejected) {
                break;
            } else if (evalResult.type == ResultType::Jump) {
                workflow = workflows.value(evalResult.target);
            }
        }
        return 0;
    }

    [[nodiscard]] qint64 run() const
    {
        return std::accumulate(ratings.begin(), ratings.end(), 0, [this](const qint64 sum, const RatingList &ratingList) {
            return sum + evalRating(ratingList);
        });
    }
};

struct Range
{
    qint64 min = 1;
    qint64 max = 4000;

    [[nodiscard]] qint64 size() const { return max - min + 1; }
};

qint64 count(QHash<QString, Range> ranges, const Result &result, const Process &process)
{
    qint64 total = 0;

    if (result.type == ResultType::Rejected)
        return 0;

    if (result.type == ResultType::Accepted) {
        qint64 product = 1;
        for (const auto &r : ranges)
            product *= r.size();
        return product;
    }

    auto workflow = process.workflows.value(result.target);
    bool found    = false;
    for (auto &rule : workflow.conditions) {
        auto const &[min, max] = ranges[rule.varName];
        Range t, f;
        if (rule.type == ConditionType::LowerThan) {
            t = {min, rule.number - 1};
            f = {rule.number, max};
        } else if (rule.type == ConditionType::GreaterThan) {
            t = {rule.number + 1, max};
            f = {min, rule.number};
        }

        if (t.min <= t.max) {
            auto newRanges          = ranges;
            newRanges[rule.varName] = t;
            total += count(newRanges, rule.result, process);
        }
        if (f.min <= f.max) {
            ranges[rule.varName] = f;
        } else {
            found = true;
            break;
        }
    }

    if (!found)
        total += count(ranges, workflow.def.result, process);

    return total;
}

QString part1(const QString &fileName)
{
    auto process = Process::parse(fileName);
    return QString::number(process.run());
}

QString part2(const QString &fileName)
{
    QHash<QString, Range> ranges{
        {"x", {}},
        {"m", {}},
        {"a", {}},
        {"s", {}},
    };

    auto process = Process::parse(fileName);
    auto result  = count(ranges, Result{.target = "in"}, process);
    return QString::number(result);
}

} // namespace day19

int main()
{
    qDebug() << "Part 1: " << day19::part1(":/input.txt");
    qDebug() << "Part 2: " << day19::part2(":/input.txt");
}
