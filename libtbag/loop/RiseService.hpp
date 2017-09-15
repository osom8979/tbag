/**
 * @file   RiseService.hpp
 * @brief  RiseService class prototype.
 * @author zer0
 * @date   2017-07-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_RISESERVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_RISESERVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdlib>
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include <set>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

/**
 * RiseComponent class prototype.
 *
 * @author zer0
 * @date   2017-07-12
 */
class TBAG_API RiseComponent : private Noncopyable
{
public:
    enum class ComState
    {
        CS_READY,
        CS_RUNNING,
        CS_THREAD_END_FAILURE,
        CS_EXCEPTION,
        CS_END_SUCCESS,
        CS_END_FAILURE,
    };

    using Mutex  = std::mutex;
    using Guard  = std::lock_guard<Mutex>;
    using Thread = std::thread;

public:
    TBAG_CONSTEXPR static int const NEVER_DIE_TRY_COUNT = -1;

private:
    ComState _state;
    int _rise_try_count;
    int _try_count;

private:
    Mutex mutable _mutex;
    Mutex mutable _thread_mutex;
    Thread _thread;

public:
    RiseComponent();
    virtual ~RiseComponent();

private:
    bool isReadyNoSafe() const TBAG_NOEXCEPT;
    bool isEndNoSafe() const TBAG_NOEXCEPT;
    bool isRunningNoSafe() const TBAG_NOEXCEPT;
    bool isThreadEndFailureNoSafe() const TBAG_NOEXCEPT;
    bool isRiseIfFailureNoSafe() const TBAG_NOEXCEPT;
    bool isNeverDieNoSafe() const TBAG_NOEXCEPT;
    bool isEndOfTryNoSafe() const TBAG_NOEXCEPT;
    bool runThreadNoSafe();

public:
    void setRiseIfFailure(int try_count = 0);
    void setNeverDie();
    void clearTryCount();

public:
    ComState getState() const TBAG_NOEXCEPT;
    int getTryCount() const TBAG_NOEXCEPT;
    bool isEnd() const TBAG_NOEXCEPT;

public:
    void joinIfJoinable();
    void autoRun();

protected:
    virtual bool run() = 0;
};

/**
 * RiseService class prototype.
 *
 * @author zer0
 * @date   2017-07-12
 */
class TBAG_API RiseService : private Noncopyable
{
public:
    using SharedCom    = std::shared_ptr<RiseComponent>;
    using SharedComSet = std::set<SharedCom>;

public:
    TBAG_CONSTEXPR static uint64_t const RISE_REPEAT_MILLISECONDS = 5 * 1000;
    TBAG_CONSTEXPR static bool const TERMINATE_LOOP_FLAG = true;

public:
    uint64_t const REPEAT;
    bool const VERBOSE;

public:
    RiseService(uint64_t repeat = RISE_REPEAT_MILLISECONDS, bool verbose = false);
    virtual ~RiseService();

public:
    inline uint64_t getRepeat() const TBAG_NOEXCEPT { return REPEAT; }
    inline bool getVerbose() const TBAG_NOEXCEPT { return VERBOSE; }

public:
    Err run();
    Err run(SharedComSet const & init_coms);

public:
    virtual void onCreate(SharedComSet & coms);
    virtual void onTimer(SharedComSet & coms);
    virtual void onDestroy(SharedComSet & coms);

public:
    /**
     * @return If true, the loop terminates.
     */
    virtual bool onInterrupt(SharedComSet & coms);
};

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_RISESERVICE_HPP__

