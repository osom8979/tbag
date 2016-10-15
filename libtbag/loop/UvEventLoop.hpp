/**
 * @file   UvEventLoop.hpp
 * @brief  UvEventLoop class prototype.
 * @author zer0
 * @date   2016-05-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

/**
 * UvEventLoop class prototype.
 *
 * @author zer0
 * @date   2016-05-18
 *
 * @remarks
 *  Use the libuv event loop.
 */
class UvEventLoop : public Noncopyable
{
public:
    struct HandlePimpl;

public:
    using UniqueHandle = std::unique_ptr<HandlePimpl>;

public:
    UniqueHandle _handle;

public:
    UvEventLoop() throw(InitializeException);
    ~UvEventLoop();

public:
    bool runDefault();

public:
    void * getNative();
    void const * getNative() const;
};

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTLOOP_HPP__

