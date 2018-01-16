/**
 * @file   GpuProgram.cpp
 * @brief  GpuProgram class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuProgram.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
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
    release();
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

Err GpuProgram::create(std::string const & source)
{
    if (validate()) {
        return Err::E_ALREADY;
    }
    return (_context != nullptr ? _context->createProgram(source, *this) : Err::E_NULLPTR);
}

Err GpuProgram::build()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return (_context != nullptr ? _context->buildProgram(*this) : Err::E_NULLPTR);
}

Err GpuProgram::release()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return (_context != nullptr ? _context->releaseProgram(*this) : Err::E_NULLPTR);
}

GpuProgram GpuProgram::instance(GpuContext const * c, std::string const & source)
{
    if (c == nullptr) {
        return GpuProgram();
    }

    GpuProgram program(c);
    if (isSuccess(program.create(source))) {
        return program;
    }
    return GpuProgram();
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

