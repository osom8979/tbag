/**
 * @file   BoundedMpMcQueue.cpp
 * @brief  BoundedMpMcQueue class implementation.
 * @author zer0
 * @date   2018-11-09
 */

#include <libtbag/lockfree/BoundedMpMcQueue.hpp>
#include <libtbag/bitwise/BitHacks.hpp>
#include <libtbag/memory/AlignedMemory.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Err.hpp>

#include <cassert>
#include <cmath>
#include <vector>

extern "C" {
#include <liblfds711.h>
}

#include <atomic>
#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lockfree {

/**
 * liblfds backend.
 *
 * @author zer0
 * @date   2018-11-09
 */
struct BoundedMpMcQueue::Impl : private Noncopyable
{
public:
    using State    = lfds711_queue_bmm_state;
    using Element  = lfds711_queue_bmm_element;
    using Elements = std::vector<Element>;

private:
    BoundedMpMcQueue * _parent;

private:
    /**
     * A pointer to a user-allocated LFDS711_PAL_ATOMIC_ISOLATION_IN_BYTES aligned struct lfds711_queue_bmm_state.
     * Stack declared variables will automatically be correctly aligned by the compiler,
     * due to the information in the structure definitions; nothing has to be done.
     * Heap allocated variables however will by no means be correctly aligned and an aligned malloc must be used.
     */
    State * _state;

    /**
     * A pointer to a user-allocated array of struct lfds711_queue_bmm_element.
     * There are no alignment requirements for this allocation.
     */
    Elements _elements;

public:
    Impl(BoundedMpMcQueue * parent, std::size_t size) : _parent(parent), _state(nullptr), _elements(size)
    {
        assert(_parent != nullptr);
        if (!libtbag::bitwise::isPowerOf2(size)) {
            tDLogE("BoundedMpMcQueue::Impl::Impl() Must be a positive integer power of 2");
            throw std::bad_alloc();
        }

        using namespace libtbag::memory;
        _state = (State*) alignedMemoryAlloc(sizeof(State), LFDS711_PAL_ATOMIC_ISOLATION_IN_BYTES);
        assert(_state != nullptr);
        assert(size == _elements.size());
        lfds711_queue_bmm_init_valid_on_current_logical_core(_state, _elements.data(), size, this);
    }

    ~Impl()
    {
        lfds711_queue_bmm_cleanup(_state, &__element_cleanup_cb__);
        _elements.clear();

        using namespace libtbag::memory;
        assert(_state != nullptr);
        alignedMemoryFree(_state);
    }

public:
    std::size_t potentially_inaccurate_count()
    {
        lfds711_pal_uint_t result = 0;
        lfds711_queue_bmm_query(_state, LFDS711_QUEUE_BMM_QUERY_GET_POTENTIALLY_INACCURATE_COUNT,
                                nullptr, &result);
        return static_cast<std::size_t>(result);
    }

    MiscValidity singlethreaded_validate(std::size_t min_elements = 0, std::size_t max_elements = 0)
    {
        lfds711_misc_validity result;
        if (min_elements == 0 && max_elements == 0) {
            lfds711_queue_bmm_query(_state, LFDS711_QUEUE_BMM_QUERY_SINGLETHREADED_VALIDATE, nullptr, &result);
        } else {
            // which specifies an expected min/max count,
            // in which case validation also counts the number of elements and check they fall within the specified range.
            lfds711_misc_validation_info info = {0,};
            info.min_elements = min_elements;
            info.max_elements = max_elements;
            lfds711_queue_bmm_query(_state, LFDS711_QUEUE_BMM_QUERY_SINGLETHREADED_VALIDATE, &info, &result);
        }

        switch (result) {
        // @formatter:off
        case LFDS711_MISC_VALIDITY_UNKNOWN:                         return MiscValidity::MV_UNKNOWN                       ;
        case LFDS711_MISC_VALIDITY_VALID:                           return MiscValidity::MV_VALID                         ;
        case LFDS711_MISC_VALIDITY_INVALID_LOOP:                    return MiscValidity::MV_INVALID_LOOP                  ;
        case LFDS711_MISC_VALIDITY_INVALID_MISSING_ELEMENTS:        return MiscValidity::MV_INVALID_MISSING_ELEMENTS      ;
        case LFDS711_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS:     return MiscValidity::MV_INVALID_ADDITIONAL_ELEMENTS   ;
        case LFDS711_MISC_VALIDITY_INVALID_TEST_DATA:               return MiscValidity::MV_INVALID_TEST_DATA             ;
        case LFDS711_MISC_VALIDITY_INVALID_ORDER:                   return MiscValidity::MV_INVALID_ORDER                 ;
        case LFDS711_MISC_VALIDITY_INVALID_ATOMIC_FAILED:           return MiscValidity::MV_INVALID_ATOMIC_FAILED         ;
        case LFDS711_MISC_VALIDITY_INDETERMINATE_NONATOMIC_PASSED:  return MiscValidity::MV_INDETERMINATE_NONATOMIC_PASSED;
        // @formatter:on
        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            return MiscValidity::MV_ERROR;
        }
    }

public:
    bool enqueue(void * key, void * value)
    {
        // Returns 1 on a successful enqueue.
        // Returns 0 if enqueing failed.
        // Enqueuing fails if the queue is full.
        return lfds711_queue_bmm_enqueue(_state, key, value) == 1;
    }

public:
    bool dequeue(void ** key, void ** value)
    {
        // Returns 1 on a successful dequeue.
        // Returns 0 if dequeing failed.
        // Dequeuing only fails if the queue is empty.
        return lfds711_queue_bmm_dequeue(_state, key, value) == 1;
    }

private:
    struct {
        libtbag::lock::UvLock       mutex;
        libtbag::lock::UvCondition  signal;
    } __wait_feature__;

public:
    bool enqueue_and_signaling(void * key, void * value)
    {
        bool const result = enqueue(key, value);
        __wait_feature__.signal.signal();
        return result;
    }

