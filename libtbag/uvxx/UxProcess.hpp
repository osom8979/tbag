/**
 * @file   UxProcess.hpp
 * @brief  UxProcess class prototype.
 * @author zer0
 * @date   2018-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalProcess.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxProcess class prototype.
 *
 * @author zer0
 * @date   2018-12-07
 */
class TBAG_API UxProcess : public libtbag::uvxx::UxHandle
{
public:
    using FuncProcess   = libtbag::uvpp::func::FuncProcess;
    using WeakProcess   = std::weak_ptr<FuncProcess>;
    using SharedProcess = std::shared_ptr<FuncProcess>;

public:
    using Options = FuncProcess::Options;

public:
    UxProcess();
    UxProcess(UxLoop & loop, Options const & options);
    UxProcess(UxProcess const & obj) TBAG_NOEXCEPT;
    UxProcess(UxProcess && obj) TBAG_NOEXCEPT;
    ~UxProcess();

public:
    UxProcess & operator =(UxProcess const & obj) TBAG_NOEXCEPT;
    UxProcess & operator =(UxProcess && obj) TBAG_NOEXCEPT;

public:
    void copy(UxProcess const & obj) TBAG_NOEXCEPT;
    void swap(UxProcess & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxProcess & lh, UxProcess & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedProcess lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncProcess>(_handle.lock()); }

public:
    // clang-format off
    void setOnClose(FuncProcess::OnClose const & cb) { lock()->close_cb = cb; }
    void setOnWalk (FuncProcess::OnWalk  const & cb) { lock()->walk_cb  = cb; }
    void setOnExit (FuncProcess::OnExit  const & cb) { lock()->exit_cb  = cb; }
    // clang-format on

public:
    Err spawn(UxLoop & loop, Options const & options);
    Err processKill(int signum);

public:
    int getPid() const;

public:
    static void disableStdioInheritance();
    static Err kill(int pid, int signum);
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPROCESS_HPP__

