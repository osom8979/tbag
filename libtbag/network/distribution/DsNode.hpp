/**
 * @file   DsNode.hpp
 * @brief  DsNode class prototype.
 * @author zer0
 * @date   2018-10-26
 * @date   2018-11-01 (Rename: Node -> DsNode)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DISTRIBUTION_DSNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DISTRIBUTION_DSNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network      {
namespace distribution {

/**
 * Distribution Node class prototype.
 *
 * @author zer0
 * @date   2018-10-26
 * @date   2018-11-01 (Rename: Node -> DsNode)
 */
class TBAG_API DsNode
{
public:
    struct Impl;
    friend struct Impl;

public:
    enum class State : int
    {
        S_NONE, ///< @warning If the state is closed, this value is returned. It is not used in actual logic.

        S_OPENING,
        S_OPENED,
        S_CLOSING,
    };

public:
    inline static char const * const getStateName(State state) TBAG_NOEXCEPT
    {
        switch (state) {
        case State::S_NONE:     return "NONE";
        case State::S_OPENING:  return "OPENING";
        case State::S_OPENED:   return "OPENED";
        case State::S_CLOSING:  return "CLOSING";
        default:                return "UNKNOWN";
        }
    }

public:
    struct Event
    {
        virtual void onConnect(std::string const & name) = 0;
        virtual void onDisconnect(std::string const & name) = 0;
        virtual void onRecv(std::string const & name, char const * buffer, std::size_t size) = 0;
    };

    struct FuncEvent : public Event
    {
        using OnConnectCb    = std::function<void(std::string const &)>;
        using OnDisconnectCb = std::function<void(std::string const &)>;
        using OnRecvCb       = std::function<void(std::string const &, char const *, std::size_t)>;

        OnConnectCb     connect_cb;
        OnDisconnectCb  disconnect_cb;
        OnRecvCb        recv_cb;

        virtual void onConnect(std::string const & name) override
        {
            if (connect_cb) {
                connect_cb(name);
            }
        }

        virtual void onDisconnect(std::string const & name) override
        {
            if (disconnect_cb) {
                disconnect_cb(name);
            }
        }

        virtual void onRecv(std::string const & name, char const * buffer, std::size_t size) override
        {
            if (recv_cb) {
                recv_cb(name, buffer, size);
            }
        }
    };

public:
    using SharedImpl      = std::shared_ptr<Impl>;
    using SharedEvent     = std::shared_ptr<Event>;
    using SharedFuncEvent = std::shared_ptr<FuncEvent>;

private:
    SharedImpl  _impl;
    SharedEvent _event;

public:
    DsNode();
    DsNode(DsNode const & obj) TBAG_NOEXCEPT;
    DsNode(DsNode && obj) TBAG_NOEXCEPT;
    ~DsNode();

public:
    DsNode & operator =(DsNode const & obj) TBAG_NOEXCEPT;
    DsNode & operator =(DsNode && obj) TBAG_NOEXCEPT;

public:
    void copy(DsNode const & obj) TBAG_NOEXCEPT;
    void swap(DsNode & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(DsNode & lh, DsNode & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

    inline SharedEvent       & event()       TBAG_NOEXCEPT { return _event; }
    inline SharedEvent const & event() const TBAG_NOEXCEPT { return _event; }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(DsNode const & x, DsNode const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(DsNode const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(DsNode const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    SharedFuncEvent updateFunctionalEvent();

public:
    Err openTcp(std::string const & name, std::string const & host, int port = 0, bool verbose = false);
    Err openPipe(std::string const & name, std::string const & path, bool verbose = false);

public:
    Err open(std::string const & name, std::string const & schema, std::string const & host,
             int port, bool verbose = false);
    Err open(std::string const & name, std::string const & schema, std::string const & host,
             bool verbose = false);

public:
    Err open(std::string const & uri);
    void close();

public:
    TBAG_CONSTEXPR static int const INFINITE_TIMEOUT = 0;

public:
    State getState() const;
    bool busyWaitingUntilConnected(std::string const & name, int timeout_millisec = INFINITE_TIMEOUT) const;

public:
    std::string host() const;
    int port() const;

public:
    std::vector<std::string> nodes() const;
    bool isConnected(std::string const & name) const;

public:
    Err connectTcp(std::string const & name, std::string const & host, int port);
    Err connectPipe(std::string const & name, std::string const & path);

public:
    Err connect(std::string const & name, std::string const & schema, std::string const & host, int port);
    Err connect(std::string const & name, std::string const & schema, std::string const & host);

public:
    Err connect(std::string const & name, std::string const & uri);
    Err connect(std::string const & uri);
    Err disconnect(std::string const & name);

public:
    Err write(std::string const & name, char const * buffer, std::size_t size);
    Err write(std::string const & name, std::string const & message);
};

} // namespace distribution
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DISTRIBUTION_DSNODE_HPP__

