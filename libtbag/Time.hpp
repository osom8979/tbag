/**
 * @file   Time.hpp
 * @brief  Time class prototype.
 * @author zer0
 * @date   2016-04-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Strings.hpp>

#include <ctime>

#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * Long date/time format.
 *
 * @warning
 *  Not ISO/IEC 8601:2004
 *
 * @remarks
 *  @code
 *   YYYY-MM-DDThh:mm:ss
 *  @endcode
 */
constexpr char    const * const TIMESTAMP_LONG_FORMAT      =  "%Y-%m-%dT%H:%M:%S";
constexpr wchar_t const * const WIDE_TIMESTAMP_LONG_FORMAT = L"%Y-%m-%dT%H:%M:%S";

template <typename CharType = char>
constexpr CharType const * const getDefaultTimestampLongFormat() noexcept;

template <>
constexpr char const * const getDefaultTimestampLongFormat<char>() noexcept
{
    return TIMESTAMP_LONG_FORMAT;
}

template <>
constexpr wchar_t const * const getDefaultTimestampLongFormat<wchar_t>() noexcept
{
    return WIDE_TIMESTAMP_LONG_FORMAT;
}

/**
 * Short date/time format.
 *
 * @warning
 *  Not ISO/IEC 8601:2004
 *
 * @remarks
 *  @code
 *   YYYYMMDDThhmmss
 *  @endcode
 */
constexpr char    const * const TIMESTAMP_SHORT_FORMAT      =  "%Y%m%dT%H%M%S";
constexpr wchar_t const * const WIDE_TIMESTAMP_SHORT_FORMAT = L"%Y%m%dT%H%M%S";

template <typename CharType>
constexpr CharType const * const getDefaultTimestampShortFormat() noexcept;

template <>
constexpr char const * const getDefaultTimestampShortFormat<char>() noexcept
{
    return TIMESTAMP_SHORT_FORMAT;
}

template <>
constexpr wchar_t const * const getDefaultTimestampShortFormat<wchar_t>() noexcept
{
    return WIDE_TIMESTAMP_SHORT_FORMAT;
}

/** Years since 1900 */
constexpr int const YEARS_SINCE = 1900;

/** Months since january: 0-11 */
constexpr int const MONTHS_SINCE = 1;

/** millisecond part of the second 0-999. */
int getMillisec(std::chrono::system_clock::time_point const & time);

std::string  getMillisecMbs(std::chrono::system_clock::time_point const & time);
std::wstring getMillisecWcs(std::chrono::system_clock::time_point const & time);

template <typename CharType>
void getMillisecString(std::chrono::system_clock::time_point const & time, std::basic_string<CharType> & result);

template <>
inline void getMillisecString<char>(std::chrono::system_clock::time_point const & time, std::basic_string<char> & result)
{
    result = getMillisecMbs(time);
}

template <>
inline void getMillisecString<wchar_t>(std::chrono::system_clock::time_point const & time, std::basic_string<wchar_t> & result)
{
    result = getMillisecWcs(time);
}

/**
 * Time class prototype.
 *
 * @author zer0
 * @date   2016-04-18
 */
class Time : public Noncopyable
{
public:
    constexpr Time() noexcept = default;
    ~Time() noexcept = default;

// ctime wrapper.
public:
    static std::chrono::system_clock::time_point getNowSystemClock() noexcept {
        return std::chrono::system_clock::now();
    }

    static time_t getTime(std::chrono::system_clock::time_point const & time_point) noexcept {
        return std::chrono::system_clock::to_time_t(time_point);
    }

    /** Obtain current time. */
    static time_t getCurrentTime() noexcept {
        return getTime(getNowSystemClock());
    }

    static tm * getGmtTime(time_t const & t) noexcept {
        return gmtime(&t);
    }

    static tm * getLocalTime(time_t const & t) noexcept {
        return localtime(&t);
    }

    static std::string getFormatString(std::string const & format, tm const * t, std::size_t allocate_size = 128) {
        // The expected size of the buffer.
        std::vector<char> buffer;
        buffer.resize(allocate_size, static_cast<typename std::vector<char>::value_type>(0x00));

        std::size_t length = std::strftime(&buffer[0], allocate_size, format.c_str(), t);
        if (length >= allocate_size) {
            return getFormatString(format, t, allocate_size * 2);
        }
        return std::string(buffer.begin(), buffer.begin() + length);
    }

    static std::wstring getFormatString(std::wstring const & format, tm const * t, std::size_t allocate_size = 128) {
        // The expected size of the buffer.
        std::vector<wchar_t> buffer;
        buffer.resize(allocate_size, static_cast<typename std::vector<wchar_t>::value_type>(0x00));

        std::size_t length = std::wcsftime(&buffer[0], allocate_size, format.c_str(), t);
        if (length >= allocate_size) {
            return getFormatString(format, t, allocate_size * 2);
        }
        return std::wstring(buffer.begin(), buffer.begin() + length);
    }

public:
    template <typename TimeUnit = std::chrono::milliseconds>
    static TimeUnit getElapsedTime(std::chrono::system_clock::time_point const & time) {
        return std::chrono::duration_cast<TimeUnit>(std::chrono::system_clock::now() - time);
    }
};

/**
 * Duration class prototype.
 *
 * @remarks
 *  Example code:
 *  @code{.cpp}
 *  performance::Duration<std::ostream> duration2(std::cout);
 *  @endcode
 *
 * @author zer0
 * @date   2015-09-17
 */
template <typename OutputStream, typename Unit = std::chrono::milliseconds>
class Duration final : public Noncopyable
{
public:
    using time_point = std::chrono::system_clock::time_point;

private:
    time_point _start;
    OutputStream & _ostream;

public:
    Duration(OutputStream & ostream) : _ostream(ostream) {
        this->_start = std::chrono::system_clock::now();
    }

    ~Duration() {
        using namespace std::chrono;
        this->_ostream << duration_cast<Unit>(system_clock::now() - this->_start).count();
    }
};

template <typename Unit = std::chrono::milliseconds>
typename Unit::rep getDuration(std::function<void(void)> const & runner)
{
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    runner();
    return std::chrono::duration_cast<Unit>(std::chrono::system_clock::now() - start).count();
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_HPP__

