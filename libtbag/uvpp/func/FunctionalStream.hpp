/**
 * @file   FunctionalStream.hpp
 * @brief  FunctionalStream class prototype.
 * @author zer0
 * @date   2017-09-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALSTREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALSTREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/lock/FakeLock.hpp>

#include <functional>
#include <utility>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalStream class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
template <typename StreamType, typename MutexType = lock::FakeLock>
class FunctionalStream : public StreamType
{
public:
    using Parent = StreamType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Stream, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

public:
    using OnShutdown   = std::function<void(ShutdownRequest&, Err)>;
    using OnConnection = std::function<void(Err)>;
    using OnAlloc      = std::function<binf(std::size_t)>;
    using OnRead       = std::function<void(Err, char const *, std::size_t)>;
    using OnWrite      = std::function<void(WriteRequest&, Err)>;

protected:
    Mutex _mutex;

private:
    OnShutdown   _shutdown_cb;
    OnConnection _connection_cb;
    OnAlloc      _alloc_cb;
    OnRead       _read_cb;
    OnWrite      _write_cb;

public:
    template <typename ... Args>
    FunctionalStream(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }
    virtual ~FunctionalStream()
    { /* EMPTY. */ }

public:
    // @formatter:off
    void setOnShutdown  (OnShutdown   const & cb) { Guard guard(_mutex);   _shutdown_cb = cb; }
    void setOnConnection(OnConnection const & cb) { Guard guard(_mutex); _connection_cb = cb; }
    void setOnAlloc     (OnAlloc      const & cb) { Guard guard(_mutex);      _alloc_cb = cb; }
    void setOnRead      (OnRead       const & cb) { Guard guard(_mutex);       _read_cb = cb; }
    void setOnWrite     (OnWrite      const & cb) { Guard guard(_mutex);      _write_cb = cb; }
    // @formatter:on

public:
    virtual void onShutdown(ShutdownRequest & request, Err code) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_shutdown_cb)) {
            _shutdown_cb(request, code);
        }
    }

    virtual void onConnection(Err code) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_connection_cb)) {
            _connection_cb(code);
        }
    }

    virtual binf onAlloc(std::size_t suggested_size) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_alloc_cb)) {
            return _alloc_cb(suggested_size);
        }
        return binf();
    }

    virtual void onRead(Err code, char const * buffer, std::size_t size) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_read_cb)) {
            _read_cb(code, buffer, size);
        }
    }

    virtual void onWrite(WriteRequest & request, Err code) override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_write_cb)) {
            _write_cb(request, code);
        }
    }
};

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALSTREAM_HPP__

