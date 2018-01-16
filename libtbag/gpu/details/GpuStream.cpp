/**
 * @file   GpuStream.cpp
 * @brief  GpuStream class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/log/Log.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuStream::GpuStream(GpuContext const * c, GpuId i) : GpuIdWrapper(c, i)
{
    // EMPTY.
}

GpuStream::GpuStream(GpuStream const & obj) : GpuStream()
{
    (*this) = obj;
}

GpuStream::GpuStream(GpuStream && obj) : GpuStream()
{
    (*this) = std::move(obj);
}

GpuStream::~GpuStream()
{
    release();
}

GpuStream & GpuStream::operator =(GpuStream const & obj)
{
    if (this != &obj) {
        GpuIdWrapper::assign(obj);
    }
    return *this;
}

GpuStream & GpuStream::operator =(GpuStream && obj)
{
    GpuStream::swap(obj);
    return *this;
}

void GpuStream::swap(GpuStream & obj)
{
    if (this != &obj) {
        GpuIdWrapper::swap(obj);
    }
}

Err GpuStream::create()
{
    if (validate()) {
        return Err::E_ALREADY;
    }
    return (_context != nullptr ? _context->createStream(*this) : Err::E_NULLPTR);
}

Err GpuStream::release()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return (_context != nullptr ? _context->releaseStream(*this) : Err::E_NULLPTR);
}

Err GpuStream::flush()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return (_context != nullptr ? _context->flush(*this) : Err::E_NULLPTR);
}

Err GpuStream::finish()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return (_context != nullptr ? _context->finish(*this) : Err::E_NULLPTR);
}

GpuStream GpuStream::instance(GpuContext const * c)
{
    if (c == nullptr) {
        return GpuStream();
    }

    GpuStream stream(c);
    if (isSuccess(stream.create())) {
        return stream;
    }
    return GpuStream();
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

