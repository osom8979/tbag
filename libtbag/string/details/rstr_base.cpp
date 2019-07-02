/**
 * @file   rstr_base.cpp
 * @brief  rstr_base class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/string/details/rstr_base.hpp>
#include <libtbag/memory/Memory.hpp>
#include <libtbag/algorithm/Pack.hpp>

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string  {
namespace details {

void rstr_base_clear(rstr_base * s) TBAG_NOEXCEPT
{
    s->flags = 0;
    s->capacity = 0;
    s->size = 0;
    s->data = nullptr;
}

void rstr_base_malloc(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(capacity >= 1);
    if (flags & RSTR_FLAG_ALIGN) {
        s->flags = RSTR_FLAG_ALIGN;
        s->data = (char*)tbAlignedMalloc(sizeof(char)*capacity, tbDefaultAlignSize());
    } else {
        s->flags = 0;
        s->data = (char*)tbMalloc(sizeof(char)*capacity);
    }
    s->capacity = capacity;
    s->size = 0;
}

void rstr_base_free(rstr_base * s) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    if (s->capacity <= 0) {
        return;
    }

    assert(s->data);
    if (s->flags & RSTR_FLAG_ALIGN) {
        tbAlignedFree(s->data);
    } else {
        tbFree(s->data);
    }
    s->capacity = 0;
    s->size = 0;
    s->data = nullptr;
}

void rstr_base_copy_string(rstr_base * s, char const * src, int size) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(s->data != nullptr);
    assert(src != nullptr);
    assert(size >= 1);
    assert(WITH_NULLSIZE(size) <= s->capacity);

    ::memcpy(s->data, src, sizeof(char)*size);
    s->data[size] = '\0';
    s->size = size;
}

void rstr_base_capacity_extend(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(capacity >= 1);

    if (capacity <= s->capacity) {
        return;
    }

    rstr_base temp = *s;
    assert(capacity > temp.capacity);

    rstr_base_malloc(s, capacity, flags);
    assert(s->capacity == capacity);
    assert(s->data == nullptr);
    assert(s->size == 0);

    // Restore original data.
    if (temp.size >= 1) {
        assert(temp.data != nullptr);
        assert(temp.capacity >= temp.size);
        assert(WITH_NULLSIZE(temp.size) <= capacity);
        rstr_base_copy_string(s, temp.data, temp.size);
    }

    // Free original memory.
    if (temp.capacity >= 1) {
        assert(temp.data != nullptr);
        rstr_base_free(&temp);
    }
}

int rstr_base_get_recommand_capacity(int capacity) TBAG_NOEXCEPT
{
    if (capacity <= RSTR_SMALL_CAPACITY) {
        return RSTR_SMALL_CAPACITY;
    }
    if (capacity <= RSTR_MEDIUM_CAPACITY) {
        return RSTR_MEDIUM_CAPACITY;
    }
    if (capacity <= RSTR_LARGE_CAPACITY) {
        return RSTR_LARGE_CAPACITY;
    }
    if (capacity <= RSTR_VLARGE_CAPACITY) {
        return RSTR_VLARGE_CAPACITY;
    }
    return RSTR_STEP_CAPACITY * libtbag::algorithm::getPackedSize(capacity, RSTR_STEP_CAPACITY);
}

void rstr_base_recommand_capacity_extend(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT
{
    rstr_base_capacity_extend(s, rstr_base_get_recommand_capacity(WITH_NULLSIZE(capacity)), flags);
}

void rstr_base_assign(rstr_base * s, char const * src, int size, int flag) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(src != nullptr);
    assert(size >= 1);
    s->size = 0;
    rstr_base_recommand_capacity_extend(s, size, flag);
    rstr_base_copy_string(s, src, size);
}

void rstr_base_append(rstr_base * s, char const * src, int size, int flag) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(src != nullptr);
    assert(size >= 1);

    if (s->size == 0) {
        rstr_base_assign(s, src, size, flag);
        return;
    }

    auto const MIN_SIZE = WITH_NULLSIZE(s->size+size);
    rstr_base_recommand_capacity_extend(s, MIN_SIZE, flag);
    assert(MIN_SIZE <= s->capacity);

    ::memcpy(s->data + s->size, src, sizeof(char)*size);
    s->size += size;
    s->data[s->size] = '\0';
}

} // namespace details
} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

