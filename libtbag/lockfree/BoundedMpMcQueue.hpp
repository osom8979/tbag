/**
 * @file   BoundedMpMcQueue.hpp
 * @brief  BoundedMpMcQueue class prototype.
 * @author zer0
 * @date   2018-11-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_BOUNDEDMPMCQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_BOUNDEDMPMCQUEUE_HPP__

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
 * BoundedMpMcQueue class prototype.
 *
 * @author zer0
 * @date   2018-11-09
 *
 * @remarks
 *  Bounded, Many-Producer, Many-consumer Queue.
 */
class TBAG_API BoundedMpMcQueue : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    enum class MiscValidity
    {
        MV_ERROR,
        MV_UNKNOWN,
        MV_VALID,
        MV_INVALID_LOOP,
        MV_INVALID_MISSING_ELEMENTS,
        MV_INVALID_ADDITIONAL_ELEMENTS,
        MV_INVALID_TEST_DATA,
        MV_INVALID_ORDER,
        MV_INVALID_ATOMIC_FAILED,
        MV_INDETERMINATE_NONATOMIC_PASSED,
    };

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
    BoundedMpMcQueue(std::size_t power_of_2_size = DEFAULT_QUEUE_SIZE);
    virtual ~BoundedMpMcQueue();

public:
    static std::size_t calcMinimumQueueSize(std::size_t request_size);

public:
    std::size_t potentially_inaccurate_count() const;
    MiscValidity singlethreaded_validate(std::size_t min_elements = 0, std::size_t max_elements = 0) const;

public:
    bool enqueue(void * key, void * value);
    bool enqueueVal(void * value);
    bool enqueueKey(void * key);

public:
    bool dequeue(void ** key, void ** value);
    bool dequeueVal(void ** value);
    bool dequeueKey(void ** key);

protected:
    virtual void onCleanup(void * key, void * value);
};

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_BOUNDEDMPMCQUEUE_HPP__

