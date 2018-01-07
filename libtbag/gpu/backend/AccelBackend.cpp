/**
 * @file   AccelBackend.cpp
 * @brief  AccelBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/AccelBackend.hpp>
#include <libtbag/gpu/backend/GpuKernels.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/UvUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

GpuBackendType AccelBackend::getType() const TBAG_NOEXCEPT
{
    return GpuBackendType::GBT_ACCEL;
}

bool AccelBackend::runAdd(GpuStream & stream, GpuMemory const & v1, GpuMemory const & v2, GpuMemory & result,
                          type::TypeTable type, int count, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(v1.type);
    checkType(v2.type);
    checkType(result.type);

    CpuEventGuard const EVENT_LOCK(event);
    if (type == type::TypeTable::TT_FLOAT) {
        return kernels::addByAccel1f((float const *)v1.data, (float const *)v2.data, (float *)result.data, count);
    } else if (type == type::TypeTable::TT_DOUBLE) {
        return kernels::addByAccel1d((double const *)v1.data, (double const *)v2.data, (double *)result.data, count);
    }
    return false;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

