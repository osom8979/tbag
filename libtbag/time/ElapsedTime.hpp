/**
 * @file   ElapsedTime.hpp
 * @brief  ElapsedTime class prototype.
 * @author zer0
 * @date   2016-09-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_ELAPSEDTIME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_ELAPSEDTIME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/time/Time.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

template <typename Duration, typename TimePoint = std::chrono::system_clock::time_point>
inline Duration getDuration(TimePoint const & time)
{
    using namespace std::chrono;
    return duration_cast<Duration>(system_clock::now() - time);
}

template <typename Duration, typename Predicated>
inline Duration getDurationWithPredicated(Predicated predicated)
{
    using SystemClock = std::chrono::system_clock;
    using TimePoint   = typename SystemClock::time_point;

    auto start = SystemClock::now();
    predicated();
    return getDuration<Duration, TimePoint>(start);
}

/**
 * ElapsedTime class prototype.
 *
 * @remarks
 *  Example code:
 *  @code{.cpp}
 *   time::ElapsedTime<std::ostream> elapsed(std::cout);
 *  @endcode
 *
 * @author zer0
 * @date   2015-09-17
 * @date   2016-09-21 (Rename: Duration -> ElapsedTime)
 */
template <typename OutputStream
        , typename Duration = std::chrono::milliseconds>
class ElapsedTime : public Noncopyable
{
public:
    using SystemClock = std::chrono::system_clock;
    using TimePoint   = typename SystemClock::time_point;

private:
    TimePoint      _start;
    OutputStream & _ostream;

public:
    ElapsedTime(OutputStream & ostream) : _ostream(ostream)
    {
        _start = SystemClock::now();
    }

    ~ElapsedTime()
    {
        _ostream << getDuration<Duration, TimePoint>(_start).count();
    }
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_ELAPSEDTIME_HPP__

