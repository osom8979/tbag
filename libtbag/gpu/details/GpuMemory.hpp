/**
 * @file   GpuMemory.hpp
 * @brief  GpuMemory class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdlib>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class GpuContext;

/**
 * GpuMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuMemory
{
private:
    GpuContext const * _context;

private:
    std::size_t _capacity;
    std::size_t _size;
    void *      _data;

public:
    GpuMemory(GpuContext const * c = nullptr);
    GpuMemory(GpuMemory const & obj);
    GpuMemory(GpuMemory && obj);
    virtual ~GpuMemory();

public:
    GpuMemory & operator =(GpuMemory const & obj);
    GpuMemory & operator =(GpuMemory && obj);

public:
    void swap(GpuMemory & obj);
    inline friend void swap(GpuMemory & lh, GpuMemory & rh) { lh.swap(rh); }

public:
    inline bool isSameContext(GpuContext const * c) const TBAG_NOEXCEPT { return _context == c; }
    inline GpuContext const * getContextPtr() const TBAG_NOEXCEPT { return _context; }

    inline std::size_t capacity() const TBAG_NOEXCEPT { return _capacity; }
    inline std::size_t     size() const TBAG_NOEXCEPT { return     _size; }

    inline void set(void * d, std::size_t c, std::size_t s) TBAG_NOEXCEPT { _data = d; _capacity = c; _size = s; }
    inline void clear() TBAG_NOEXCEPT { _data = nullptr; _capacity = 0; _size = 0; }

    inline bool       exists() const TBAG_NOEXCEPT { return _data != nullptr && _capacity > 0; }
    inline void       * data()       TBAG_NOEXCEPT { return _data; }
    inline void const * data() const TBAG_NOEXCEPT { return _data; }

    template <typename T> inline T castData() const TBAG_NOEXCEPT { return (T)_data; }

    inline bool validate(GpuContext const * c) const TBAG_NOEXCEPT
    { return (_context != nullptr) && isSameContext(c) && exists(); }
};

using GpuMemories = std::vector<GpuMemory>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__

