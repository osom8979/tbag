/**
 * @file   UxSignal.hpp
 * @brief  UxSignal class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSIGNAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSIGNAL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalSignal.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxSignal class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxSignal : public libtbag::uvxx::UxHandle
{
public:
    using FuncSignal   = libtbag::uvpp::func::FuncSignal;
    using WeakSignal   = std::weak_ptr<FuncSignal>;
    using SharedSignal = std::shared_ptr<FuncSignal>;

public:
    UxSignal();
    UxSignal(UxLoop & loop);
    UxSignal(UxSignal const & obj) TBAG_NOEXCEPT;
    UxSignal(UxSignal && obj) TBAG_NOEXCEPT;
    ~UxSignal();

public:
    UxSignal & operator =(UxSignal const & obj) TBAG_NOEXCEPT;
    UxSignal & operator =(UxSignal && obj) TBAG_NOEXCEPT;

public:
    void copy(UxSignal const & obj) TBAG_NOEXCEPT;
    void swap(UxSignal & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxSignal & lh, UxSignal & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedSignal lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncSignal>(_handle.lock()); }

public:
    // @formatter:off
    void setOnClose (FuncSignal::OnClose  const & cb) { lock()->close_cb  = cb; }
    void setOnWalk  (FuncSignal::OnWalk   const & cb) { lock()->walk_cb   = cb; }
    void setOnSignal(FuncSignal::OnSignal const & cb) { lock()->signal_cb = cb; }
    // @formatter:on

public:
    Err init(UxLoop & loop);

public:
    int getSignalNumber() const TBAG_NOEXCEPT;

public:
    Err start(int signum);
    Err startOneshot(int signum);
    Err stop();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSIGNAL_HPP__

