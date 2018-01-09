/**
 * @file   AccelBackend.cpp
 * @brief  AccelBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/AccelBackend.hpp>
#include <libtbag/gpu/backend/GpuKernels.hpp>
#include <libtbag/gpu/accel/AccelRaw.hpp>

#include <libtbag/Err.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/algorithm/Pack.hpp>
#include <libtbag/memory/AlignedMemory.hpp>

#include <cstdlib>
#include <cassert>

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

    std::size_t const ALIGNED_SIZE = memory::getDefaultAlignedSize();
    std::size_t packed_size = 0;
    if (type == type::TypeTable::TT_FLOAT) {
        packed_size = algorithm::getPackedSize(sizeof(float) * count, ALIGNED_SIZE);
    } else if (type == type::TypeTable::TT_DOUBLE) {
        packed_size = algorithm::getPackedSize(sizeof(double) * count, ALIGNED_SIZE);
    } else {
        return false;
    }
    std::size_t const CAPACITY_SIZE = packed_size * ALIGNED_SIZE;
    if (    v1.capacity > CAPACITY_SIZE) { return false; }
    if (    v2.capacity > CAPACITY_SIZE) { return false; }
    if (result.capacity > CAPACITY_SIZE) { return false; }

    CpuEventGuard const EVENT_LOCK(event);
    if (type == type::TypeTable::TT_FLOAT) {
        return tbAccelAdd1f((float const *)v1.data, (float const *)v2.data, (float *)result.data, count) == TB_TRUE;
    } else if (type == type::TypeTable::TT_DOUBLE) {
        return tbAccelAdd1d((double const *)v1.data, (double const *)v2.data, (double *)result.data, count) == TB_TRUE;
    }
    return false;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

