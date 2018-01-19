/**
 * @file   AddKernel.cpp
 * @brief  AddKernel class implementation.
 * @author zer0
 * @date   2018-01-19
 */

#include <libtbag/gpu/kernels/AddKernel.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/gpu/accel/AccelRaw.hpp>
#include <libtbag/gpu/cpu/CpuRaw.hpp>
#include <libtbag/gpu/cuda/CudaRaw.hpp>
#include <libtbag/gpu/opencl/OpenCLRaw.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace kernels {

AddKernel::AddKernel(GpuStream const & stream) : BaseKernel(stream)
{
    if (getType() == GpuType::GT_OPENCL) {
        using namespace libtbag::gpu::opencl;
        _ki = createKernel(getOpenCLSourceOfAdd1i(), getOpenCLName1i(getOpenCLPrefixOfAdd()));
        _ku = createKernel(getOpenCLSourceOfAdd1u(), getOpenCLName1u(getOpenCLPrefixOfAdd()));
        _kf = createKernel(getOpenCLSourceOfAdd1f(), getOpenCLName1f(getOpenCLPrefixOfAdd()));
        _kd = createKernel(getOpenCLSourceOfAdd1d(), getOpenCLName1d(getOpenCLPrefixOfAdd()));
    }
}

AddKernel::~AddKernel()
{
    // EMPTY.
}

Err AddKernel::run(SyncedMemory const & v1, SyncedMemory const & v2, SyncedMemory & r, int count)
{
    auto const ELEM_TYPE = v1.type();
    if (ELEM_TYPE != v2.type() || ELEM_TYPE != r.type()) {
        return Err::E_ILLARGS;
    }
    if (isSupport(ELEM_TYPE) == false) {
        return Err::E_UNSUPOP;
    }

    auto const GPU_TYPE = getType();
    bool result = false;

    if (GPU_TYPE == GpuType::GT_OPENCL) {
        std::size_t global_work_size[1] = { static_cast<std::size_t>(count) };
        switch (ELEM_TYPE) {
        case TypeTable::TT_INT:    return _stream.atContext().runKernel(_stream, *_ki, 1, nullptr, global_work_size, nullptr, _event.get());
        case TypeTable::TT_UINT:   return _stream.atContext().runKernel(_stream, *_ku, 1, nullptr, global_work_size, nullptr, _event.get());
        case TypeTable::TT_FLOAT:  return _stream.atContext().runKernel(_stream, *_kf, 1, nullptr, global_work_size, nullptr, _event.get());
        case TypeTable::TT_DOUBLE: return _stream.atContext().runKernel(_stream, *_kd, 1, nullptr, global_work_size, nullptr, _event.get());
        }
    } else if (GPU_TYPE == GpuType::GT_CUDA) {
        result = cuda::runCudaAdd(ELEM_TYPE, v1.getGpuData(), v2.getGpuData(), r.getMutableGpuData(), count, _stream.castId<void*>());
    } else if (GPU_TYPE == GpuType::GT_ACCEL) {
        result = accel::runAccelAdd(ELEM_TYPE, v1.getGpuData(), v2.getGpuData(), r.getMutableGpuData(), count);
    } else {
        result = cpu::runCpuAdd(ELEM_TYPE, v1.getGpuData(), v2.getGpuData(), r.getMutableGpuData(), count);
    }

    return (result ? Err::E_SUCCESS : Err::E_UNKNOWN);
}

} // namespace kernels
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

