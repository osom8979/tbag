/**
 * @file   MemoryWrapper.hpp
 * @brief  MemoryWrapper class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_MEMORYWRAPPER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_MEMORYWRAPPER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class GpuContext;

/**
 * MemoryWrapper class prototype.
 *
 * @author zer0
 * @date   2018-01-16
 */
class TBAG_API MemoryWrapper
{
protected:
    GpuContext const * _context;

protected:
    std::size_t _capacity;
    std::size_t _size;
    void *      _data;

public:
    MemoryWrapper(GpuContext const * c = nullptr);
    MemoryWrapper(MemoryWrapper const & obj);
    MemoryWrapper(MemoryWrapper && obj);
    ~MemoryWrapper();

public:
    MemoryWrapper & operator =(MemoryWrapper const & obj);
    MemoryWrapper & operator =(MemoryWrapper && obj);

public:
    void swap(MemoryWrapper & obj);
    inline friend void swap(MemoryWrapper & lh, MemoryWrapper & rh) { lh.swap(rh); }

public:
    inline bool isSameContext(GpuContext const * c) const TBAG_NOEXCEPT { return _context == c; }
    inline GpuContext const * getContextPtr() const TBAG_NOEXCEPT { return _context; }

    inline std::size_t capacity() const TBAG_NOEXCEPT { return _capacity; }
    inline std::size_t     size() const TBAG_NOEXCEPT { return     _size; }

    inline bool       exists() const TBAG_NOEXCEPT { return _data != nullptr && _capacity > 0; }
    inline void       * data()       TBAG_NOEXCEPT { return _data; }
    inline void const * data() const TBAG_NOEXCEPT { return _data; }

    template <typename T> inline T castData() const TBAG_NOEXCEPT { return (T)_data; }

    inline bool validate() const TBAG_NOEXCEPT
    { return (_context != nullptr) && exists(); }
    inline bool validate(GpuContext const * c) const TBAG_NOEXCEPT
    { return (_context != nullptr) && isSameContext(c) && exists(); }
};

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_MEMORYWRAPPER_HPP__

