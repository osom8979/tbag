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
    using Nanoseconds  = std::chrono::nanoseconds;
    using Microseconds = std::chrono::microseconds;
    using Milliseconds = std::chrono::milliseconds;
    using Seconds      = std::chrono::seconds;
    using Minutes      = std::chrono::minutes;
    using Hours        = std::chrono::hours;

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

    template <typename Rep, typename Period>
    Duration(std::chrono::duration<Rep, Period> const & obj)
            : _nano(std::chrono::duration_cast<decltype(_nano)>(obj))
    { /* EMPTY. */ }

    template <typename BaseDurationT>
    Duration(std::string const & text, BaseDurationT def = {})
    {
        auto const code = fromString<BaseDurationT>(text);
        if (isFailure(code)) {
            _nano = def;
        }
    }

    Duration(Duration const & obj);
    Duration(Duration && obj) TBAG_NOEXCEPT;
    ~Duration();

public:
    static Duration nano (std::size_t val);
    static Duration micro(std::size_t val);
    static Duration milli(std::size_t val);
    static Duration sec  (std::size_t val);
    static Duration min  (std::size_t val);
    static Duration hour (std::size_t val);

    static Duration nano (std::string const & text, Nanoseconds  const & def = {});
    static Duration micro(std::string const & text, Microseconds const & def = {});
    static Duration milli(std::string const & text, Milliseconds const & def = {});
    static Duration sec  (std::string const & text, Seconds      const & def = {});
    static Duration min  (std::string const & text, Minutes      const & def = {});
    static Duration hour (std::string const & text, Hours        const & def = {});

public:
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
    template <typename R, typename P>
    friend bool operator < (std::chrono::duration<R,P> const & lh, Duration const & rh) { return lh <  rh._nano; }
    template <typename R, typename P>
    friend bool operator <=(std::chrono::duration<R,P> const & lh, Duration const & rh) { return lh <= rh._nano; }
    template <typename R, typename P>
    friend bool operator > (std::chrono::duration<R,P> const & lh, Duration const & rh) { return lh >  rh._nano; }
    template <typename R, typename P>
    friend bool operator >=(std::chrono::duration<R,P> const & lh, Duration const & rh) { return lh >= rh._nano; }
    template <typename R, typename P>
    friend bool operator ==(std::chrono::duration<R,P> const & lh, Duration const & rh) { return lh == rh._nano; }
    template <typename R, typename P>
    friend bool operator !=(std::chrono::duration<R,P> const & lh, Duration const & rh) { return lh != rh._nano; }

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
    std::string toString() const;

public:
    Err fromNanosecondsText (std::string const & text);
    Err fromMicrosecondsText(std::string const & text);
    Err fromMillisecondsText(std::string const & text);
    Err fromSecondsText     (std::string const & text);
    Err fromMinutesText     (std::string const & text);
    Err fromHoursText       (std::string const & text);

    Err fromString(Nanoseconds , std::string const & text) { return fromNanosecondsText (text); }
    Err fromString(Microseconds, std::string const & text) { return fromMicrosecondsText(text); }
    Err fromString(Milliseconds, std::string const & text) { return fromMillisecondsText(text); }
    Err fromString(Seconds     , std::string const & text) { return fromSecondsText     (text); }
    Err fromString(Minutes     , std::string const & text) { return fromMinutesText     (text); }
    Err fromString(Hours       , std::string const & text) { return fromHoursText       (text); }

    template <typename BaseT>
    Err fromString(std::string const & text)
    {
        return fromString(BaseT{}, text);
    }

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

