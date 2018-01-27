/**
 * @file   GpuKernel.cpp
 * @brief  GpuKernel class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuKernel.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// --------------------------
// GpuProgram implementation.
// --------------------------

GpuProgram::GpuProgram(GpuContext const & context, std::string const & source)
        : GpuIdWrapper(context)
{
    if (isFailure(atContext().createProgram(source, *this))) {
        throw std::bad_alloc();
    }
}

GpuProgram::~GpuProgram()
{
    if (validate()) {
        atContext().releaseProgram(*this);
    }
}

Err GpuProgram::build()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().buildProgram(*this);
}

GpuProgram * GpuProgram::newInstance(GpuContext const & context, std::string const & source)
{
    return new GpuProgram(context, source);
}

// -------------------------
// GpuKernel implementation.
// -------------------------

GpuKernel::GpuKernel(GpuProgram const & program, std::string const & kernel_symbol)
        : GpuIdWrapper(program.atContext()), _name(kernel_symbol)
{
    if (isFailure(atContext().createKernel(program, kernel_symbol, *this))) {
        throw std::bad_alloc();
    }
}

GpuKernel::~GpuKernel()
{
    atContext().releaseKernel(*this);
}

GpuKernel * GpuKernel::newInstance(GpuProgram const & program, std::string const & kernel_symbol)
{
    return new GpuKernel(program, kernel_symbol);
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

