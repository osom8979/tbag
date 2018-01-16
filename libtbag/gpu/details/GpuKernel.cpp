/**
 * @file   GpuKernel.cpp
 * @brief  GpuKernel class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuKernel.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/GpuProgram.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuKernel::GpuKernel(GpuContext const * c, GpuId i) : GpuIdWrapper(c, i), _name()
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
    release();
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

Err GpuKernel::create(GpuProgram const & program, std::string const & kernel_symbol)
{
    if (validate()) {
        return Err::E_ALREADY;
    }
    if (_context == nullptr) {
        return Err::E_NULLPTR;
    }
    assert(_context != nullptr);

    Err const CODE = _context->createKernel(program, kernel_symbol, *this);
    if (isSuccess(CODE)) {
        _name = kernel_symbol;
    } else {
        _name.clear();
    }
    return CODE;
}

Err GpuKernel::release()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    if (_context == nullptr) {
        return Err::E_NULLPTR;
    }
    assert(_context != nullptr);

    _name.clear();
    return _context->releaseKernel(*this);
}

GpuKernel GpuKernel::instance(GpuProgram const & program, std::string const & kernel_symbol)
{
    GpuKernel kernel(program.getContextPtr());
    if (isSuccess(kernel.create(program, kernel_symbol))) {
        return kernel;
    }
    return GpuKernel();
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

