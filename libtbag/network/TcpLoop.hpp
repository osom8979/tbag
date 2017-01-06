/**
 * @file   TcpLoop.hpp
 * @brief  TcpLoop class prototype.
 * @author zer0
 * @date   2016-12-29
 * @date   2016-12-31 (Craete BaseTcpLoop)
 * @date   2017-01-02 (Remove BaseTcpLoop)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/CommonTcp.hpp>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Async.hpp>
#include <libtbag/uv/Tcp.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * TcpLoop class prototype.
 *
 * @author zer0
 * @date   2016-12-29
 */
class TBAG_API TcpLoop : public CommonTcp
{
public:
    using Parent = CommonTcp;

public:
    struct AsyncHelper : public libtbag::uv::Async
    {
        enum class ActionType
        {
            CLOSE,
            START_READ,
            STOP_READ,
        };

        TcpLoop & loop;
        ActionType const ACTION;

        AsyncHelper(TcpLoop & l, ActionType a) : loop(l), ACTION(a)
        { libtbag::uv::Async::init(loop.atLoop()); }

        virtual void onAsync() override
        {
            switch (ACTION) {
            case ActionType::CLOSE:      loop.atTcp()->close();     break;
            case ActionType::START_READ: loop.atTcp()->startRead(); break;
            case ActionType::STOP_READ:  loop.atTcp()->stopRead();  break;
            }
            close();
        }

        virtual void onClose() override
        {
            loop.atLoop().eraseChildHandle(this);
        }
    };

public:
    using SharedAsyncHelper = std::shared_ptr<AsyncHelper>;

private:
    uv::Loop _loop;

public:
    TcpLoop();
    virtual ~TcpLoop();

public:
    // @formatter:off
    inline uv::Loop       & atLoop()       TBAG_NOEXCEPT { return _loop; }
    inline uv::Loop const & atLoop() const TBAG_NOEXCEPT { return _loop; }
    // @formatter:on

public:
    inline bool isEqualOwnerThreadId() const
    { return atLoop().getOwnerThreadId() == std::this_thread::get_id(); }

private:
    void safeAsync(AsyncHelper::ActionType type);

public:
    void safeClose    ();
    void safeStartRead();
    void safeStopRead ();

public:
    virtual bool run(std::string const & ip, int port) = 0;
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__

