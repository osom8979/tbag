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
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <memory>
#include <functional>

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
class TBAG_API OutStream : public libtbag::Noncopyable
{
public:
    struct PipePimpl;
    friend struct PipePimpl;

public:
    enum class ErrorCode
    {
        UNKNOWN_ERROR = 0,
        SUCCESS,
        END_OF_FILE
    };

public:
    using UniquePipePimpl = std::unique_ptr<PipePimpl>;
    using OnReadCallback = std::function<void(ErrorCode, char*, std::size_t)>;

private:
    UniquePipePimpl _pipe;
    OnReadCallback _callback;

public:
    OutStream(OnReadCallback const & callback);
    OutStream();
    ~OutStream();

public:
    inline void setOnReadCallback(OnReadCallback const & callback)
    { _callback = callback; }

public:
    void * getNative();
    void const * getNative() const;

public:
    bool startRead();
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_OUTSTREAM_HPP__

