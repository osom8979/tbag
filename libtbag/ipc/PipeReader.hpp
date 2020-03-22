/**
 * @file   PipeReader.hpp
 * @brief  PipeReader class prototype.
 * @author zer0
 * @date   2020-03-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_IPC_PIPEREADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_IPC_PIPEREADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <vector>
#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ipc {

/**
 * PipeReader class prototype.
 *
 * @author zer0
 * @date   2020-03-21
 */
class TBAG_API PipeReader : private Noncopyable
{
public:
    struct Impl;

public:
    using Uniquempl = std::unique_ptr<Impl>;
    using ufile = libtbag::uvpp::ufile;

    /**
     * Return value:
     *  - true: Continue.
     *  - false: Exit signal.
     */
    using Callback = std::function<bool(char const *, int)>;

private:
    Uniquempl _impl;

public:
    PipeReader();
    PipeReader(ufile f, Callback const & cb);
    ~PipeReader();

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    Err start(ufile f, Callback const & cb);
    Err asyncRaise();
    void done();
};

} // namespace ipc

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_IPC_PIPEREADER_HPP__

