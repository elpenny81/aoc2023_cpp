#pragma once
#include <QtGlobal>
#include <cstddef>

namespace utils::literals {

    // Integer types
    inline namespace integer {
        constexpr qint8 operator "" _i8(unsigned long long int value) {
            return static_cast<qint8>(value);
        }

        constexpr qint16 operator "" _i16(unsigned long long int value) {
            return static_cast<qint16>(value);
        }

        constexpr qint32 operator "" _i32(unsigned long long int value) {
            return static_cast<qint32>(value);
        }

        constexpr qint64 operator "" _i64(unsigned long long int value) {
            return static_cast<qint64>(value);
        }

        constexpr quint8 operator "" _u8(unsigned long long int value) {
            return static_cast<quint8>(value);
        }

        constexpr quint16 operator "" _u16(unsigned long long int value) {
            return static_cast<quint16>(value);
        }

        constexpr quint32 operator "" _u32(unsigned long long int value) {
            return static_cast<quint32>(value);
        }

        constexpr quint64 operator "" _u64(unsigned long long int value) {
            return static_cast<quint64>(value);
        }
    }

    // Floating-point types
    inline namespace floating_point {
        constexpr float operator "" _f(long double value) {
            return static_cast<float>(value);
        }

        constexpr double operator "" _d(long double value) {
            return static_cast<double>(value);
        }

        constexpr long double operator "" _ld(long double value) {
            return value;
        }
    }

}