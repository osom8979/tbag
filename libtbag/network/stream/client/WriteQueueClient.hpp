/**
 * @file   WriteQueueClient.hpp
 * @brief  WriteQueueClient class prototype.
 * @author zer0
 * @date   2017-09-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITEQUEUECLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITEQUEUECLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/network/stream/client/WriteStateClient.hpp>
#include <libtbag/container/ReuseQueue.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {
namespace client  {

/**
 * WriteQueueClient class prototype.
 *
 * @author zer0
 * @date   2017-09-24
 *
 * @warning
 *  Do not use a mutex if the function name starts with '_'.
 */
class TBAG_API WriteQueueClient : public WriteStateClient
{
public:
    using Buffer       = std::vector<char>;
    using SharedBuffer = std::shared_ptr<Buffer>;
    using ReuseQueue   = container::ReuseQueue<SharedBuffer>;

    using StreamType = WriteStateClient::StreamType;
    using WriteReady = WriteStateClient::WriteReady;
    using Loop       = WriteStateClient::Loop;

    using ClientGuard = std::lock_guard<WriteStateClient>;

private:
    ReuseQueue  _queue;
    std::size_t _max_size;

public:
    WriteQueueClient(ClientInterface * parent, Loop & loop, StreamType type);
    WriteQueueClient(ClientInterface * parent, Loop & loop, StreamType type, WriteReady const & ready);
    virtual ~WriteQueueClient();

protected:
    Err _writeFromQueue();

public:
    bool empty() const;
    std::size_t size() const;

public:
    std::size_t getMaxSize() const;
    void setMaxSize(std::size_t size);

public:
    Err autoWrite(char const * buffer, std::size_t size);

public:
    virtual void preShutdown(Err code) override;
    virtual void preWrite   (Err code) override;
};

} // namespace client
} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITEQUEUECLIENT_HPP__

