/**
 * @file   Profile.hpp
 * @brief  Profile class prototype.
 * @author zer0
 * @date   2016-09-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_PROFILE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_PROFILE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <functional>
#include <chrono>
#include <atomic>
#include <memory>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

/**
 * Profile class prototype.
 *
 * @author zer0
 * @date   2016-09-21
 */
class TBAG_API Profile : public Noncopyable
{
public:
    using TimePoint = std::chrono::system_clock::time_point;
    using Duration  = TimePoint::duration;
    using Rep       = TimePoint::rep;
    using Period    = TimePoint::period;

    using RepeatCallback = std::function<void(Rep average)>;

public:
    class TBAG_API Timer : public Noncopyable
    {
    public:
        friend class Profile;

    private:
        Profile & _profile;
        TimePoint _start;

    public:
        Timer(Profile & profile);
        ~Timer();
    };

public:
    using SharedTimer = std::shared_ptr<Timer>;

private:
    std::size_t _cycle_count;
    std::size_t _current_cycle;

private:
    Rep _total_duration;
    RepeatCallback _callback;

private:
    std::atomic_bool _enable;

public:
    Profile();
    Profile(std::size_t cycle);
    Profile(std::size_t cycle, RepeatCallback const & callback);
    Profile(std::size_t cycle, RepeatCallback const & callback, bool enable);
    ~Profile();

public:
    inline bool isEnable() const TBAG_NOEXCEPT
    { return _enable; }
    inline void setEnable(bool enable = true) TBAG_NOEXCEPT
    { _enable = enable; }

    inline std::size_t getCycleCount() const TBAG_NOEXCEPT
    { return _cycle_count; }
    inline void setCycleCount(std::size_t cycle) TBAG_NOEXCEPT
    { _cycle_count = cycle; }

    inline void setCallback(RepeatCallback const & callback) TBAG_NOEXCEPT
    { _callback = callback; }

private:
    void update(Duration duration);

public:
    SharedTimer tick();

public:
    static char const * const getTimeUnitString() TBAG_NOEXCEPT;
};

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_PROFILE_HPP__

