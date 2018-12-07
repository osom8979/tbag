/**
 * @file   UxPipe.hpp
 * @brief  UxPipe class prototype.
 * @author zer0
 * @date   2018-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPIPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPIPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxStream.hpp>
#include <libtbag/uvpp/func/FunctionalPipe.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

/**
 * UxPipe class prototype.
 *
 * @author zer0
 * @date   2018-12-07
 */
class TBAG_API UxPipe : public libtbag::uvxx::UxStream
{
public:
    using FuncPipe   = libtbag::uvpp::func::FuncPipe;
    using WeakPipe   = std::weak_ptr<FuncPipe>;
    using SharedPipe = std::shared_ptr<FuncPipe>;

public:
    using ConnectRequest = libtbag::uvpp::ConnectRequest;

public:
    using ufile = libtbag::uvpp::ufile;

public:
    UxPipe();
    UxPipe(UxLoop & loop, bool is_ipc = false);
    UxPipe(UxPipe const & obj) TBAG_NOEXCEPT;
    UxPipe(UxPipe && obj) TBAG_NOEXCEPT;
    ~UxPipe();

public:
    UxPipe & operator =(UxPipe const & obj) TBAG_NOEXCEPT;
    UxPipe & operator =(UxPipe && obj) TBAG_NOEXCEPT;

public:
    void copy(UxPipe const & obj) TBAG_NOEXCEPT;
    void swap(UxPipe & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxPipe & lh, UxPipe & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedPipe lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncPipe>(_handle.lock()); }

public:
    // @formatter:off
    void setOnClose     (FuncPipe::OnClose      const & cb) { lock()->close_cb      = cb; }
    void setOnWalk      (FuncPipe::OnWalk       const & cb) { lock()->walk_cb       = cb; }
    void setOnShutdown  (FuncPipe::OnShutdown   const & cb) { lock()->shutdown_cb   = cb; }
    void setOnConnection(FuncPipe::OnConnection const & cb) { lock()->connection_cb = cb; }
    void setOnAlloc     (FuncPipe::OnAlloc      const & cb) { lock()->alloc_cb      = cb; }
    void setOnRead      (FuncPipe::OnRead       const & cb) { lock()->read_cb       = cb; }
    void setOnWrite     (FuncPipe::OnWrite      const & cb) { lock()->write_cb      = cb; }
    void setOnConnect   (FuncPipe::OnConnect    const & cb) { lock()->connect_cb    = cb; }
    // @formatter:on

public:
    Err init(UxLoop & loop, bool is_ipc = false);

public:
    Err open(ufile file);
    Err bind(char const * name);
    void connect(ConnectRequest & request, char const * name);

public:
    std::string getSockName() const;
    std::string getPeerName() const;

public:
    void pendingInstances(int count);
    int getPendingCount();
    int getPendingType();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXPIPE_HPP__

