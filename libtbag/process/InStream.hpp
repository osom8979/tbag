/**
 * @file   InStream.hpp
 * @brief  InStream class prototype.
 * @author zer0
 * @date   2016-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_INSTREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_INSTREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/loop/event/UvHandler.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * InStream class prototype.
 *
 * @author zer0
 * @date   2016-11-03
 */
class InStream : public libtbag::Noncopyable
{
public:
    struct PipePimpl;
    friend struct PipePimpl;

public:
    using UniquePipePimpl = std::unique_ptr<PipePimpl>;

public:
    enum class ErrorCode
    {
        UNKNOWN_ERROR = 0,
        SUCCESS,
    };

public:
    struct OnWriteCallback
    {
        virtual void onWrite(ErrorCode code) = 0;
    };

private:
    UniquePipePimpl _pipe;
    OnWriteCallback * _callback;

public:
    InStream(OnWriteCallback * callback);
    InStream();
    virtual ~InStream();

public:
    inline void setOnReadCallback(OnWriteCallback * callback)
    { _callback = callback; }

public:
    void * getNative();
    void const * getNative() const;

public:
    bool write(char * buffer, std::size_t length);
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_INSTREAM_HPP__

