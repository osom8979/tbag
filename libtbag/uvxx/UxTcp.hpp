/**
 * @file   UxTcp.hpp
 * @brief  UxTcp class prototype.
 * @author zer0
 * @date   2018-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxStream.hpp>
#include <libtbag/uvpp/func/FunctionalTcp.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxTcp class prototype.
 *
 * @author zer0
 * @date   2018-12-07
 */
class TBAG_API UxTcp : public libtbag::uvxx::UxStream
{
public:
    using FuncTcp   = libtbag::uvpp::func::FuncTcp;
    using WeakTcp   = std::weak_ptr<FuncTcp>;
    using SharedTcp = std::shared_ptr<FuncTcp>;

public:
    using ConnectRequest = libtbag::uvpp::ConnectRequest;

public:
    using BindFlag = FuncTcp::BindFlag;

public:
    UxTcp();
    UxTcp(UxLoop & loop);
    UxTcp(UxTcp const & obj) TBAG_NOEXCEPT;
    UxTcp(UxTcp && obj) TBAG_NOEXCEPT;
    ~UxTcp();

public:
    UxTcp & operator =(UxTcp const & obj) TBAG_NOEXCEPT;
    UxTcp & operator =(UxTcp && obj) TBAG_NOEXCEPT;

public:
    void copy(UxTcp const & obj) TBAG_NOEXCEPT;
    void swap(UxTcp & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxTcp & lh, UxTcp & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedTcp lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncTcp>(_handle.lock()); }

public:
    // @formatter:off
    void setOnClose     (FuncTcp::OnClose      const & cb) { lock()->close_cb      = cb; }
    void setOnWalk      (FuncTcp::OnWalk       const & cb) { lock()->walk_cb       = cb; }
    void setOnShutdown  (FuncTcp::OnShutdown   const & cb) { lock()->shutdown_cb   = cb; }
    void setOnConnection(FuncTcp::OnConnection const & cb) { lock()->connection_cb = cb; }
    void setOnAlloc     (FuncTcp::OnAlloc      const & cb) { lock()->alloc_cb      = cb; }
    void setOnRead      (FuncTcp::OnRead       const & cb) { lock()->read_cb       = cb; }
    void setOnWrite     (FuncTcp::OnWrite      const & cb) { lock()->write_cb      = cb; }
    void setOnConnect   (FuncTcp::OnConnect    const & cb) { lock()->connect_cb    = cb; }
    // @formatter:on

public:
    Err init(UxLoop & loop);

public:
    Err setNodelay(bool enable = true);
    Err keepAlive(bool enable, unsigned int delay);
    Err acceptsSimultaneous(int enable = true);

public:
    Err bind(sockaddr const * address, unsigned int flags = 0);
    Err bind(sockaddr const * address, BindFlag flags);

public:
    Err getSockName(struct sockaddr * name, int * namelen);
    Err getPeerName(struct sockaddr * name, int * namelen);

public:
    std::string getSockIp();
    std::string getPeerIp();

public:
    int getSockPort();
    int getPeerPort();

public:
    Err connect(ConnectRequest & request, sockaddr const * address);
};

// ----------------
// Utility methods.
// ----------------

// @formatter:off
TBAG_API Err initCommonClientSock(UxTcp & tcp, UxTcp::ConnectRequest & request, struct sockaddr const * addr);
TBAG_API Err initCommonClient    (UxTcp & tcp, UxTcp::ConnectRequest & request, std::string const & host, int port);

TBAG_API Err initCommonServerSock(UxTcp & tcp, struct sockaddr const * addr);
TBAG_API Err initCommonServer    (UxTcp & tcp, std::string const & ip, int port);
// @formatter:on

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXTCP_HPP__

