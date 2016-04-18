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
    static constexpr char const * const TIMESTAMP_LONG_FORMAT = "%Y-%m-%dT%H:%M:%S";

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
    static constexpr char const * const TIMESTAMP_SHORT_FORMAT = "%Y%m%dT%H%M%S";

    /** Years since 1900 */
    static constexpr int const YEARS_SINCE = 1900;

    /** Months since january: 0-11 */
    static constexpr int const MONTHS_SINCE = 1;

// ctime wrapper.
public:
    /** Obtain current time. */
    static time_t getCurrentTime() noexcept {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

    static tm * getCurrentGmtTime(time_t const & t) noexcept {
        return gmtime(&t);
    }

    static tm * getCurrentLocalTime(time_t const & t) noexcept {
        return localtime(&t);
    }

    static std::string getFormatString(std::string const & format, tm const * t, std::size_t allocate_size = 128) {
        // The expected size of the buffer.
        std::vector<char> buffer;
        buffer.resize(allocate_size, static_cast<typename std::vector<char>::value_type>(0x00));

        std::size_t length = strftime(&buffer[0], allocate_size, format.c_str(), t);
        if (length >= allocate_size) {
            return getFormatString(format, t, allocate_size * 2);
        }
        return std::string(buffer.begin(), buffer.begin() + length);
    }

public:
    template <typename TimeUnit = std::chrono::milliseconds>
    static TimeUnit getElapsedTime(std::chrono::system_clock::time_point const & time) {
        return std::chrono::duration_cast<TimeUnit>(std::chrono::system_clock::now() - time);
    }

    /** millisecond part of the second 0-999. */
    static int getMillisec(std::chrono::system_clock::time_point const & time) {
        std::chrono::system_clock::duration epoch = time.time_since_epoch();
        epoch -= std::chrono::duration_cast<std::chrono::seconds>(epoch);

        // It does not work on some platforms:
        // return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();

        return static_cast<int>(epoch / std::chrono::milliseconds(1));
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

