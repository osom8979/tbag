/**
 * @file   BaseKernel.hpp
 * @brief  BaseKernel class prototype.
 * @author zer0
 * @date   2018-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_KERNELS_BASEKERNEL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_KERNELS_BASEKERNEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/gpu/SyncedMemory.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace kernels {

/**
 * BaseKernel class prototype.
 *
 * @author zer0
 * @date   2018-01-19
 */
class TBAG_API BaseKernel : private Noncopyable
{
public:
    template <typename T>
    using TypeInfo  = type::TypeInfo<T>;
    using TypeTable = type::TypeTable;

protected:
    GpuStream const & _stream;

protected:
    SharedGpuKernel _ki;
    SharedGpuKernel _ku;
    SharedGpuKernel _kf;
    SharedGpuKernel _kd;
    SharedGpuEvent  _event;

public:
    BaseKernel(GpuStream const & stream);
    virtual ~BaseKernel();

public:
    bool isSupport(TypeTable type) const TBAG_NOEXCEPT;

public:
    GpuType getType() const TBAG_NOEXCEPT;

public:
    SharedGpuKernel createKernel(std::string const & source, std::string const & symbol);
};

} // namespace kernels
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_KERNELS_BASEKERNEL_HPP__

