/**
 * @file   Duration.hpp
 * @brief  Duration class prototype.
 * @author zer0
 * @date   2020-05-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Err.hpp>
#include <libtbag/predef.hpp>

#include <string>
#include <chrono>
#include <utility>
#include <ostream>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/**
 * Duration class prototype.
 *
 * @author zer0
 * @date   2020-05-15
 */
class TBAG_API Duration
{
public:
    using Nanoseconds = std::chrono::nanoseconds;
    using Rep = typename Nanoseconds::rep;

public:
    template <typename T>
    struct is_duration : std::false_type
    { /* EMPTY. */ };
    template <typename R, typename P>
    struct is_duration<std::chrono::duration<R,P> > : std::true_type
    { /* EMPTY. */ };
    template <typename R, typename P>
    struct is_duration<std::chrono::duration<R,P> const> : std::true_type
    { /* EMPTY. */ };
    template <typename R, typename P>
    struct is_duration<std::chrono::duration<R,P> volatile> : std::true_type
    { /* EMPTY. */ };
    template <typename R, typename P>
    struct is_duration<std::chrono::duration<R,P> const volatile> : std::true_type
    { /* EMPTY. */ };

private:
    Nanoseconds _nano;

public:
    Duration();
    Duration(std::size_t nano);
    Duration(std::string const & text);

    template <typename Rep, typename Period>
    Duration(std::chrono::duration<Rep, Period> const & obj)
            : _nano(std::chrono::duration_cast<decltype(_nano)>(obj))
    { /* EMPTY. */ }

    Duration(Duration const & obj);
    Duration(Duration && obj) TBAG_NOEXCEPT;
    ~Duration();

public:
    Duration & operator =(std::size_t value);
    Duration & operator =(std::string const & text);

    template <typename Rep, typename Period>
    Duration & operator =(std::chrono::duration<Rep, Period> const & obj)
    {
        _nano = std::chrono::duration_cast<decltype(_nano)>(obj);
        return *this;
    }

    Duration & operator =(Duration const & obj);
    Duration & operator =(Duration && obj) TBAG_NOEXCEPT;

public:
    void swap(Duration & obj) TBAG_NOEXCEPT;

public:
    friend void swap(Duration & lh, Duration & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    // clang-format off
    explicit operator std::chrono::nanoseconds () const { return std::chrono::duration_cast<std::chrono::nanoseconds >(_nano); }
    explicit operator std::chrono::microseconds() const { return std::chrono::duration_cast<std::chrono::microseconds>(_nano); }
    explicit operator std::chrono::milliseconds() const { return std::chrono::duration_cast<std::chrono::milliseconds>(_nano); }
    explicit operator std::chrono::seconds     () const { return std::chrono::duration_cast<std::chrono::seconds     >(_nano); }
    explicit operator std::chrono::minutes     () const { return std::chrono::duration_cast<std::chrono::minutes     >(_nano); }
    explicit operator std::chrono::hours       () const { return std::chrono::duration_cast<std::chrono::hours       >(_nano); }
    // clang-format on

public:
    bool operator < (Duration const & obj) const { return _nano <  obj._nano; }
    bool operator <=(Duration const & obj) const { return _nano <= obj._nano; }
    bool operator > (Duration const & obj) const { return _nano >  obj._nano; }
    bool operator >=(Duration const & obj) const { return _nano >= obj._nano; }
    bool operator ==(Duration const & obj) const { return _nano == obj._nano; }
    bool operator !=(Duration const & obj) const { return _nano != obj._nano; }

public:
    std::size_t toNanoseconds () const;
    std::size_t toMicroseconds() const;
    std::size_t toMilliseconds() const;
    std::size_t toSeconds     () const;
    std::size_t toMinutes     () const;
    std::size_t toHours       () const;
    std::size_t toDays        () const;

public:
    std::string toNanosecondsText () const;
    std::string toMicrosecondsText() const;
    std::string toMillisecondsText() const;
    std::string toSecondsText     () const;
    std::string toMinutesText     () const;
    std::string toHoursText       () const;
    std::string toDaysText        () const;

public:
    std::string toUpperTimeText() const;

public:
    void fromString(std::string const & text);
    std::string toString() const;

public:
    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator <<(std::basic_ostream<CharT, TraitsT> & os,
                                                            Duration const & duration)
    {
        return os << duration.toString();
    }
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATION_HPP__

