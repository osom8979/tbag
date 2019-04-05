/**
 * @file   UxFsPoll.hpp
 * @brief  UxFsPoll class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXFSPOLL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXFSPOLL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalFsPoll.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxFsPoll class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxFsPoll : public libtbag::uvxx::UxHandle
{
public:
    using FuncFsPoll   = libtbag::uvpp::func::FuncFsPoll;
    using WeakFsPoll   = std::weak_ptr<FuncFsPoll>;
    using SharedFsPoll = std::shared_ptr<FuncFsPoll>;

public:
    using FileState = FuncFsPoll::FileState;

public:
    UxFsPoll();
    UxFsPoll(UxLoop & loop);
    UxFsPoll(UxFsPoll const & obj) TBAG_NOEXCEPT;
    UxFsPoll(UxFsPoll && obj) TBAG_NOEXCEPT;
    ~UxFsPoll();

public:
    UxFsPoll & operator =(UxFsPoll const & obj) TBAG_NOEXCEPT;
    UxFsPoll & operator =(UxFsPoll && obj) TBAG_NOEXCEPT;

public:
    void copy(UxFsPoll const & obj) TBAG_NOEXCEPT;
    void swap(UxFsPoll & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxFsPoll & lh, UxFsPoll & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedFsPoll lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncFsPoll>(_handle.lock()); }

public:
    // clang-format off
    void setOnClose (FuncFsPoll::OnClose  const & cb) { lock()->close_cb   = cb; }
    void setOnWalk  (FuncFsPoll::OnWalk   const & cb) { lock()->walk_cb    = cb; }
    void setOnFsPoll(FuncFsPoll::OnFsPoll const & cb) { lock()->fs_poll_cb = cb; }
    // clang-format on

public:
    Err init(UxLoop & loop);

public:
    Err start(char const * path, unsigned int interval = 1000);
    Err stop();

public:
    std::string getPath();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXFSPOLL_HPP__

