/**
 * @file   UvOutStream.hpp
 * @brief  UvOutStream class prototype.
 * @author zer0
 * @date   2016-12-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVOUTSTREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVOUTSTREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>
#include <libtbag/uv/Handle.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace stream {

/**
 * libuv output stream utility class.
 *
 * @author zer0
 * @date   2016-12-16
 */
class TBAG_API UvOutStream : public uv::Handle
{
public:
    using UvHandleType = uv::UvHandleType;
    using Buffer = std::vector<char>;

public:
    struct OnReadCallback
    {
        virtual void onRead(Err code, ssize_t nread, void const * buf) = 0;
    };

private:
    Buffer _buffer;
    OnReadCallback * _on_read_cb;

public:
    inline void setOnReadCallback(OnReadCallback * callback) TBAG_NOEXCEPT
    { _on_read_cb = callback; }

public:
    UvOutStream(UvHandleType type);
    ~UvOutStream();

public:
    Err read();

public:
    void onAlloc(void * handle, size_t suggested_size, void * buf);
    void onRead(void * stream, ssize_t nread, void const * buf);
};

} // namespace stream

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVOUTSTREAM_HPP__

