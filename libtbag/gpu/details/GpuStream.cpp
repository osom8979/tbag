/**
 * @file   GpuStream.cpp
 * @brief  GpuStream class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/SyncedMemory.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuStream::GpuStream(GpuContext const & context) : GpuIdWrapper(context)
{
    if (isFailure(context.createStream(*this))) {
        throw std::bad_alloc();
    }
}

GpuStream::~GpuStream()
{
    if (validate()) {
        atContext().releaseStream(*this);
    }
}

Err GpuStream::flush() const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().flush(*this);
}

Err GpuStream::finish() const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().finish(*this);
}

Err GpuStream::fill(SyncedMemory & out, AnyPod data, int count, GpuEvent * event)
{
    auto const RESULT_TYPE = out.type();
    if (checkGpuTypeTable(RESULT_TYPE) == false) {
        return Err::E_ILLSTATE;
    }
    if (RESULT_TYPE != data.type() || out.countOfElem() < count) {
        return Err::E_ILLARGS;
    }

    void * gpu_out = out.getMutableGpuData();

    // @formatter:off
    switch (RESULT_TYPE) {
    case TypeTable::TT_INT:    return atContext().fill(*this, (int     *)gpu_out, data.cast<int     >(), count, event);
    case TypeTable::TT_UINT:   return atContext().fill(*this, (unsigned*)gpu_out, data.cast<unsigned>(), count, event);
    case TypeTable::TT_FLOAT:  return atContext().fill(*this, (float   *)gpu_out, data.cast<float   >(), count, event);
    case TypeTable::TT_DOUBLE: return atContext().fill(*this, (double  *)gpu_out, data.cast<double  >(), count, event);
    default: return Err::E_UNSUPOP;
    }
    // @formatter:on
}

Err GpuStream::add(SyncedMemory const & in1, SyncedMemory const & in2, SyncedMemory & out, int count, GpuEvent * event)
{
    auto const RESULT_TYPE = out.type();
    if (checkGpuTypeTable(RESULT_TYPE) == false) {
        return Err::E_ILLSTATE;
    }
    if (in1.type() != RESULT_TYPE || in2.type() != RESULT_TYPE) {
        return Err::E_ILLARGS;
    }

    void const * gpu_in1 = in1.getGpuData();
    void const * gpu_in2 = in2.getGpuData();
    void * gpu_out = out.getMutableGpuData();

    // @formatter:off
    switch (RESULT_TYPE) {
    case TypeTable::TT_INT:    return atContext().add(*this, (int      const *)gpu_in1, (int      const *)gpu_in2, (int     *)gpu_out, count, event);
    case TypeTable::TT_UINT:   return atContext().add(*this, (unsigned const *)gpu_in1, (unsigned const *)gpu_in2, (unsigned*)gpu_out, count, event);
    case TypeTable::TT_FLOAT:  return atContext().add(*this, (float    const *)gpu_in1, (float    const *)gpu_in2, (float   *)gpu_out, count, event);
    case TypeTable::TT_DOUBLE: return atContext().add(*this, (double   const *)gpu_in1, (double   const *)gpu_in2, (double  *)gpu_out, count, event);
    default: return Err::E_UNSUPOP;
    }
    // @formatter:on
}

GpuStream * GpuStream::newInstance(GpuContext const & context)
{
    return new GpuStream(context);
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

