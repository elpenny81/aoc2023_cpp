#pragma once

#include <numeric>
#include <utility>
#include <QVector>

namespace utils {

template<typename Iterable>
class enumerate_object
{
private:
    Iterable                        _iter;
    std::size_t                     _size;
    decltype(std::begin(_iter))     _begin;
    const decltype(std::end(_iter)) _end;

public:
    enumerate_object(Iterable iter)
        : _iter(iter)
        , _size(0)
        , _begin(std::begin(iter))
        , _end(std::end(iter))
    {}

    const enumerate_object &begin() const { return *this; }

    const enumerate_object &end() const { return *this; }

    bool operator!=(const enumerate_object &) const { return _begin != _end; }

    void operator++()
    {
        ++_begin;
        ++_size;
    }

    auto operator*() const -> std::pair<std::size_t, decltype(*_begin)> { return {_size, *_begin}; }
};

template<typename Iterable>
auto enumerate(Iterable &&iter) -> enumerate_object<Iterable>
{
    return {std::forward<Iterable>(iter)};
}

template<typename Iterable>
auto sum(Iterable &&iter)
{
    using T = typename std::remove_reference<decltype(*std::begin(iter))>::type;
    return std::accumulate(std::begin(iter), std::end(iter), T{});
}

template<typename T, typename U>
QVector<std::pair<T, U>> zip(const QVector<T> &v1, const QVector<U> &v2)
{
    QVector<std::pair<T, U>> result;
    auto                     size = std::min(v1.size(), v2.size());
    result.reserve(size);
    for (int i = 0; i < size; ++i)
        result.append({v1[i], v2[i]});
    return result;
}

QVector<std::pair<QChar, QChar>> zip(const QString &v1, const QString &v2)
{
    QVector<std::pair<QChar, QChar>> result;
    auto                             size = std::min(v1.size(), v2.size());
    result.reserve(size);
    for (int i = 0; i < size; ++i)
        result.append({v1[i], v2[i]});
    return result;
}

} // namespace utils
