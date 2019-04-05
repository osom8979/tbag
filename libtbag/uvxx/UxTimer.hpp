/**
 * @file   UxTimer.hpp
 * @brief  UxTimer class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTIMER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTIMER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalTimer.hpp>

#include <cstdint>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxTimer class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxTimer : public libtbag::uvxx::UxHandle
{
public:
    using FuncTimer   = libtbag::uvpp::func::FuncTimer;
    using WeakTimer   = std::weak_ptr<FuncTimer>;
    using SharedTimer = std::shared_ptr<FuncTimer>;

public:
    UxTimer();
    UxTimer(UxLoop & loop);
    UxTimer(UxTimer const & obj) TBAG_NOEXCEPT;
    UxTimer(UxTimer && obj) TBAG_NOEXCEPT;
    ~UxTimer();

public:
    UxTimer & operator =(UxTimer const & obj) TBAG_NOEXCEPT;
    UxTimer & operator =(UxTimer && obj) TBAG_NOEXCEPT;

public:
    void copy(UxTimer const & obj) TBAG_NOEXCEPT;
    void swap(UxTimer & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxTimer & lh, UxTimer & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedTimer lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncTimer>(_handle.lock()); }

public:
    // clang-format off
    void setOnClose(FuncTimer::OnClose const & cb) { lock()->close_cb = cb; }
    void setOnWalk (FuncTimer::OnWalk  const & cb) { lock()->walk_cb  = cb; }
    void setOnTimer(FuncTimer::OnTimer const & cb) { lock()->timer_cb = cb; }
    // clang-format on

public:
    Err init(UxLoop & loop);

public:
    Err start(uint64_t timeout, uint64_t repeat = 0);
    Err stop();
    Err again();

public:
    void setRepeat(uint64_t repeat);
    uint64_t getRepeat();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTIMER_HPP__

