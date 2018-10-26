/**
 * @file   Node.hpp
 * @brief  Node class prototype.
 * @author zer0
 * @date   2018-10-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_NODE_NODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_NODE_NODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace node    {

/**
 * Node class prototype.
 *
 * @author zer0
 * @date   2018-10-26
 */
class TBAG_API Node
{
public:
    struct Impl;
    friend struct Impl;

public:
    enum class NodeType : int
    {
        NT_NONE,
        NT_MASTER,
        NT_SLAVE,
    };

public:
    struct Event
    {
        // @formatter:off
        virtual void onOpen(Err code){};
        virtual void onClose(Err code){};

        virtual void onLogin(std::string const & client_name, Err code){};
        virtual void onLogout(std::string const & client_name, Err code){};

        virtual void onConnect(std::string const & server_name, Err code){};
        virtual void onDisconnect(std::string const & server_name, Err code){};

        virtual void onServerRecv(std::string const & client_name, char const * buffer, std::size_t size, Err code){};
        virtual void onClientRecv(std::string const & server_name, char const * buffer, std::size_t size, Err code){};
        // @formatter:on
    };

public:
    using SharedImpl  = std::shared_ptr<Impl>;
    using SharedEvent = std::shared_ptr<Event>;

private:
    SharedImpl  _impl;
    SharedEvent _event;

public:
    Node();
    Node(std::nullptr_t) TBAG_NOEXCEPT;
    Node(Node const & obj) TBAG_NOEXCEPT;
    Node(Node && obj) TBAG_NOEXCEPT;
    ~Node();

public:
    Node & operator =(Node const & obj) TBAG_NOEXCEPT;
    Node & operator =(Node && obj) TBAG_NOEXCEPT;

public:
    void copy(Node const & obj) TBAG_NOEXCEPT;
    void swap(Node & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Node & lh, Node & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(Node const & x, Node const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(Node const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

public:
    Err open(std::string const & uri);
    Err close();

public:
    Err connect(std::string const & server_name, std::string const & uri);
    Err disconnect(std::string const & server_name);

public:
    Err c2s(std::string const & server_name, char const * buffer, std::size_t size);
    Err s2c(std::string const & client_name, char const * buffer, std::size_t size);
};

} // namespace node
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_NODE_NODE_HPP__

