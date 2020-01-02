/**
 * @file   BoxBase.hpp
 * @brief  BoxBase class prototype.
 * @author zer0
 * @date   2020-01-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXBASE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXBASE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/box/BoxTraits.hpp>

#include <cassert>
#include <cstddef>

#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

/**
 * BoxBase class prototype.
 *
 * @author zer0
 * @date   2020-01-01
 */
class TBAG_API BoxBase
{
public:
    using size_type = ui32;
    using difference_type = std::ptrdiff_t;
    using SharedBoxData = std::shared_ptr<box_data>;
    using Buffer = std::vector<ui8>;

protected:
    SharedBoxData _base;

protected:
    BoxBase();
    explicit BoxBase(std::nullptr_t) TBAG_NOEXCEPT;
    explicit BoxBase(box_data && box) TBAG_NOEXCEPT;
    explicit BoxBase(SharedBoxData const & box) TBAG_NOEXCEPT;
    explicit BoxBase(SharedBoxData && box) TBAG_NOEXCEPT;
    BoxBase(BoxBase const & obj) TBAG_NOEXCEPT;
    BoxBase(BoxBase && obj) TBAG_NOEXCEPT;
    ~BoxBase();

public:
    inline bool exists() const TBAG_NOEXCEPT
    {
        return static_cast<bool>(_base);
    }

    inline operator bool() const TBAG_NOEXCEPT
    {
        return exists();
    }

    void reset() TBAG_NOEXCEPT_SP_OP(_base.reset())
    {
        _base.reset();
    }

public:
    inline box_data       * base()       TBAG_NOEXCEPT { return _base.get(); }
    inline box_data const * base() const TBAG_NOEXCEPT { return _base.get(); }

public:
    inline btype getType() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->type;
        }
        return type_none();
    }

    inline btype type() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->type;
    }

    inline bool isSupportType() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_support_type(getType());
    }

    inline char const * getTypeName() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_type_name(getType());
    }

    inline ui32 getTypeByte() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_type_byte(getType());
    }

    // clang-format off
    inline bool is_none() const TBAG_NOEXCEPT { return getType() == type_none(); }
    inline bool is_bool() const TBAG_NOEXCEPT { return getType() == type_bool(); }
    inline bool is_si8 () const TBAG_NOEXCEPT { return getType() == type_si8 (); }
    inline bool is_si16() const TBAG_NOEXCEPT { return getType() == type_si16(); }
    inline bool is_si32() const TBAG_NOEXCEPT { return getType() == type_si32(); }
    inline bool is_si64() const TBAG_NOEXCEPT { return getType() == type_si64(); }
    inline bool is_ui8 () const TBAG_NOEXCEPT { return getType() == type_ui8 (); }
    inline bool is_ui16() const TBAG_NOEXCEPT { return getType() == type_ui16(); }
    inline bool is_ui32() const TBAG_NOEXCEPT { return getType() == type_ui32(); }
    inline bool is_ui64() const TBAG_NOEXCEPT { return getType() == type_ui64(); }
    inline bool is_fp32() const TBAG_NOEXCEPT { return getType() == type_fp32(); }
    inline bool is_fp64() const TBAG_NOEXCEPT { return getType() == type_fp64(); }
    // clang-format on

    inline bool is_signed() const TBAG_NOEXCEPT
    {
        return is_si8() || is_si16() || is_si32() || is_si64();
    }

    inline bool is_unsigned() const TBAG_NOEXCEPT
    {
        return is_ui8() || is_ui16() || is_ui32() || is_ui64();
    }

    inline bool is_floating() const TBAG_NOEXCEPT
    {
        return is_fp32() || is_fp64();
    }

public:
    inline btype getDevice() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->device;
        }
        return device_none();
    }

    inline btype device() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->device;
    }

    inline bool isSupportDevice() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_support_device(getDevice());
    }

    inline char const * getDeviceName(bdev dev) const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_device_name(getType());
    }

    // clang-format off
    inline bool is_device_none() const TBAG_NOEXCEPT { return getDevice() == device_none(); }
    inline bool is_device_cpu () const TBAG_NOEXCEPT { return getDevice() == device_cpu (); }
    inline bool is_device_cuda() const TBAG_NOEXCEPT { return getDevice() == device_cuda(); }
    inline bool is_device_cl  () const TBAG_NOEXCEPT { return getDevice() == device_cl  (); }
    // clang-format on

public:
    inline ui64 const * getExtensions() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->ext;
        }
        return nullptr;
    }

    inline ui64 const * ext() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->ext;
    }

    // clang-format off
    inline ui64 getExtension0() const TBAG_NOEXCEPT { if (_base) { return _base->ext[0]; } return 0; }
    inline ui64 getExtension1() const TBAG_NOEXCEPT { if (_base) { return _base->ext[1]; } return 0; }
    inline ui64 getExtension2() const TBAG_NOEXCEPT { if (_base) { return _base->ext[2]; } return 0; }
    inline ui64 getExtension3() const TBAG_NOEXCEPT { if (_base) { return _base->ext[3]; } return 0; }
    inline ui64 ext0() const TBAG_NOEXCEPT { assert(exists()); return _base->ext[0]; }
    inline ui64 ext1() const TBAG_NOEXCEPT { assert(exists()); return _base->ext[1]; }
    inline ui64 ext2() const TBAG_NOEXCEPT { assert(exists()); return _base->ext[2]; }
    inline ui64 ext3() const TBAG_NOEXCEPT { assert(exists()); return _base->ext[3]; }
    // clang-format on

    inline bool testExtensions(ui64 e0, ui64 e1, ui64 e2, ui64 e3) const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->ext[0] == e0 &&
                   _base->ext[1] == e1 &&
                   _base->ext[2] == e2 &&
                   _base->ext[3] == e3;
        }
        return false;
    }

