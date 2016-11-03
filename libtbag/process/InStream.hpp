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
#include <libtbag/predef.hpp>
#include <libtbag/loop/event/UvHandler.hpp>

#include <vector>
#include <memory>
#include <functional>

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
class TBAG_API InStream : public libtbag::Noncopyable
{
public:
    struct PipePimpl;
    friend struct PipePimpl;

public:
    enum class ErrorCode
    {
        UNKNOWN_ERROR = 0,
        SUCCESS,
    };

public:
    using UniquePipePimpl = std::unique_ptr<PipePimpl>;
    using OnWriteCallback = std::function<void(ErrorCode)>;

private:
    UniquePipePimpl _pipe;
    OnWriteCallback _callback;

public:
    InStream(OnWriteCallback const & callback);
    InStream();
    ~InStream();

public:
    inline void setOnWriteCallback(OnWriteCallback const & callback)
    { _callback = callback; }

public:
    void * getNative();
    void const * getNative() const;

public:
    bool write(char const * buffer, std::size_t length);
    bool write(std::vector<char> const & buffer);
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_INSTREAM_HPP__

