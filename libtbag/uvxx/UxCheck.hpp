/**
 * @file   UxCheck.hpp
 * @brief  UxCheck class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCHECK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCHECK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalCheck.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxCheck class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxCheck : public libtbag::uvxx::UxHandle
{
public:
    using FuncCheck   = libtbag::uvpp::func::FuncCheck;
    using WeakCheck   = std::weak_ptr<FuncCheck>;
    using SharedCheck = std::shared_ptr<FuncCheck>;

public:
    UxCheck();
    UxCheck(UxLoop & loop);
    UxCheck(UxCheck const & obj) TBAG_NOEXCEPT;
    UxCheck(UxCheck && obj) TBAG_NOEXCEPT;
    ~UxCheck();

public:
    UxCheck & operator =(UxCheck const & obj) TBAG_NOEXCEPT;
    UxCheck & operator =(UxCheck && obj) TBAG_NOEXCEPT;

public:
    void copy(UxCheck const & obj) TBAG_NOEXCEPT;
    void swap(UxCheck & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxCheck & lh, UxCheck & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedCheck lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncCheck>(_handle.lock()); }

public:
    // clang-format off
    void setOnClose(FuncCheck::OnClose const & cb) { lock()->close_cb = cb; }
    void setOnWalk (FuncCheck::OnWalk  const & cb) { lock()->walk_cb  = cb; }
    void setOnCheck(FuncCheck::OnCheck const & cb) { lock()->check_cb = cb; }
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCHECK_HPP__