    bool enqueue_and_wait_signaling(void * key, void * value)
    {
        while (!__wait_feature__.mutex.tryLock()) {
            // No-wait retry...
        }
        bool const result = enqueue(key, value);
        __wait_feature__.signal.signal();
        __wait_feature__.mutex.unlock();
        return result;
    }

    bool dequeue_and_signaling(void ** key, void ** value)
    {
        bool const result = dequeue(key, value);
        __wait_feature__.signal.broadcast();
        return result;
    }

    bool dequeue_and_wait_signaling(void ** key, void ** value)
    {
        while (!__wait_feature__.mutex.tryLock()) {
            // No-wait retry...
        }
        bool const result = dequeue(key, value);
        __wait_feature__.signal.broadcast();
        __wait_feature__.mutex.unlock();
        return result;
    }

    void wait_dequeue(void ** key, void ** value)
    {
        __wait_feature__.mutex.lock();
        while (!dequeue(key, value)) {
            __wait_feature__.signal.wait(__wait_feature__.mutex);
        }
        __wait_feature__.mutex.unlock();
    }

    void wait_dequeue(void ** key, void ** value, uint64_t wait_timeout_nano)
    {
        __wait_feature__.mutex.lock();
        while (!dequeue(key, value)) {
            __wait_feature__.signal.wait(__wait_feature__.mutex, wait_timeout_nano);
        }
        __wait_feature__.mutex.unlock();
    }

private:
    static void __element_cleanup_cb__(State * state, void * key, void * value)
    {
        assert(state != nullptr);
        assert(state->user_state != nullptr);

        auto * impl = (Impl*)state->user_state;
        assert(impl != nullptr);
        impl->_parent->onCleanup(key, value);
    }
};

// --------------------------------
// BoundedMpMcQueue implementation.
// --------------------------------

BoundedMpMcQueue::BoundedMpMcQueue(std::size_t power_of_2_size)
        : _impl(std::make_unique<Impl>(this, power_of_2_size))
{
    assert(static_cast<bool>(_impl));
}

BoundedMpMcQueue::~BoundedMpMcQueue()
{
    _impl.reset();
}

std::size_t BoundedMpMcQueue::calcMinimumQueueSize(std::size_t request_size)
{
    using namespace libtbag::bitwise;
    if (isPowerOf2(request_size)) {
        return request_size;
    }
    auto const RESULT_SIZE = static_cast<std::size_t>(pow(2, findMostSignificantBit(request_size) + 1));
    assert(isPowerOf2(RESULT_SIZE));
    assert(RESULT_SIZE >= request_size);
    return RESULT_SIZE;
}

std::size_t BoundedMpMcQueue::potentially_inaccurate_count() const
{
    assert(static_cast<bool>(_impl));
    return _impl->potentially_inaccurate_count();
}

BoundedMpMcQueue::MiscValidity BoundedMpMcQueue::singlethreaded_validate(
        std::size_t min_elements,
        std::size_t max_elements) const
{
    assert(static_cast<bool>(_impl));
    return _impl->singlethreaded_validate(min_elements, max_elements);
}

bool BoundedMpMcQueue::enqueue(void * key, void * value)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue(key, value);
}

bool BoundedMpMcQueue::enqueueAndSignaling(void * key, void * value)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue_and_signaling(key, value);
}

bool BoundedMpMcQueue::enqueueAndWaitSignaling(void * key, void * value)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue_and_wait_signaling(key, value);
}

bool BoundedMpMcQueue::enqueue(void * value)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue(nullptr, value);
}

bool BoundedMpMcQueue::enqueueAndSignaling(void * value)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue_and_signaling(nullptr, value);
}

bool BoundedMpMcQueue::enqueueAndWaitSignaling(void * value)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue_and_wait_signaling(nullptr, value);
}

bool BoundedMpMcQueue::dequeue(void ** key, void ** value)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue(key, value);
}

bool BoundedMpMcQueue::dequeueAndSignaling(void ** key, void ** value)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue_and_signaling(key, value);
}

bool BoundedMpMcQueue::dequeueAndWaitSignaling(void ** key, void ** value)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue_and_wait_signaling(key, value);
}

bool BoundedMpMcQueue::dequeue(void ** value)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue(nullptr, value);
}

bool BoundedMpMcQueue::dequeueAndSignaling(void ** value)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue_and_signaling(nullptr, value);
}

bool BoundedMpMcQueue::dequeueAndWaitSignaling(void ** value)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue_and_wait_signaling(nullptr, value);
}

void BoundedMpMcQueue::dequeueWait(void ** key, void ** value)
{
    assert(static_cast<bool>(_impl));
    _impl->wait_dequeue(key, value);
}

void BoundedMpMcQueue::dequeueWait(void ** key, void ** value, uint64_t wait_timeout_nano)
{
    assert(static_cast<bool>(_impl));
    _impl->wait_dequeue(key, value, wait_timeout_nano);
}

void BoundedMpMcQueue::onCleanup(void * key, void * value)
{
    // EMPTY.
}

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

