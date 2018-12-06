/**
 * @file   UxPrepare.hpp
 * @brief  UxPrepare class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPREPARE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPREPARE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalPrepare.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxPrepare class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxPrepare : public libtbag::uvxx::UxHandle
{
public:
    using FuncPrepare   = libtbag::uvpp::func::FuncPrepare;
    using WeakPrepare   = std::weak_ptr<FuncPrepare>;
    using SharedPrepare = std::shared_ptr<FuncPrepare>;

public:
    UxPrepare();
    UxPrepare(UxLoop & loop);
    UxPrepare(UxPrepare const & obj) TBAG_NOEXCEPT;
    UxPrepare(UxPrepare && obj) TBAG_NOEXCEPT;
    ~UxPrepare();

public:
    UxPrepare & operator =(UxPrepare const & obj) TBAG_NOEXCEPT;
    UxPrepare & operator =(UxPrepare && obj) TBAG_NOEXCEPT;

public:
    void copy(UxPrepare const & obj) TBAG_NOEXCEPT;
    void swap(UxPrepare & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxPrepare & lh, UxPrepare & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedPrepare lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncPrepare>(_handle.lock()); }

public:
    // @formatter:off
    void setOnClose  (FuncPrepare::OnClose   const & cb) { lock()->close_cb = cb; }
    void setOnWalk   (FuncPrepare::OnWalk    const & cb) { lock()->walk_cb  = cb; }
    void setOnPrepare(FuncPrepare::OnPrepare const & cb) { lock()->prepare_cb = cb; }
    // @formatter:on

public:
    Err init(UxLoop & loop);
    Err start();
    Err stop();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPREPARE_HPP__

