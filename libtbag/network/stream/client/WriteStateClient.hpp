/**
 * @file   WriteStateClient.hpp
 * @brief  WriteStateClient class prototype.
 * @author zer0
 * @date   2017-09-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITESTATECLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITESTATECLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/ReuseQueue.hpp>
#include <libtbag/network/stream/client/WriteStateProperty.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {
namespace client  {

/**
 * Internal StreamClient class prototype.
 *
 * @author zer0
 * @date   2017-06-16
 * @date   2017-09-24 (Rename: StreamClient::Internal -> InternalClient)
 * @date   2017-09-27 (Rename: InternalClient -> WriteStateClient)
 */
class TBAG_API WriteStateClient : public BasicClientTypes,
                                  public BasicClientTypes::OnUserTimerCallback,
                                  public BasicClientTypes::OnShutdownTimerCallback,
                                  public BasicClientTypes::OnAsyncWriteCallback,
                                  private Noncopyable
{
public:
    struct Internal;
    friend struct Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    mutable Mutex _mutex;
    UniqueInternal _internal;

private:
    ClientInterface * _parent;
    void * _user_data;

public:
    WriteStateClient(ClientInterface * parent, Loop & loop, StreamType type);
    WriteStateClient(ClientInterface * parent, Loop & loop, StreamType type, UpdateReady const & UNUSED_PARAM(ready));
    virtual ~WriteStateClient();

public:
    inline void       * getUserData()       TBAG_NOEXCEPT { Guard g(_mutex); return _user_data; }
    inline void const * getUserData() const TBAG_NOEXCEPT { Guard g(_mutex); return _user_data; }

    inline void * udata(void * data = nullptr) TBAG_NOEXCEPT
    {
        Guard g(_mutex);
        if (data != nullptr) {
            _user_data = data;
        }
        return _user_data;
    }

public:
    StreamType   getStreamType() const TBAG_NOEXCEPT;
    WriteState   getState     () const;
    char const * getStateName () const;

    uint64_t getWriteTimeout(uint64_t millisec) const;
    void setWriteTimeout(uint64_t millisec);

    WeakStream          getClient       ();
    WeakSafetyAsync     getAsync        ();
    SharedUserTimer     getUserTimer    ();
    SharedShutdownTimer getShutdownTimer();

public:
    Id            id() const;
    std::string dest() const;
    int         port() const;

    Err  initClient(std::string const & destination, int port = 0);
    Err  initInternalHandles();

    Err  startRead();
    Err   stopRead();

    bool isActiveUserTimer() const;
    Err     startUserTimer(uint64_t millisec);
    Err      stopUserTimer();

    Err   close();
    Err  cancel();

    Err write(char const * buffer, std::size_t size);

protected:
    virtual void     onUserTimer() override;
    virtual void onShutdownTimer() override;
    virtual void    onAsyncWrite() override;

public:
    void preConnect (Err code);
    void preShutdown(Err code);
    void preWrite   (Err code);
    void preRead    (Err code, ReadPacket const & packet);
    void preClose   ();
};

} // namespace client
} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITESTATECLIENT_HPP__

