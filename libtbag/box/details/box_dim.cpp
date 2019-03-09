/**
 * @file   box_dim.cpp
 * @brief  box dim implementation.
 * @author zer0
 * @date   2019-03-04
 */

#include <libtbag/box/details/box_dim.hpp>
#include <libtbag/memory/Memory.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

ui32 * box_dim_malloc(ui32 rank) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    return (ui32*)tbMalloc(sizeof(ui32) * rank);
}

ui32 * box_dim_malloc_args(ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    va_list ap;
    va_start(ap, rank);
    auto * dims = box_dim_malloc_vargs(rank, ap);
    assert(dims != nullptr);
    va_end(ap);
    return dims;
}

ui32 * box_dim_malloc_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    auto * dims = box_dim_malloc(rank);
    assert(dims != nullptr);
    box_dim_set_vargs(dims, rank, ap);
    return dims;
}

void box_dim_free(ui32 * dims) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    tbFree(dims);
}

void box_dim_set_args(ui32 * TBAG_RESTRICT dims, ui32 args_count, ...) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    va_list ap;
    va_start(ap, args_count);
    box_dim_set_vargs(dims, args_count, ap);
    va_end(ap);
}

void box_dim_set_vargs(ui32 * TBAG_RESTRICT dims, ui32 args_count, va_list ap) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    va_list ap2;
    va_copy(ap2, ap);
    for (ui32 i = 0; i < args_count; ++i) {
        dims[i] = va_arg(ap2, ui32);
    }
    va_end(ap2);
}

void box_dim_copy(ui32 * dest, ui32 const * src, ui32 rank) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    assert(rank >= 1);
    while (rank) {
        *dest = *src;
        ++dest;
        ++src;
        --rank;
    }
}

ui32 * box_dim_clone(ui32 const * src, ui32 rank) TBAG_NOEXCEPT
{
    assert(src != nullptr);
    assert(rank >= 1);
    ui32 * result = box_dim_malloc(rank);
    assert(result != nullptr);
    box_dim_copy(result, src, rank);
    return result;
}

bool box_dim_is_equals(ui32 const * dims1, ui32 rank1, ui32 const * dims2, ui32 rank2) TBAG_NOEXCEPT
{
    assert(dims1 != nullptr);
    assert(dims2 != nullptr);
    if (rank1 != rank2) {
        return false;
    }
    assert(rank1 == rank2);
    if (dims1 == dims2) {
        return true;
    }
    assert(dims1 != dims2);
    for (ui32 i = 0; i < rank1; ++i) {
        if (dims1[i] != dims2[i]) {
            return false;
        }
    }
    return true;
}

ui32 box_dim_get_size(ui32 const * dims, ui32 rank) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    ui32 size = dims[0];
    for (std::size_t i = 1; i < rank; ++i) {
        size *= dims[i];
    }
    return size;
}

ui32 box_dim_get_index_args(ui32 const * dims, ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    va_list ap;
    va_start(ap, rank);
    auto const INDEX = box_dim_get_index_vargs(dims, rank, ap);
    va_end(ap);
    return INDEX;
}

ui32 box_dim_get_index_vargs(ui32 const * dims, ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);

    // 1rank: x
    // 2rank: x + (y * width)
    // 3rank: x + (y * width) + (z * width * height)
    // 4rank: x + (y * width) + (z * width * height) + (w * width * height * depth)
    // ...
    // 4rank: x + ((y + (z + (w * depth)) * height) * width)
    // 3rank: x + (y + (z * height)) * width
    // 2rank: x + (y * width)
    // 1rank: x

    va_list ap2;
    va_copy(ap2, ap);
    ui32 index = 0;
    while (rank >= 2) {
        index = (va_arg(ap2, ui32) + index) * dims[rank-2];
        --rank;
    }
    assert(rank == 1);
    index += va_arg(ap2, ui32);
    va_end(ap2);
    return index;
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

