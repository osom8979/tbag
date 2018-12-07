/**
 * @file   UxLoop.hpp
 * @brief  UxLoop class prototype.
 * @author zer0
 * @date   2018-12-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

/**
 * UxLoop class prototype.
 *
 * @author zer0
 * @date   2018-12-05
 */
class TBAG_API UxLoop
{
public:
    using Loop       = libtbag::uvpp::Loop;
    using RunMode    = Loop::RunMode;
    using ThreadId   = Loop::ThreadId;
    using SharedLoop = std::shared_ptr<Loop>;

private:
    SharedLoop _loop;

public:
    UxLoop(bool print_internal = false, bool verbose = false);
    UxLoop(std::nullptr_t) TBAG_NOEXCEPT;
    UxLoop(UxLoop const & obj) TBAG_NOEXCEPT;
    UxLoop(UxLoop && obj) TBAG_NOEXCEPT;
    ~UxLoop();

public:
    UxLoop & operator =(UxLoop const & obj) TBAG_NOEXCEPT;
    UxLoop & operator =(UxLoop && obj) TBAG_NOEXCEPT;

public:
    void copy(UxLoop const & obj) TBAG_NOEXCEPT;
    void swap(UxLoop & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxLoop & lh, UxLoop & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_loop); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Loop       * get()       TBAG_NOEXCEPT { return _loop.get(); }
    inline Loop const * get() const TBAG_NOEXCEPT { return _loop.get(); }

public:
    inline Loop       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline Loop const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline Loop       & operator *()       TBAG_NOEXCEPT { return *get(); }
    inline Loop const & operator *() const TBAG_NOEXCEPT { return *get(); }

public:
    Err release();

public:
    ThreadId getOwnerThreadId() const;
    bool isRunning() const;

public:
    bool isAlive() const;

public:
    std::size_t size() const;
    bool empty() const;

public:
    Err run(RunMode mode = RunMode::RUN_DEFAULT);
    Err close();
    void walk(void * arg = nullptr);

public:
    void stop();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXLOOP_HPP__

