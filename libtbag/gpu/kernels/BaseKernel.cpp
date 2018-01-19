/**
 * @file   BaseKernel.cpp
 * @brief  BaseKernel class implementation.
 * @author zer0
 * @date   2018-01-19
 */

#include <libtbag/gpu/kernels/BaseKernel.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace kernels {

BaseKernel::BaseKernel(GpuStream const & stream) : _stream(stream), _ki(), _ku(), _kf(), _kd()
{
    // EMPTY.
}

BaseKernel::~BaseKernel()
{
    // EMPTY.
}

bool BaseKernel::isSupport(TypeTable type) const TBAG_NOEXCEPT
{
    return type == TypeTable::TT_INT
           || type == TypeTable::TT_UINT
           || type == TypeTable::TT_FLOAT
           || type == TypeTable::TT_DOUBLE;
}

GpuType BaseKernel::getType() const TBAG_NOEXCEPT
{
    return _stream.atContext().getType();
}

SharedGpuKernel BaseKernel::createKernel(std::string const & source, std::string const & symbol)
{
    SharedGpuKernel kernel;
    try {
        GpuProgram program(_stream.atContext(), source);
        if (isSuccess(program.build())) {
            kernel.reset(new GpuKernel(program, symbol));
        }
    } catch (...) {
        // EMPTY.
    }
    return kernel;
}

} // namespace kernels
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

