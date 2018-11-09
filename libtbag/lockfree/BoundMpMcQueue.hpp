/**
 * @file   BoundMpMcQueue.hpp
 * @brief  BoundMpMcQueue class prototype.
 * @author zer0
 * @date   2018-11-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_BOUNDMPMCQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_BOUNDMPMCQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/tmp/Power.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lockfree {

/**
 * BoundMpMcQueue class prototype.
 *
 * @author zer0
 * @date   2018-11-09
 *
 * @remarks
 *  Bounded, Many-Producer, Many-consumer Queue.
 */
class TBAG_API BoundMpMcQueue : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_QUEUE_SIZE = 1024;

public:
    static_assert(libtbag::tmp::is_power_of_2<DEFAULT_QUEUE_SIZE>::value,
                  "Must be a positive integer power of 2");

private:
    UniqueImpl _impl;

public:
    BoundMpMcQueue(std::size_t power_of_2_size = DEFAULT_QUEUE_SIZE);
    virtual ~BoundMpMcQueue();

public:
    bool enqueue(void * value);
    bool dequeue(void ** value);
};

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_BOUNDMPMCQUEUE_HPP__

