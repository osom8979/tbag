/**
 * @file   DurationStringParser.hpp
 * @brief  DurationStringParser class prototype.
 * @author zer0
 * @date   2020-05-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRINGPARSER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRINGPARSER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/Unit.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/**
 * DurationStringParser class prototype.
 *
 * @author zer0
 * @date   2020-05-16
 */
template <typename DurationT>
struct DurationStringParser : private Noncopyable
{
    using nanoseconds = std::chrono::nanoseconds;
    using microseconds = std::chrono::microseconds;
    using milliseconds = std::chrono::milliseconds;
    using seconds = std::chrono::seconds;
    using minutes = std::chrono::minutes;
    using hours = std::chrono::hours;
    using ErrSize = libtbag::ErrPair<std::size_t>;

    template <typename T>
    static std::size_t size(T const & duration) TBAG_NOEXCEPT
    {
        return std::chrono::duration_cast<DurationT>(duration).count();
    }

    // clang-format off
    static ErrSize nano (std::size_t const & v) TBAG_NOEXCEPT { return {E_SUCCESS, size(nanoseconds(v))}; }
    static ErrSize micro(std::size_t const & v) TBAG_NOEXCEPT { return {E_SUCCESS, size(microseconds(v))}; }
    static ErrSize milli(std::size_t const & v) TBAG_NOEXCEPT { return {E_SUCCESS, size(milliseconds(v))}; }
    static ErrSize sec  (std::size_t const & v) TBAG_NOEXCEPT { return {E_SUCCESS, size(seconds(v))}; }
    static ErrSize min  (std::size_t const & v) TBAG_NOEXCEPT { return {E_SUCCESS, size(minutes(v))}; }
    static ErrSize hour (std::size_t const & v) TBAG_NOEXCEPT { return {E_SUCCESS, size(hours(v))}; }
    static ErrSize day  (std::size_t const & v) TBAG_NOEXCEPT { return {E_SUCCESS, size(hours(v*DAY_TO_HOURS))}; }
    // clang-format on

    static ErrSize parse1(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 1);
        // clang-format off
        switch (suffix[0]) {
        case 'n': return nano(value);
        case 'u': return micro(value);
        case 's': return sec(value);
        case 'm': return min(value);
        case 'h': return hour(value);
        case 'd': return day(value);
        default:  return E_PARSING;
        }
        // clang-format on
    }

    static ErrSize parse2(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 2);
        // All three letters end with 's'. ('ns', 'us', 'ms')
        if (suffix[1] == 's') {
            // clang-format off
            switch (suffix[0]) {
            case 'n': return nano(value);
            case 'u': return micro(value);
            case 'm': return milli(value);
            }
            // clang-format on
        }
        return E_PARSING;
    }

    static ErrSize parse3(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 3);
        if (TBAG_STRCMP3(suffix, "sec")) {
            return sec(value);
        }
        if (TBAG_STRCMP3(suffix, "min")) {
            return min(value);
        }
        if (TBAG_STRCMP3(suffix, "day")) {
            return min(value);
        }
        return E_PARSING;
    }

    static ErrSize parse4(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 4);
        if (TBAG_STRCMP4(suffix, "nano")) {
            return nano(value);
        }
        if (TBAG_STRCMP4(suffix, "mill")) {
            return milli(value);
        }
        if (TBAG_STRCMP4(suffix, "hour")) {
            return hour(value);
        }
        if (TBAG_STRCMP4(suffix, "days")) {
            return day(value);
        }
        return E_PARSING;
    }

    static ErrSize parse5(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 5);
        if (TBAG_STRCMP5(suffix, "micro")) {
            return micro(value);
        }
        if (TBAG_STRCMP5(suffix, "milli")) {
            return milli(value);
        }
        if (TBAG_STRCMP5(suffix, "hours")) {
            return hour(value);
        }
        return E_PARSING;
    }

    static ErrSize parse6(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 6);
        if (TBAG_STRCMP6(suffix, "second")) {
            return sec(value);
        }
        if (TBAG_STRCMP6(suffix, "minute")) {
            return min(value);
        }
        return E_PARSING;
    }

    static ErrSize parse7(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 7);
        if (TBAG_STRCMP7(suffix, "nanosec")) {
            return nano(value);
        }
        if (TBAG_STRCMP7(suffix, "seconds")) {
            return sec(value);
        }
        if (TBAG_STRCMP7(suffix, "minutes")) {
            return min(value);
        }
        return E_PARSING;
    }

    static ErrSize parse8(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 8);
        if (TBAG_STRCMP8(suffix, "microsec")) {
            return micro(value);
        }
        if (TBAG_STRCMP8(suffix, "millisec")) {
            return milli(value);
        }
        return E_PARSING;
    }

    static ErrSize parse9(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 9);
        return E_PARSING;
    }

    static ErrSize parse10(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 10);
        if (TBAG_STRCMP10(suffix, "nanosecond")) {
            return nano(value);
        }
        return E_PARSING;
    }

    static ErrSize parse11(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 11);
        if (TBAG_STRCMP11(suffix, "nanoseconds")) {
            return nano(value);
        }
        if (TBAG_STRCMP11(suffix, "microsecond")) {
            return micro(value);
        }
        if (TBAG_STRCMP11(suffix, "millisecond")) {
            return milli(value);
        }
        return E_PARSING;
    }

    static ErrSize parse12(std::size_t value, std::string const & suffix)
    {
        assert(suffix.size() == 12);
        if (TBAG_STRCMP12(suffix, "microseconds")) {
            return micro(value);
        }
        if (TBAG_STRCMP12(suffix, "milliseconds")) {
            return milli(value);
        }
        return E_PARSING;
    }

    static ErrSize parse(std::string const & str)
    {
        std::string number;
        std::string unit;
        auto const split_code = libtbag::string::splitNumberAndUnit(str, number, unit);
        if (isFailure(split_code)) {
            return split_code;
        }

        auto const value = libtbag::string::toValue<std::size_t>(number);
        auto const unit_size = unit.size();

        // clang-format off
        switch (unit_size) {
        case  0: return { E_SUCCESS, value };
        case  1: return parse1(value, unit);
        case  2: return parse2(value, unit);
        case  3: return parse3(value, unit);
        case  4: return parse4(value, unit);
        case  5: return parse5(value, unit);
        case  6: return parse6(value, unit);
        case  7: return parse7(value, unit);
        case  8: return parse8(value, unit);
        case  9: return parse9(value, unit);
        case 10: return parse10(value, unit);
        case 11: return parse11(value, unit);
        case 12: return parse12(value, unit);
        default: return E_PARSING;
        }
        // clang-format on
    }
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRINGPARSER_HPP__

