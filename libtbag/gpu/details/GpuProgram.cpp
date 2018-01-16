/**
 * @file   GpuProgram.cpp
 * @brief  GpuProgram class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuProgram.hpp>
#include <libtbag/log/Log.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuProgram::GpuProgram(GpuContext const * c, GpuId i) : GpuIdWrapper(c, i)
{
    // EMPTY.
}

GpuProgram::GpuProgram(GpuProgram const & obj) : GpuProgram()
{
    (*this) = obj;
}

GpuProgram::GpuProgram(GpuProgram && obj) : GpuProgram()
{
    (*this) = std::move(obj);
}

GpuProgram::~GpuProgram()
{
    // EMPTY.
}

GpuProgram & GpuProgram::operator =(GpuProgram const & obj)
{
    if (this != &obj) {
        GpuIdWrapper::assign(obj);
    }
    return *this;
}

GpuProgram & GpuProgram::operator =(GpuProgram && obj)
{
    GpuProgram::swap(obj);
    return *this;
}

void GpuProgram::swap(GpuProgram & obj)
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

