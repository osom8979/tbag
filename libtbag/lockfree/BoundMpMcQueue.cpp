/**
 * @file   BoundMpMcQueue.cpp
 * @brief  BoundMpMcQueue class implementation.
 * @author zer0
 * @date   2018-11-09
 */

#include <libtbag/lockfree/BoundMpMcQueue.hpp>
#include <libtbag/bitwise/BitHacks.hpp>
#include <libtbag/memory/AlignedMemory.hpp>
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
    Impl(std::size_t size) : _state(nullptr), _elements(size)
    {
        if (!libtbag::bitwise::isPowerOf2(size)) {
            tDLogE("BoundMpMcQueue::Impl::Impl() Must be a positive integer power of 2");
            throw std::bad_alloc();
        }

        using namespace libtbag::memory;
        _state = (State*) alignedMemoryAlloc(sizeof(State), LFDS711_PAL_ATOMIC_ISOLATION_IN_BYTES);
        assert(_state != nullptr);
        assert(size == _elements.size());
        lfds711_queue_bmm_init_valid_on_current_logical_core(_state, _elements.data(), size, nullptr);
    }

    ~Impl()
    {
        lfds711_queue_bmm_cleanup(_state, &__element_cleanup_cb__);
        _elements.clear();

        using namespace libtbag::memory;
        assert(_state != nullptr);
        alignedMemoryFree(_state);
    }

private:
    static void __element_cleanup_cb__(State * qbmms, void * key, void * value)
    {
        // EMPTY.
    }
};

// ------------------------------
// BoundMpMcQueue implementation.
// ------------------------------

BoundMpMcQueue::BoundMpMcQueue(std::size_t power_of_2_size) : _impl(std::make_unique<Impl>(power_of_2_size))
{
    // EMPTY.
}

BoundMpMcQueue::~BoundMpMcQueue()
{
    // EMPTY.
}

bool BoundMpMcQueue::enqueue(void * value)
{
    return false;
}

bool BoundMpMcQueue::dequeue(void ** value)
{
    return false;
}

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

