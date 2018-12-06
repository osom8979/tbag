/**
 * @file   UxAsync.hpp
 * @brief  UxAsync class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXASYNC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXASYNC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalAsync.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxAsync class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxAsync : public libtbag::uvxx::UxHandle
{
public:
    using FuncAsync   = libtbag::uvpp::func::FuncAsync;
    using WeakAsync   = std::weak_ptr<FuncAsync>;
    using SharedAsync = std::shared_ptr<FuncAsync>;

public:
    UxAsync();
    UxAsync(UxLoop & loop);
    UxAsync(UxAsync const & obj) TBAG_NOEXCEPT;
    UxAsync(UxAsync && obj) TBAG_NOEXCEPT;
    ~UxAsync();

public:
    UxAsync & operator =(UxAsync const & obj) TBAG_NOEXCEPT;
    UxAsync & operator =(UxAsync && obj) TBAG_NOEXCEPT;

public:
    void copy(UxAsync const & obj) TBAG_NOEXCEPT;
    void swap(UxAsync & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxAsync & lh, UxAsync & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedAsync lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncAsync>(_handle.lock()); }

public:
    // @formatter:off
    void setOnClose(FuncAsync::OnClose const & cb) { lock()->close_cb = cb; }
    void setOnWalk (FuncAsync::OnWalk  const & cb) { lock()->walk_cb  = cb; }
    void setOnAsync(FuncAsync::OnAsync const & cb) { lock()->async_cb = cb; }
    // @formatter:on

public:
    Err init(UxLoop & loop);
    Err send();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXASYNC_HPP__

