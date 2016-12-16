/**
 * @file   UvInStream.hpp
 * @brief  UvInStream class prototype.
 * @author zer0
 * @date   2016-12-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVINSTREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVINSTREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>
#include <libtbag/util/UvUtils.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace stream {

/**
 * libuv input stream utility class.
 *
 * @author zer0
 * @date   2016-12-16
 */
class TBAG_API UvInStream : public util::UvNative
{
public:
    using UvNative = util::UvNative;

public:
    struct Callback
    {
        virtual void onWrite(ErrorCode code) = 0;
    };

private:
    UvNative   _write;
    Callback * _callback;

public:
    UvInStream(Type type);
    ~UvInStream();

public:
    inline void setCallback(Callback * callback)
    { _callback = callback; }

public:
    ErrorCode write(char const * buffer, std::size_t length);
    ErrorCode write(std::vector<char> const & buffer);

public:
    void onWrite(void * request, int status);
};

} // namespace stream

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STREAM_UVINSTREAM_HPP__

