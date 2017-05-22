/**
 * @file   StreamServerNode.hpp
 * @brief  StreamServerNode class prototype.
 * @author zer0
 * @date   2017-05-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVERNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVERNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/Client.hpp>
#include <libtbag/network/Server.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * StreamServerNode class prototype.
 *
 * @author zer0
 * @date   2017-05-23
 */
template <typename BaseType>
class StreamServerNode : public BaseType
{
public:
    using Parent = BaseType;
    STATIC_ASSERT_CHECK_IS_BASE_OF(Client, Parent);

public:
    using WeakClient = Server::WeakClient;
    using Loop       = uvpp::Loop;

public:
    Server * _parent;

public:
    StreamServerNode(Loop & loop, Server * parent) : Parent(loop), _parent(parent)
    {
        // EMPTY.
    }

    virtual ~StreamServerNode()
    {
        // EMPTY.
    }

private:
    WeakClient getWeakClient()
    {
        assert(_parent != nullptr);
        return _parent->getClient(this->getId());
    }

public:
    virtual void onConnect(Err code) override
    {
        // EMPTY.
    }

    virtual void onShutdown(Err code) override
    {
        assert(_parent != nullptr);
        _parent->onClientShutdown(getWeakClient(), code);
    }

    virtual void onWrite(Err code) override
    {
        assert(_parent != nullptr);
        _parent->onClientWrite(getWeakClient(), code);
    }

    virtual void onRead(Err code, char const * buffer, std::size_t size) override
    {
        assert(_parent != nullptr);
        _parent->onClientRead(getWeakClient(), code, buffer, size);
    }

    virtual void onClose() override
    {
        assert(_parent != nullptr);
        _parent->onClientClose(getWeakClient());

        // Deallocate user data.
        void * user_data = this->getUserData();
        if (user_data) {
            _parent->onClientUserDataDealloc(getWeakClient(), user_data);
        }
    }
};

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVERNODE_HPP__

