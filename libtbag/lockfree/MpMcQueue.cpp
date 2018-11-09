/**
 * @file   MpMcQueue.cpp
 * @brief  MpMcQueue class implementation.
 * @author zer0
 * @date   2018-11-09
 */

#include <libtbag/lockfree/MpMcQueue.hpp>
#include <libtbag/memory/AlignedMemory.hpp>

#include <cassert>

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
struct MpMcQueue::Impl : private Noncopyable
{
public:
    using State   = lfds711_queue_umm_state;
    using Element = lfds711_queue_umm_element;
    using Flag    = lfds711_misc_flag;

public:
    struct Data
    {
        lfds711_queue_umm_element queue_element;
        void * data = nullptr;
    };

public:
    std::size_t const SIZE;

private:
    State   * _state = nullptr;
    Element * _dummy = nullptr;

public:
    Impl(std::size_t size) : SIZE(size)
    {
        using namespace libtbag::memory;
        _state = (State   *) alignedMemoryAlloc(sizeof(State)  , LFDS711_PAL_ATOMIC_ISOLATION_IN_BYTES);
        _dummy = (Element *) alignedMemoryAlloc(sizeof(Element), LFDS711_PAL_ATOMIC_ISOLATION_IN_BYTES);
        assert(_state != nullptr);
        assert(_dummy != nullptr);
        lfds711_queue_umm_init_valid_on_current_logical_core(_state, _dummy, this);
    }

    ~Impl()
    {
        lfds711_queue_umm_cleanup(_state, &__element_cleanup_cb__);
        using namespace libtbag::memory;
        assert(_dummy != nullptr);
        assert(_state != nullptr);
        alignedMemoryFree(_dummy);
        alignedMemoryFree(_state);
    }

private:
    static void __element_cleanup_cb__(State * qumms, Element * qumme, Flag flag)
    {
        // EMPTY.
    }
};

// -------------------------
// MpMcQueue implementation.
// -------------------------

MpMcQueue::MpMcQueue(std::size_t size) : _impl(std::make_unique<Impl>(size))
{
    // EMPTY.
}

MpMcQueue::~MpMcQueue()
{
    // EMPTY.
}

bool MpMcQueue::enqueue(void * value)
{
    return false;
}

bool MpMcQueue::dequeue(void ** value)
{
    return false;
}

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