public:
    template <typename T>
    inline void * getData() TBAG_NOEXCEPT
    {
        if (_base) {
            return static_cast<T*>(_base->data);
        }
        return nullptr;
    }

    template <typename T>
    inline void const * getData() const TBAG_NOEXCEPT
    {
        if (_base) {
            return static_cast<T*>(_base->data);
        }
        return nullptr;
    }

    inline void * getData() TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->data;
        }
        return nullptr;
    }

    inline void const * getData() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->data;
        }
        return nullptr;
    }

    template <typename T>
    inline T * data() TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T*>(_base->data);
    }

    template <typename T>
    inline T const * data() const TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T const *>(_base->data);
    }

    inline void * data() TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->data;
    }

    inline void const * data() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->data;
    }

    template <typename T>
    inline T * data(ui32 i) TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T*>(_base->get_data_ptr_by_offset(i));
    }

    template <typename T>
    inline T const * data(ui32 i) const TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T const *>(_base->get_data_ptr_by_offset(i));
    }

    inline void * data(ui32 i) TBAG_NOEXCEPT
    {
        assert(exists());
        assert(i < size());
        return _base->get_data_ptr_by_offset(i);
    }

    inline void const * data(ui32 i) const TBAG_NOEXCEPT
    {
        assert(exists());
        assert(i < size());
        return _base->get_data_ptr_by_offset(i);
    }

    inline ui32 getTotalDataByte() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->total_data_byte;
        }
        return 0;
    }

    inline ui32 total_data_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->total_data_byte;
    }

    inline ui32 getCapacity() const TBAG_NOEXCEPT
    {
        return getTotalDataByte();
    }

    inline ui32 capacity() const TBAG_NOEXCEPT
    {
        return total_data_byte();
    }

    inline ui32 getSize() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->size;
        }
        return 0;
    }

    inline ui32 size() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->size;
    }

    inline bool isEmpty() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->size == 0;
        }
        return true;
    }

    inline bool empty() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->size == 0;
    }

public:
    inline ui32 const * getDimensions() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->dims;
        }
        return nullptr;
    }

    inline ui32 const * dims() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->dims;
    }

    inline ui32 getDimension(ui32 i) const TBAG_NOEXCEPT
    {
        if (_base && i < _base->rank) {
            return _base->dims[i];
        }
        return 0;
    }

    inline ui32 dim(ui32 i) const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->dims[i];
    }

    inline ui32 getTotalDimsByte() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->total_dims_byte;
        }
        return 0;
    }

    inline ui32 total_dims_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->total_dims_byte;
    }

    inline ui32 getDimensionsCapacity() const TBAG_NOEXCEPT
    {
        return getTotalDimsByte();
    }

    inline ui32 dims_capacity() const TBAG_NOEXCEPT
    {
        return total_dims_byte();
    }

    inline ui32 getRank() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->rank;
        }
        return 0;
    }

    inline ui32 rank() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->rank;
    }

    inline ui32 getStride(ui32 i) const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_dim_get_stride(getDimensions(), getRank(), i);
    }

    inline ui32 getStrideByte(ui32 i) const TBAG_NOEXCEPT
    {
        return getStride(i) * getTypeByte();
    }

public:
    inline ui8 * getInfo() TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->info;
        }
        return nullptr;
    }

    inline ui8 const * getInfo() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->info;
        }
        return nullptr;
    }

    inline ui8 * info() TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->info;
    }

    inline ui8 const * info() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->info;
    }

    inline ui32 getTotalInfoByte() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->total_info_byte;
        }
        return 0;
    }

    inline ui32 total_info_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->total_info_byte;
    }

    inline ui32 getInfoCapacity() const TBAG_NOEXCEPT
    {
        return getTotalInfoByte();
    }

    inline ui32 info_capacity() const TBAG_NOEXCEPT
    {
        return total_info_byte();
    }

    inline ui32 getInfoSize() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->info_size;
        }
        return 0;
    }

    inline ui32 info_size() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _base->info_size;
    }

public:
    inline void setOpaque(void * v) const TBAG_NOEXCEPT
    {
        if (_base) {
            _base->set_opaque(v);
        }
    }

    template <typename T>
    inline void setOpaque(T v) TBAG_NOEXCEPT
    {
        if (_base) {
            _base->set_opaque(v);
        }
    }

    template <typename T>
    inline void getOpaque(T * v) const TBAG_NOEXCEPT
    {
        if (_base && v != nullptr) {
            _base->get_opaque(v);
        }
    }

    template <typename T>
    inline T getOpaque() const TBAG_NOEXCEPT
    {
        T result = T();
        getOpaque(&result);
        return result;
    }

    inline void * getOpaquePointer() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->get_opaque_pointer();
        }
        return nullptr;
    }

    inline box_opaque_delete_cb getOpaqueDeleter() const TBAG_NOEXCEPT
    {
        if (_base) {
            return _base->opaque_deleter;
        }
        return nullptr;
    }

    inline void setOpaqueDeleter(box_opaque_delete_cb cb) TBAG_NOEXCEPT
    {
        if (_base) {
            _base->opaque_deleter = cb;
        }
    }

public:
    inline void clearData() TBAG_NOEXCEPT
    {
        if (_base) {
            _base->size = 0;
            _base->rank = 0;
        }
    }

    inline void clearInfo() TBAG_NOEXCEPT
    {
        if (_base) {
            _base->info_size = 0;
        }
    }

    inline void clear() TBAG_NOEXCEPT
    {
        clearData();
        clearInfo();
    }
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXBASE_HPP__

