/**
 * @file   HostMemory.cpp
 * @brief  HostMemory class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/HostMemory.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

HostMemory::HostMemory(GpuContext const * c)
        : GpuMemory(c), _flag(HostMemoryFlag::HMF_DEFAULT)
{
    // EMPTY.
}

HostMemory::HostMemory(GpuMemory const & mem)
        : GpuMemory(mem), _flag(HostMemoryFlag::HMF_DEFAULT)
{
    // EMPTY.
}

HostMemory::HostMemory(HostMemory const & obj) : HostMemory()
{
    (*this) = obj;
}

HostMemory::HostMemory(HostMemory && obj) : HostMemory()
{
    (*this) = std::move(obj);
}

HostMemory::~HostMemory()
{
    // EMPTY.
}

HostMemory & HostMemory::operator =(HostMemory const & obj)
{
    if (this != &obj) {
        GpuMemory::operator=(obj);
        _flag = obj._flag;
    }
    return *this;
}

HostMemory & HostMemory::operator =(HostMemory && obj)
{
    HostMemory::swap(obj);
    return *this;
}

void HostMemory::swap(HostMemory & obj)
{
    if (this != &obj) {
        GpuMemory::swap(obj);
        std::swap(_flag, obj._flag);
    }
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

