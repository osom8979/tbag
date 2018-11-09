/**
 * @file   BoundMpMcQueue.cpp
 * @brief  BoundMpMcQueue class implementation.
 * @author zer0
 * @date   2018-11-09
 */

#include <libtbag/lockfree/BoundMpMcQueue.hpp>
#include <libtbag/bitwise/BitHacks.hpp>
#include <libtbag/memory/AlignedMemory.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <vector>

extern "C" {
#include <liblfds711.h>
}

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
struct BoundMpMcQueue::Impl : private Noncopyable
{
public:
    using State    = lfds711_queue_bmm_state;
    using Element  = lfds711_queue_bmm_element;
    using Elements = std::vector<Element>;

private:
    BoundMpMcQueue * _parent;

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
    Impl(BoundMpMcQueue * parent, std::size_t size) : _parent(parent), _state(nullptr), _elements(size)
    {
        assert(_parent != nullptr);
        if (!libtbag::bitwise::isPowerOf2(size)) {
            tDLogE("BoundMpMcQueue::Impl::Impl() Must be a positive integer power of 2");
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

    bool enqueue(void * value)
    {
        return enqueue(nullptr, value);
    }

    bool enqueueKey(void * key)
    {
        return enqueue(key, nullptr);
    }

public:
    bool dequeue(void ** key, void ** value)
    {
        // Returns 1 on a successful dequeue.
        // Returns 0 if dequeing failed.
        // Dequeuing only fails if the queue is empty.
        return lfds711_queue_bmm_dequeue(_state, key, value) == 1;
    }

    bool dequeue(void ** value)
    {
        return dequeue(nullptr, value);
    }

    bool dequeueKey(void ** key)
    {
        return dequeue(key, nullptr);
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

// ------------------------------
// BoundMpMcQueue implementation.
// ------------------------------

BoundMpMcQueue::BoundMpMcQueue(std::size_t power_of_2_size)
        : _impl(std::make_unique<Impl>(this, power_of_2_size))
{
    assert(static_cast<bool>(_impl));
}

BoundMpMcQueue::~BoundMpMcQueue()
{
    // EMPTY.
}

std::size_t BoundMpMcQueue::potentially_inaccurate_count() const
{
    assert(static_cast<bool>(_impl));
    return _impl->potentially_inaccurate_count();
}

BoundMpMcQueue::MiscValidity BoundMpMcQueue::singlethreaded_validate(
        std::size_t min_elements, std::size_t max_elements) const
{
    assert(static_cast<bool>(_impl));
    return _impl->singlethreaded_validate(min_elements, max_elements);
}

bool BoundMpMcQueue::enqueue(void * key, void * value)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue(key, value);
}

bool BoundMpMcQueue::enqueue(void * value)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue(value);
}

bool BoundMpMcQueue::enqueueKey(void * key)
{
    assert(static_cast<bool>(_impl));
    return _impl->enqueue(key);
}

bool BoundMpMcQueue::dequeue(void ** key, void ** value)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue(key, value);
}

bool BoundMpMcQueue::dequeue(void ** value)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue(value);
}

bool BoundMpMcQueue::dequeueKey(void ** key)
{
    assert(static_cast<bool>(_impl));
    return _impl->dequeue(key);
}

void BoundMpMcQueue::onCleanup(void * key, void * value)
{
    assert(static_cast<bool>(_impl));
}

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

