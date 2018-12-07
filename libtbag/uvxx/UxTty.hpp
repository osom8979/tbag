/**
 * @file   UxTty.hpp
 * @brief  UxTty class prototype.
 * @author zer0
 * @date   2018-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxStream.hpp>
#include <libtbag/uvpp/func/FunctionalTty.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxTty class prototype.
 *
 * @author zer0
 * @date   2018-12-07
 */
class TBAG_API UxTty : public libtbag::uvxx::UxStream
{
public:
    using FuncTty   = libtbag::uvpp::func::FuncTty;
    using WeakTty   = std::weak_ptr<FuncTty>;
    using SharedTty = std::shared_ptr<FuncTty>;

public:
    using TtyMode     = FuncTty::TtyMode;
    using GeneralFile = FuncTty::GeneralFile;

public:
    using ufile = libtbag::uvpp::ufile;

public:
    UxTty();
    UxTty(UxLoop & loop, ufile fd, bool readable);
    UxTty(UxLoop & loop, GeneralFile fd);
    UxTty(UxTty const & obj) TBAG_NOEXCEPT;
    UxTty(UxTty && obj) TBAG_NOEXCEPT;
    ~UxTty();

public:
    UxTty & operator =(UxTty const & obj) TBAG_NOEXCEPT;
    UxTty & operator =(UxTty && obj) TBAG_NOEXCEPT;

public:
    void copy(UxTty const & obj) TBAG_NOEXCEPT;
    void swap(UxTty & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxTty & lh, UxTty & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedTty lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncTty>(_handle.lock()); }

public:
    // @formatter:off
    void setOnClose     (FuncTty::OnClose      const & cb) { lock()->close_cb      = cb; }
    void setOnWalk      (FuncTty::OnWalk       const & cb) { lock()->walk_cb       = cb; }
    void setOnShutdown  (FuncTty::OnShutdown   const & cb) { lock()->shutdown_cb   = cb; }
    void setOnConnection(FuncTty::OnConnection const & cb) { lock()->connection_cb = cb; }
    void setOnAlloc     (FuncTty::OnAlloc      const & cb) { lock()->alloc_cb      = cb; }
    void setOnRead      (FuncTty::OnRead       const & cb) { lock()->read_cb       = cb; }
    void setOnWrite     (FuncTty::OnWrite      const & cb) { lock()->write_cb      = cb; }
    // @formatter:on

public:
    Err init(UxLoop & loop, ufile fd, bool readable);
    Err init(UxLoop & loop, GeneralFile fd);

public:
    Err setMode(TtyMode mode);
    Err resetMode();

public:
    Err getWinSize(int * width, int * height);

public:
    static ufile toFile(GeneralFile fd) TBAG_NOEXCEPT;

public:
    static bool guessHandle(ufile fd);
    static bool guessHandle(GeneralFile fd);
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTTY_HPP__

