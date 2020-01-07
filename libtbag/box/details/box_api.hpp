/**
 * @file   box_api.hpp
 * @brief  C-based box method prototype.
 * @author zer0
 * @date   2019-02-21
 * @date   2019-03-04 (Rename: libtbag/box/CBox -> libtbag/box/details/box)
 * @date   2019-03-14 (Rename: libtbag/box/details/box -> libtbag/box/details/box_api)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/box/details/box_common.hpp>

#include <cstdint>
#include <cstdarg>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

struct box_cursor;
struct box_data;

// clang-format off
inline bool box_is_unknown_type (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_PREFIX(type) == TBAG_BOX_TYPE_PREFIX_UNKNOWN ; }
inline bool box_is_boolean_type (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_PREFIX(type) == TBAG_BOX_TYPE_PREFIX_BOOLEAN ; }
inline bool box_is_string_type  (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_PREFIX(type) == TBAG_BOX_TYPE_PREFIX_STRING  ; }
inline bool box_is_signed_type  (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_PREFIX(type) == TBAG_BOX_TYPE_PREFIX_SIGNED  ; }
inline bool box_is_unsigned_type(btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_PREFIX(type) == TBAG_BOX_TYPE_PREFIX_UNSIGNED; }
inline bool box_is_floating_type(btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_PREFIX(type) == TBAG_BOX_TYPE_PREFIX_FLOATING; }
inline bool box_is_complex_type (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_PREFIX(type) == TBAG_BOX_TYPE_PREFIX_COMPLEX ; }
// clang-format on

// clang-format off
inline bool box_is_system_type(btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_SUFFIX(type) == TBAG_BOX_TYPE_SIZE_SYSTEM; }
inline bool box_is_0bit_type  (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_SUFFIX(type) == TBAG_BOX_TYPE_SIZE_0BIT  ; }
inline bool box_is_8bit_type  (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_SUFFIX(type) == TBAG_BOX_TYPE_SIZE_8BIT  ; }
inline bool box_is_16bit_type (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_SUFFIX(type) == TBAG_BOX_TYPE_SIZE_16BIT ; }
inline bool box_is_32bit_type (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_SUFFIX(type) == TBAG_BOX_TYPE_SIZE_32BIT ; }
inline bool box_is_64bit_type (btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_SUFFIX(type) == TBAG_BOX_TYPE_SIZE_64BIT ; }
inline bool box_is_128bit_type(btype type) TBAG_NOEXCEPT { return TBAG_GET_BOX_TYPE_SUFFIX(type) == TBAG_BOX_TYPE_SIZE_128BIT; }
// clang-format on

TBAG_API bool box_support_type(btype type) TBAG_NOEXCEPT;
TBAG_API bool box_support_device(bdev dev) TBAG_NOEXCEPT;

TBAG_API char const * box_get_type_name(btype type) TBAG_NOEXCEPT;
TBAG_API char const * box_get_device_name(bdev dev) TBAG_NOEXCEPT;

TBAG_API ui32 box_get_type_byte(btype type) TBAG_NOEXCEPT;

TBAG_API bool box_ext_is_equals(ui64 const * ext1, ui64 const * ext2) TBAG_NOEXCEPT;

TBAG_API ui32 * box_dim_malloc(ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_args(ui32 rank, ...) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_dims(ui32 rank, ui32 const * dims) TBAG_NOEXCEPT;
TBAG_API void   box_dim_free(ui32 * dims) TBAG_NOEXCEPT;

TBAG_API void box_dim_set_args(ui32 * TBAG_RESTRICT result, ui32 args_count, ...) TBAG_NOEXCEPT;
TBAG_API void box_dim_set_vargs(ui32 * TBAG_RESTRICT result, ui32 args_count, va_list ap) TBAG_NOEXCEPT;
TBAG_API void box_dim_set_dims(ui32 * TBAG_RESTRICT result, ui32 args_count, ui32 const * dims) TBAG_NOEXCEPT;

TBAG_API void box_dim_copy(ui32 * dest, ui32 const * src, ui32 rank) TBAG_NOEXCEPT;

TBAG_API ui32 * box_dim_clone(ui32 const * src, ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_clone_with_alloc_size(ui32 const * src, ui32 alloc_size, ui32 rank) TBAG_NOEXCEPT;

TBAG_API bool box_dim_is_equals(ui32 const * dims1, ui32 rank1, ui32 const * dims2, ui32 rank2) TBAG_NOEXCEPT;
TBAG_API bool box_dim_is_equals_args(ui32 const * dims1, ui32 rank1, ui32 rank2, ...) TBAG_NOEXCEPT;
TBAG_API bool box_dim_is_equals_vargs(ui32 const * dims1, ui32 rank1, ui32 rank2, va_list ap) TBAG_NOEXCEPT;

TBAG_API ui32 box_dim_get_total_size(ui32 const * dims, ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 box_dim_get_total_size_args(ui32 rank, ...) TBAG_NOEXCEPT;
TBAG_API ui32 box_dim_get_total_size_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT;
TBAG_API ui32 box_dim_get_stride(ui32 const * dims, ui32 rank, ui32 dim_index) TBAG_NOEXCEPT;

/**
 * The formula to obtain the index is as follows:
 * calc 1rank offset: x
 * calc 2rank offset: x + (y * width)
 * calc 3rank offset: x + (y * width) + (z * width * height)
 * calc 4rank offset: x + (y * width) + (z * width * height) + (w * width * height * depth)
 * ...
 *
 * Therefore, it can be converted into the following formula:
 * ...
 * calc 4rank offset: x + ((y + (z + (w * depth)) * height) * width)
 * calc 3rank offset: x + (y + (z * height)) * width
 * calc 2rank offset: x + (y * width)
 * calc 1rank offset: x
 *
 * Calculated from inside parentheses.
 */
