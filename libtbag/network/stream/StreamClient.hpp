/**
 * @file   StreamClient.hpp
 * @brief  StreamClient class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/network/Client.hpp>
#include <libtbag/network/stream/StreamClientBackend.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * StreamClient class prototype.
 *
 * @author zer0
 * @date   2017-05-10
 */
template <typename BaseStreamType>
class StreamClient : public Client
{
public:
    using BaseStream = BaseStreamType;
    STATIC_ASSERT_CHECK_IS_BASE_OF(uvpp::Stream, BaseStream);

public:
    using ClientBackend = StreamClientBackend<BaseStream>;

public:
    using SharedClientBackend = std::shared_ptr<ClientBackend>;
    using   WeakClientBackend =   std::weak_ptr<ClientBackend>;

private:
    SharedClientBackend    _client;
    SharedSafetyWriteAsync _async;
    SharedTimeoutClose     _close;
    SharedTimeoutShutdown  _shutdown;

    SharedSafetyWriter _last_writer;
    mutable Mutex _mutex;

public:
    StreamClient(Loop & loop)
    {
        bool const IS_AUTO_CLOSE = false;

        _client   = loop.newHandle<ClientBackend>(loop, this);
        _async    = loop.newHandle<SafetyWriteAsync>(loop);
        _close    = loop.newHandle<TimeoutClose>(loop, _client.get(), IS_AUTO_CLOSE);
        _shutdown = loop.newHandle<TimeoutShutdown>(loop, _client.get(), IS_AUTO_CLOSE);

        assert(static_cast<bool>(_client));
        assert(static_cast<bool>(_async));
        assert(static_cast<bool>(_close));
        assert(static_cast<bool>(_shutdown));
    }

    virtual ~StreamClient()
    {
        _client.reset();
        _async.reset();
        _close.reset();
        _shutdown.reset();
        _last_writer.reset();
    }

public:
    // @formatter:off
    inline WeakClientBackend     getClient() { Guard g(_mutex); return WeakClientBackend   (_client);   }
    inline WeakSafetyWriteAsync   getAsync() { Guard g(_mutex); return WeakSafetyWriteAsync(_async);    }
    inline WeakTimeoutClose       getClose() { Guard g(_mutex); return WeakTimeoutClose    (_close);    }
    inline WeakTimeoutShutdown getShutdown() { Guard g(_mutex); return WeakTimeoutShutdown (_shutdown); }

    inline bool isWriting() const
    { Guard g(_mutex); return static_cast<bool>(_last_writer); }

    virtual void     lock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(    _mutex.lock())) {   _mutex.lock(); }
    virtual void   unlock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(  _mutex.unlock())) { _mutex.unlock(); }
    virtual bool try_lock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_mutex.try_lock())) { return _mutex.try_lock(); }
    // @formatter:on

private:
    void startTimeoutShutdown(Milliseconds const & millisec)
    {
        assert(static_cast<bool>(_shutdown));
        Err const CODE = _shutdown->start(static_cast<uint64_t>(millisec.count()));
        tDLogD("StreamClient::startTimeoutShutdown({}) result code: {}", millisec.count(), getErrName(CODE));
    }

    void startTimeoutClose(Milliseconds const & millisec)
    {
        assert(static_cast<bool>(_close));
        Err const CODE = _close->start(static_cast<uint64_t>(millisec.count()));
        tDLogD("StreamClient::startTimeoutClose({}) result code: {}", millisec.count(), getErrName(CODE));
    }

    void cancelTimeoutShutdown()
    {
        assert(static_cast<bool>(_shutdown));
        if (_shutdown->isActive()) {
            _shutdown->cancel();
            _shutdown->stop();
        }
        tDLogD("StreamClient::cancelTimeoutShutdown()");
    }

    void cancelTimeoutClose()
    {
        assert(static_cast<bool>(_close));
        if (_close->isActive()) {
            _close->cancel();
            _close->stop();
        }
        tDLogD("StreamClient::cancelTimeoutClose()");
    }

private:
    bool write(SharedSafetyWriter writer, uint64_t millisec)
    {
        assert(static_cast<bool>(_client));
        assert(static_cast<bool>(_async));

        if (static_cast<bool>(writer) == false) {
            tDLogD("StreamClient::write() writer is nullptr.");
            return false;
        }

        if (static_cast<bool>(_last_writer)) {
            tDLogD("StreamClient::write() busy state.");
            return false;
        }

        Loop * loop = _client->getLoop();
        assert(loop != nullptr);

        if (millisec >= 1U) {
            startTimeoutShutdown(Milliseconds(millisec));
        }

        if (loop->isAliveAndThisThread()) {
            _last_writer = writer;
            _last_writer->run(nullptr);
        } else {
            _last_writer = writer;
            if (_async->asyncWrite(writer) == false) {
                _last_writer.reset();
            }
        }

        return true;
    }

