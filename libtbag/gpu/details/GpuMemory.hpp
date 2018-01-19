/**
 * @file   GpuMemory.hpp
 * @brief  GpuMemory class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>

#include <cstdlib>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

// Forward declaration.
namespace accel  { class  AccelContext; }
namespace cpu    { class    CpuContext; }
namespace cuda   { class   CudaContext; }
namespace opencl { class OpenCLContext; }

namespace details {

// Forward declaration.
class GpuContext;
class GpuStream;
class GpuEvent;
class GpuMemory;
class HostMemory;

/**
 * MemoryWrapper class prototype.
 *
 * @author zer0
 * @date   2018-01-16
 */
class TBAG_API MemoryWrapper : private Noncopyable
{
private:
    GpuStream const & _stream;

protected:
    std::size_t _capacity;
    std::size_t _size;
    void *      _data;

public:
    MemoryWrapper(GpuStream const & stream);
    ~MemoryWrapper();

public:
    inline GpuStream const & atStream() const TBAG_NOEXCEPT { return _stream; }
    inline bool isSameStream(GpuStream const & stream) const TBAG_NOEXCEPT { return (&_stream) == (&stream); }

    inline std::size_t capacity() const TBAG_NOEXCEPT { return _capacity; }
    inline std::size_t     size() const TBAG_NOEXCEPT { return     _size; }

    inline bool validate() const TBAG_NOEXCEPT { return _data != nullptr && _capacity > 0; }
    inline bool validate(GpuStream const & stream) const TBAG_NOEXCEPT { return isSameStream(stream) && validate(); }

    inline void       * data()       TBAG_NOEXCEPT { return _data; }
    inline void const * data() const TBAG_NOEXCEPT { return _data; }

    template <typename T> inline T cast() const TBAG_NOEXCEPT { return (T)_data; }

public:
    GpuContext const & atContext() const TBAG_NOEXCEPT;
    bool isSameContext(GpuContext const & context) const TBAG_NOEXCEPT;
};

/**
 * GpuMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuMemory : public MemoryWrapper
{
public:
    friend class accel::AccelContext;
    friend class cpu::CpuContext;
    friend class cuda::CudaContext;
    friend class opencl::OpenCLContext;
    friend class GpuContext;

public:
    GpuMemory(GpuStream const & stream, std::size_t size);
    ~GpuMemory();

protected:
    void set(void * data, std::size_t capacity, std::size_t size) TBAG_NOEXCEPT;
    void clear() TBAG_NOEXCEPT;

public:
    Err      copy( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err      copy(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
};

/**
 * HostMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API HostMemory : public MemoryWrapper
{
public:
    friend class accel::AccelContext;
    friend class cpu::CpuContext;
    friend class cuda::CudaContext;
    friend class opencl::OpenCLContext;
    friend class GpuContext;

private:
    HostMemoryFlag _flag;

public:
    HostMemory(GpuStream const & stream, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT);
    ~HostMemory();

protected:
    void set(void * data, std::size_t capacity, std::size_t size, HostMemoryFlag flag) TBAG_NOEXCEPT;
    void clear() TBAG_NOEXCEPT;

public:
    Err      copy( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err      copy(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
};

using SharedGpuMemory  = std::shared_ptr<GpuMemory>;
using WeakedGpuMemory  = std::weak_ptr<GpuMemory>;

using SharedHostMemory = std::shared_ptr<HostMemory>;
using WeakedHostMemory = std::weak_ptr<HostMemory>;

using HostMemories = std::vector<HostMemory>;
using  GpuMemories = std::vector<GpuMemory>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__