TBAG_API ui32 box_dim_get_offset_args(ui32 const * dims, ui32 rank, ...) TBAG_NOEXCEPT;
TBAG_API ui32 box_dim_get_offset_vargs(ui32 const * dims, ui32 rank, va_list ap) TBAG_NOEXCEPT;
TBAG_API ui32 box_dim_get_offset_dims(ui32 const * dims, ui32 rank, ui32 const * indexes) TBAG_NOEXCEPT;

TBAG_API ui8 * box_info_malloc(ui32 info_size) TBAG_NOEXCEPT;
TBAG_API void  box_info_free(ui8 * info) TBAG_NOEXCEPT;
TBAG_API bool  box_info_assign_buffer(ui8 * dest, ui32 dest_size, ui8 const * src, ui32 src_size) TBAG_NOEXCEPT;
TBAG_API bool  box_info_assign_string(ui8 * dest, ui32 dest_size, char const * src) TBAG_NOEXCEPT;

TBAG_API void       * box_data_ptr_offset_raw(void       * data, btype type, ui32 offset) TBAG_NOEXCEPT;
TBAG_API void const * box_data_ptr_offset_raw(void const * data, btype type, ui32 offset) TBAG_NOEXCEPT;

TBAG_API bool box_data_check_address_raw(void const * data_begin, ui32 size, btype type,
                                         void const * check_data) TBAG_NOEXCEPT;

TBAG_API int box_index_abs(ui32 const * dims, ui32 dim_index, int data_index) TBAG_NOEXCEPT;
TBAG_API int box_index_begin_abs(ui32 const * dims, ui32 dim_index, int data_index, int data_step) TBAG_NOEXCEPT;
TBAG_API int box_index_end_abs(ui32 const * dims, ui32 dim_index, int data_index, int data_step) TBAG_NOEXCEPT;
TBAG_API bool box_step_check(int begin, int end, int step) TBAG_NOEXCEPT;