private:
    void closeAll()
    {
        assert(static_cast<bool>(_close));
        if (_close->isActive()) {
            _close->cancel();
            _close->stop();
        }
        if (_close->isClosing() == false) {
            _close->close();
        }

        assert(static_cast<bool>(_shutdown));
        if (_shutdown->isActive()) {
            _shutdown->cancel();
            _shutdown->stop();
        }
        if (_shutdown->isClosing() == false) {
            _shutdown->close();
        }

        assert(static_cast<bool>(_client));
        if (_client->isClosing() == false) {
            _client->close();
        }

        assert(static_cast<bool>(_async));
        if (_async->isClosing() == false) {
            _async->close();
        }

        _last_writer.reset();
    }

public:
    /** Obtain the Client handle id. */
    virtual Id getId() const override
    {
        return _client->id();
    }

public:
    virtual bool init(String const & destination, int port = 0, uint64_t millisec = 0) override
    {
        assert(static_cast<bool>(_client));
        Guard guard(_mutex);
        if (static_cast<bool>(_client) && realInitialize(*_client, destination, port)) {
            if (millisec >= 1U) {
                startTimeoutClose(Milliseconds(millisec));
            }
            return true;
        }
        return false;
    }

public:
    virtual bool start() override
    {
        assert(static_cast<bool>(_client));
        Guard guard(_mutex);

        Err const CODE = _client->startRead();
        tDLogD("StreamClient::start() result code: {}", getErrName(CODE));
        return CODE == Err::E_SUCCESS;
    }

    virtual bool stop() override
    {
        assert(static_cast<bool>(_client));
        Guard guard(_mutex);

        Err const CODE = _client->stopRead();
        tDLogD("StreamClient::stop() result code: {}", getErrName(CODE));
        return CODE == Err::E_SUCCESS;
    }

    virtual void close() override
    {
        assert(static_cast<bool>(_client));
        assert(static_cast<bool>(_async));

        Loop * loop = _client->getLoop();
        assert(loop != nullptr);

        if (loop->isAliveAndThisThread()) {
            tDLogD("StreamClient::close() sync request.");
            Guard guard(_mutex);
            closeAll();
        } else {
            tDLogD("StreamClient::close() async request.");
            _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
                Guard guard(_mutex);
                closeAll();
            });
        }
    }

    virtual void cancel() override
    {
        assert(static_cast<bool>(_client));
        assert(static_cast<bool>(_async));

        Loop * loop = _client->getLoop();
        assert(loop != nullptr);

        if (loop->isAliveAndThisThread()) {
            tDLogD("StreamClient::cancel() sync request.");
            Guard guard(_mutex);
            startTimeoutShutdown(Milliseconds(0U));
        } else {
            tDLogD("StreamClient::cancel() async request.");
            _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
                Guard guard(_mutex);
                startTimeoutShutdown(Milliseconds(0U));
            });
        }
    }

public:
    virtual bool write(binf const * buffer, Size size, uint64_t millisec = 0) override
    {
        assert(static_cast<bool>(_client));
        Guard guard(_mutex);
        return write(SafetyWriteAsync::createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
    }

    virtual bool write(char const * buffer, Size size, uint64_t millisec = 0) override
    {
        assert(static_cast<bool>(_client));
        Guard guard(_mutex);
        return write(SafetyWriteAsync::createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
    }

public:
    virtual void * getUserData() override
    {
        assert(static_cast<bool>(_client));
        return _client->getUserData();
    }

    template <typename Predicated>
    void updateUserData(Predicated predicated)
    {
        assert(static_cast<bool>(_client));
        Guard guard(_mutex);
        predicated(_client->getUserData());
    }

public:
    virtual void runBackendConnect(Err code) override
    {
        _mutex.lock();
        cancelTimeoutClose();
        _mutex.unlock();

        onConnect(code);
    }

    virtual void runBackendShutdown(Err code) override
    {
        _mutex.lock();
        cancelTimeoutShutdown();
        _last_writer.reset();
        _mutex.unlock();

        onShutdown(code);
    }

    virtual void runBackendWrite(Err code) override
    {
        _mutex.lock();
        cancelTimeoutShutdown();
        _last_writer.reset();
        _mutex.unlock();

        onWrite(code);
    }

    virtual void runBackendRead(Err code, char const * buffer, Size size) override
    {
        onRead(code, buffer, size);
    }

    virtual void runBackendClose() override
    {
        onClose();

        _mutex.lock();
        closeAll();
        _mutex.unlock();
    }

public:
    virtual bool realInitialize(ClientBackend & backend, String const & destination, int port) = 0;
};

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENT_HPP__

