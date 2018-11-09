/**
 * @file   MpMcQueue.hpp
 * @brief  MpMcQueue class prototype.
 * @author zer0
 * @date   2018-11-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_MPMCQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_MPMCQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lockfree {

/**
 * MpMcQueue class prototype.
 *
 * @author zer0
 * @date   2018-11-09
 *
 * @remarks
 *  Unbounded, Many-Producer, Many-consumer Queue.
 */
class TBAG_API MpMcQueue : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    MpMcQueue(std::size_t size);
    ~MpMcQueue();

public:
    bool enqueue(void * value);
    bool dequeue(void ** value);
};

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_MPMCQUEUE_HPP__