TBAG_API void * box_data_malloc(bdev device, ui32 byte) TBAG_NOEXCEPT;
TBAG_API void * box_data_malloc(btype type, bdev device, ui32 element_size) TBAG_NOEXCEPT;
TBAG_API void   box_data_free(bdev device, void * data) TBAG_NOEXCEPT;

TBAG_API Err box_comp_test(box_data const * lh, box_data const * rh, box_data const * out);

/**
 * box_slice structure.
 *
 * @author zer0
 * @date   2019-12-29
 * @date   2019-12-30 (Rename: Slice -> box_slice)
 */
struct box_slice
{
    int begin = box_nop;
    int end = box_nop;
    int step = 1;
};

/**
 * Box container information structure.
 *
 * @author zer0
 * @date   2019-03-04
 *
 * @remarks
 *  It is an alternative to the 1d vector container. @n
 *  Therefore, the entire memory must be connected in one dimension.
 */
struct TBAG_API box_data : private Noncopyable
{
    /** Data type. */
    btype type;

    /** Device type. */
    bdev device;

    /** Device extension information. */
    /** [PARALLEL] 0:Platform, 1:Device, 2:Context 3:Unused*/
    ui64 ext[TBAG_BOX_EXT_SIZE];

    /** Real data. */
    void * data;

    /** Real data byte. */
    ui32 total_data_byte;

    /** Total number of entries. */
    ui32 size;

    /**
     * Dimension data.
     *
     * @remarks
     *  Example:
     *  - Index: 0,1,2,3
     *  - Tensor: batch,height,width,color
     *  - 2D-Matrix: rows,cols
     *  - 2D-Image: height,width
     *  - Coordinate: w,z,y,x
     */
    ui32 * dims;

    /** Dimension data byte. */
    ui32 total_dims_byte;

    /** Number of dimensions. */
    ui32 rank;

    /** Extra information. */
    ui8 * info;

    /** Extra information data byte. */
    ui32 total_info_byte;

    /** Extra information size. */
    ui32 info_size;

    /** User's data. */
    box_any opaque;

    /** If not null, Called when released. */
    box_opaque_delete_cb opaque_deleter;

    box_data() TBAG_NOEXCEPT;
    box_data(box_data && obj) TBAG_NOEXCEPT;
    ~box_data();

    box_data & operator =(box_data && obj) TBAG_NOEXCEPT;

    void swap(box_data & obj) TBAG_NOEXCEPT;

