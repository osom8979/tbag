/**
 * @file   GpuKernel.cpp
 * @brief  GpuKernel class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuKernel.hpp>
#include <libtbag/log/Log.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuKernel::GpuKernel(GpuContext const * c, GpuId i) : GpuIdWrapper(c, i)
{
    // EMPTY.
}

GpuKernel::GpuKernel(GpuKernel const & obj) : GpuKernel()
{
    (*this) = obj;
}

GpuKernel::GpuKernel(GpuKernel && obj) : GpuKernel()
{
    (*this) = std::move(obj);
}

GpuKernel::~GpuKernel()
{
    // EMPTY.
}

GpuKernel & GpuKernel::operator =(GpuKernel const & obj)
{
    if (this != &obj) {
        GpuIdWrapper::assign(obj);
    }
    return *this;
}

GpuKernel & GpuKernel::operator =(GpuKernel && obj)
{
    GpuKernel::swap(obj);
    return *this;
}

void GpuKernel::swap(GpuKernel & obj)
{
    if (this != &obj) {
        GpuIdWrapper::swap(obj);
    }
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

