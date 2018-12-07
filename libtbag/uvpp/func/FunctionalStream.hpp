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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalStream class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
template <typename StreamType>
struct FunctionalStream : public FunctionalHandle<StreamType>
{
    using Parent       = FunctionalHandle<StreamType>;
    using OnShutdown   = std::function<void(ShutdownRequest&, Err)>;
    using OnConnection = std::function<void(Err)>;
    using OnAlloc      = std::function<binf(std::size_t)>;
    using OnRead       = std::function<void(Err, char const *, std::size_t)>;
    using OnWrite      = std::function<void(WriteRequest&, Err)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Stream, Parent);

    OnShutdown   shutdown_cb;
    OnConnection connection_cb;
    OnAlloc      alloc_cb;
    OnRead       read_cb;
    OnWrite      write_cb;

    template <typename ... Args>
    FunctionalStream(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalStream()
    { /* EMPTY. */ }

    virtual void onShutdown(ShutdownRequest & request, Err code) override
    {
        if (shutdown_cb) {
            shutdown_cb(request, code);
        } else {
            Parent::onShutdown(request, code);
        }
    }

    virtual void onConnection(Err code) override
    {
        if (connection_cb) {
            connection_cb(code);
        } else {
            Parent::onConnection(code);
        }
    }

    virtual binf onAlloc(std::size_t suggested_size) override
    {
        if (alloc_cb) {
            return alloc_cb(suggested_size);
        } else {
            return Parent::onAlloc(suggested_size);
        }
    }

    virtual void onRead(Err code, char const * buffer, std::size_t size) override
    {
        if (read_cb) {
            read_cb(code, buffer, size);
        } else {
            Parent::onRead(code, buffer, size);
        }
    }

    virtual void onWrite(WriteRequest & request, Err code) override
    {
        if (write_cb) {
            write_cb(request, code);
        } else {
            Parent::onWrite(request, code);
        }
    }
};

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALSTREAM_HPP__