    friend inline void swap(box_data & lh, box_data & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

    void release();

    void clear() TBAG_NOEXCEPT;
    void clear_opaque() TBAG_NOEXCEPT;

    bool exists_data() const TBAG_NOEXCEPT;
    bool exists_dims() const TBAG_NOEXCEPT;
    bool exists_info() const TBAG_NOEXCEPT;

    bool support_type() const TBAG_NOEXCEPT
    { return box_support_type(type); }
    bool support_device(bdev dev) const TBAG_NOEXCEPT
    { return box_support_device(device); }

    char const * get_type_name(btype type) const TBAG_NOEXCEPT
    { return box_get_type_name(type); }
    char const * get_device_name() const TBAG_NOEXCEPT
    { return box_get_device_name(device); }

    ui32 get_type_byte() const TBAG_NOEXCEPT
    { return box_get_type_byte(type); }

    void set_opaque(box_any const & v) TBAG_NOEXCEPT;
    void set_opaque(void * v) TBAG_NOEXCEPT;
    void set_opaque(bool v) TBAG_NOEXCEPT;
    void set_opaque(si8  v) TBAG_NOEXCEPT;
    void set_opaque(si16 v) TBAG_NOEXCEPT;
    void set_opaque(si32 v) TBAG_NOEXCEPT;
    void set_opaque(si64 v) TBAG_NOEXCEPT;
    void set_opaque(ui8  v) TBAG_NOEXCEPT;
    void set_opaque(ui16 v) TBAG_NOEXCEPT;
    void set_opaque(ui32 v) TBAG_NOEXCEPT;
    void set_opaque(ui64 v) TBAG_NOEXCEPT;
    void set_opaque(fp32 v) TBAG_NOEXCEPT;
    void set_opaque(fp64 v) TBAG_NOEXCEPT;

    void get_opaque(box_any * v) TBAG_NOEXCEPT;
    void get_opaque(void ** v) const TBAG_NOEXCEPT;
    void get_opaque(bool * v) const TBAG_NOEXCEPT;
    void get_opaque(si8  * v) const TBAG_NOEXCEPT;
    void get_opaque(si16 * v) const TBAG_NOEXCEPT;
    void get_opaque(si32 * v) const TBAG_NOEXCEPT;
    void get_opaque(si64 * v) const TBAG_NOEXCEPT;
    void get_opaque(ui8  * v) const TBAG_NOEXCEPT;
    void get_opaque(ui16 * v) const TBAG_NOEXCEPT;
    void get_opaque(ui32 * v) const TBAG_NOEXCEPT;
    void get_opaque(ui64 * v) const TBAG_NOEXCEPT;
    void get_opaque(fp32 * v) const TBAG_NOEXCEPT;
    void get_opaque(fp64 * v) const TBAG_NOEXCEPT;

    box_any get_opaque_any() const TBAG_NOEXCEPT;
    void * get_opaque_pointer() const TBAG_NOEXCEPT;
    bool get_opaque_bool() const TBAG_NOEXCEPT;
    si8  get_opaque_si8 () const TBAG_NOEXCEPT;
    si16 get_opaque_si16() const TBAG_NOEXCEPT;
    si32 get_opaque_si32() const TBAG_NOEXCEPT;
    si64 get_opaque_si64() const TBAG_NOEXCEPT;
    ui8  get_opaque_ui8 () const TBAG_NOEXCEPT;
    ui16 get_opaque_ui16() const TBAG_NOEXCEPT;
    ui32 get_opaque_ui32() const TBAG_NOEXCEPT;
    ui64 get_opaque_ui64() const TBAG_NOEXCEPT;
    fp32 get_opaque_fp32() const TBAG_NOEXCEPT;
    fp64 get_opaque_fp64() const TBAG_NOEXCEPT;

    Err alloc_args(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, ...);
    Err alloc_vargs(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, va_list ap);
    Err alloc_dims_copy(btype src_type, bdev src_device, ui64 const * src_ext,
                        ui32 const * src_dims, ui32 src_dims_byte, ui32 src_rank);
    Err alloc_dims_move(btype src_type, bdev src_device, ui64 const * src_ext,
                        ui32 * src_dims, ui32 src_dims_byte, ui32 src_rank);

    ui32 get_dims_total_size() const TBAG_NOEXCEPT;

    ErrPair<box_cursor> init_cursor(void * data, ui32 dim_index,
                                    int begin, int end, int step) TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(void * data, ui32 dim_index,
                                    box_slice const & slice) TBAG_NOEXCEPT;

    ErrPair<box_cursor> init_cursor(ui32 dim_index, box_slice const & slice) TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(ui32 dim_index, int begin, int end, int step) TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(ui32 dim_index, int begin, int end) TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(ui32 dim_index, int begin) TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(ui32 dim_index) TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor() TBAG_NOEXCEPT;

    Err resize_args(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, ...);
    Err resize_vargs(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, va_list ap);
    Err resize_dims(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, ui32 const * src_dims);

    ErrPair<box_data> clone(btype change_type, btype change_device, ui64 const * change_ext) const;
    ErrPair<box_data> clone(btype change_device, ui64 const * change_ext) const;
    ErrPair<box_data> clone(btype change_type) const;
    ErrPair<box_data> clone() const;

    void       * get_data_ptr_by_offset(ui32 offset)       TBAG_NOEXCEPT;
    void const * get_data_ptr_by_offset(ui32 offset) const TBAG_NOEXCEPT;

    bool check_data_address(void const * test_data_pointer) const TBAG_NOEXCEPT;

    template <typename ... Args>
    ui32 get_offset_args(Args && ... args) const TBAG_NOEXCEPT
    { return box_dim_get_offset_args(dims, rank, std::forward<Args>(args) ...); }
    ui32 get_offset_vargs(va_list ap) const TBAG_NOEXCEPT
    { return box_dim_get_offset_vargs(dims, rank, ap); }
    ui32 get_offset_dims(ui32 const * indexes) const TBAG_NOEXCEPT
    { return box_dim_get_offset_dims(dims, rank, indexes); }

    void checked_assign_info_buffer(ui8 const * src, ui32 src_size);
    void checked_assign_info_string(char const * src);

    Err set_data(void const * src_data, btype src_type, bdev src_device,
                 ui64 const * src_ext, void * dest_data);
    Err set_data(void const * src_data, btype src_type, bdev src_device,
                 ui64 const * src_ext, ui32 box_data_offset);
    Err set_data_args(void const * src_data, btype src_type, bdev src_device,
                      ui64 const * src_ext, ui32 box_rank, ...);
    Err set_data_vargs(void const * src_data, btype src_type, bdev src_device,
                       ui64 const * src_ext, ui32 box_rank, va_list box_ap);
    Err set_data_dims(void const * src_data, btype src_type, bdev src_device,
                      ui64 const * src_ext, ui32 box_rank, ui32 const * box_indexes);

    Err get_data(void * out_data, btype out_type, bdev out_device,
                 ui64 const * out_ext, void const * src_data) const;
    Err get_data(void * out_data, btype out_type, bdev out_device,
                 ui64 const * out_ext, ui32 box_data_offset) const;
    Err get_data_args(void * out_data, btype out_type, bdev out_device,
                      ui64 const * out_ext, ui32 box_rank, ...) const;
    Err get_data_vargs(void * out_data, btype out_type, bdev out_device,
                       ui64 const * out_ext, ui32 box_rank, va_list box_ap) const;
    Err get_data_dims(void * out_data, btype out_type, bdev out_device,
                      ui64 const * out_ext, ui32 box_rank, ui32 const * box_indexes) const;

    Err assign_data(btype src_type, bdev src_device, ui64 const * src_ext,
                    ui32 src_size, void const * src_data);
    Err checked_assign_data(btype src_type, bdev src_device, ui64 const * src_ext,
                            ui32 src_rank, ui32 const * src_dims, void const * src_data);

    Err eq(box_data const * comp, box_data * result) const;
    Err ne(box_data const * comp, box_data * result) const;
    Err lt(box_data const * comp, box_data * result) const;
    Err le(box_data const * comp, box_data * result) const;
    Err gt(box_data const * comp, box_data * result) const;
    Err ge(box_data const * comp, box_data * result) const;
};

/**
 * Box container iterator cursor structure.
 *
 * @author zer0
 * @date   2019-05-16
 * @date   2019-09-24 (Update properties)
 */
struct TBAG_API box_cursor
{
    /** Original box pointer. */
    box_data * box;

    /** Current data pointer. */
    void * begin;

    /** The exact point where the loop should stop. */
    void * end;

    /** Byte size to jump step by step. */
    int stride_byte;

    /** Cursor's dimension index. */
    ui32 dim_index;

    box_cursor() TBAG_NOEXCEPT;
    ~box_cursor();

    ErrPair<box_cursor> init_cursor(ui32 dim_index, box_slice const & slice) const TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(ui32 dim_index, int begin_index, int end_index, int step_index) const TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(ui32 dim_index, int begin_index, int end_index) const TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(ui32 dim_index, int begin_index) const TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor(ui32 dim_index) const TBAG_NOEXCEPT;
    ErrPair<box_cursor> init_cursor() const TBAG_NOEXCEPT;

    int diff() const TBAG_NOEXCEPT;

    bool is_last_dim() const TBAG_NOEXCEPT;
    bool is_continue() const TBAG_NOEXCEPT;
    bool is_end() const TBAG_NOEXCEPT;
    bool next() TBAG_NOEXCEPT;
};

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_HPP__

