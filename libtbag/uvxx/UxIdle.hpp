/**
 * @file   UxIdle.hpp
 * @brief  UxIdle class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXIDLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXIDLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxIdle class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxIdle : public libtbag::uvxx::UxHandle
{
public:
    using FuncIdle   = libtbag::uvpp::func::FuncIdle;
    using WeakIdle   = std::weak_ptr<FuncIdle>;
    using SharedIdle = std::shared_ptr<FuncIdle>;

public:
    UxIdle();
    UxIdle(UxLoop & loop);
    UxIdle(UxIdle const & obj) TBAG_NOEXCEPT;
    UxIdle(UxIdle && obj) TBAG_NOEXCEPT;
    ~UxIdle();

public:
    UxIdle & operator =(UxIdle const & obj) TBAG_NOEXCEPT;
    UxIdle & operator =(UxIdle && obj) TBAG_NOEXCEPT;

public:
    void copy(UxIdle const & obj) TBAG_NOEXCEPT;
    void swap(UxIdle & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxIdle & lh, UxIdle & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedIdle lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncIdle>(_handle.lock()); }

public:
    // clang-format off
    void setOnClose(FuncIdle::OnClose const & cb) { lock()->close_cb = cb; }
    void setOnWalk (FuncIdle::OnWalk  const & cb) { lock()->walk_cb  = cb; }
    void setOnIdle (FuncIdle::OnIdle  const & cb) { lock()->idle_cb  = cb; }
    // clang-format on

public:
    Err init(UxLoop & loop);

public:
    Err start();
    Err stop();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXIDLE_HPP__

