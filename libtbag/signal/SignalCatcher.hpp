/**
 * @file   SignalCatcher.hpp
 * @brief  SignalCatcher class prototype.
 * @author zer0
 * @date   2019-06-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALCATCHER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALCATCHER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal {

/**
 * SignalCatcher class prototype.
 *
 * @author zer0
 * @date   2019-06-26
 */
class TBAG_API SignalCatcher : private Noncopyable
{
public:
    struct Impl;

public:
    using Uniquempl = std::unique_ptr<Impl>;
    using Callback = std::function<void(void)>;

private:
    Uniquempl _impl;

public:
    SignalCatcher();
    SignalCatcher(int signum, Callback const & cb);
    ~SignalCatcher();

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    Err start(int signum, Callback const & cb);
    Err startInterrupt(Callback const & cb);
    Err asyncRaise();
    void done();
};

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SIGNAL_SIGNALCATCHER_HPP__

