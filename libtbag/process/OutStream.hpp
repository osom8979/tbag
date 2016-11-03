/**
 * @file   OutStream.hpp
 * @brief  OutStream class prototype.
 * @author zer0
 * @date   2016-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_OUTSTREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_OUTSTREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * OutStream class prototype.
 *
 * @author zer0
 * @date   2016-11-03
 */
class OutStream : public libtbag::Noncopyable
{
public:
    struct PipePimpl;
    friend struct PipePimpl;

public:
    using UniquePipePimpl = std::unique_ptr<PipePimpl>;

public:
    struct OnReadCallback
    {
        virtual void onRead(ErrorCode code, char * buffer, std::size_t length) = 0;
    };

public:
    enum class ErrorCode
    {
        ERROR = 0,
        SUCCESS,
        END_OF_FILE
    };

private:
    UniquePipePimpl _pipe;
    OnReadCallback * _callback;

public:
    OutStream(OnReadCallback * callback);
    OutStream();
    virtual ~OutStream();

public:
    inline void setOnReadCallback(OnReadCallback * callback)
    { _callback = callback; }

public:
    void * getNative();
    void const * getNative() const;
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_OUTSTREAM_HPP__

