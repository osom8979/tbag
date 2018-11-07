/**
 * @file   Bex.hpp
 * @brief  Bex class prototype.
 * @author zer0
 * @date   2018-10-22
 * @date   2018-11-07 (Rename: BagEx -> Bex)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BEX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BEX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/container/Bag.hpp>
#include <libtbag/memory/Allocator.hpp>

#include <cassert>
#include <cstdint>

#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <utility>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * Type table of Bex class.
 *
 * @author zer0
 * @date   2018-11-07
 */
enum class BexType : int
{
    // @formatter:off
    BT_NONE = 0, ///< Not initialized.
    BT_INT8    ,
    BT_UINT8   ,
    BT_INT16   ,
    BT_UINT16  ,
    BT_INT32   ,
    BT_UINT32  ,
    BT_INT64   ,
    BT_UINT64  ,
    BT_FLOAT32 ,
    BT_FLOAT64 ,
    // @formatter:on
};

template <typename T, BexType bex_type>
struct Info : public std::true_type
{
    using type = T;
    TBAG_CONSTEXPR static BexType const value = bex_type;
    TBAG_CONSTEXPR static std::size_t const size = sizeof(T);
};

template <>
struct Info<void, BexType::BT_NONE> : public std::false_type
{
    using type = void;
    TBAG_CONSTEXPR static BexType const value = BexType::BT_NONE;
    TBAG_CONSTEXPR static std::size_t const size = 0;
};

// @formatter:off
template <typename T> struct TypeInfo : public Info<    void, BexType::BT_NONE   > { /* EMPTY. */ };
template <> struct TypeInfo<  int8_t> : public Info<  int8_t, BexType::BT_INT8   > { /* EMPTY. */ };
template <> struct TypeInfo< uint8_t> : public Info< uint8_t, BexType::BT_UINT8  > { /* EMPTY. */ };
template <> struct TypeInfo< int16_t> : public Info< int16_t, BexType::BT_INT16  > { /* EMPTY. */ };
template <> struct TypeInfo<uint16_t> : public Info<uint16_t, BexType::BT_UINT16 > { /* EMPTY. */ };
template <> struct TypeInfo< int32_t> : public Info< int32_t, BexType::BT_INT32  > { /* EMPTY. */ };
template <> struct TypeInfo<uint32_t> : public Info<uint32_t, BexType::BT_UINT32 > { /* EMPTY. */ };
template <> struct TypeInfo< int64_t> : public Info< int64_t, BexType::BT_INT64  > { /* EMPTY. */ };
template <> struct TypeInfo<uint64_t> : public Info<uint64_t, BexType::BT_UINT64 > { /* EMPTY. */ };
template <> struct TypeInfo<   float> : public Info<   float, BexType::BT_FLOAT32> { /* EMPTY. */ };
template <> struct TypeInfo<  double> : public Info<  double, BexType::BT_FLOAT64> { /* EMPTY. */ };
// @formatter:on

// @formatter:off
template <BexType T> struct EnumInfo             : public Info<    void, BexType::BT_NONE   > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_INT8   > : public Info<  int8_t, BexType::BT_INT8   > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_UINT8  > : public Info< uint8_t, BexType::BT_UINT8  > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_INT16  > : public Info< int16_t, BexType::BT_INT16  > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_UINT16 > : public Info<uint16_t, BexType::BT_UINT16 > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_INT32  > : public Info< int32_t, BexType::BT_INT32  > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_UINT32 > : public Info<uint32_t, BexType::BT_UINT32 > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_INT64  > : public Info< int64_t, BexType::BT_INT64  > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_UINT64 > : public Info<uint64_t, BexType::BT_UINT64 > { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_FLOAT32> : public Info<   float, BexType::BT_FLOAT32> { /* EMPTY. */ };
template <> struct EnumInfo<BexType::BT_FLOAT64> : public Info<  double, BexType::BT_FLOAT64> { /* EMPTY. */ };
// @formatter:on

// @formatter:off
static_assert(TypeInfo<  int8_t>::size == 1,    "int8_t size is not correct.");
static_assert(TypeInfo< uint8_t>::size == 1,   "uint8_t size is not correct.");
static_assert(TypeInfo< int16_t>::size == 2,   "int16_t size is not correct.");
static_assert(TypeInfo<uint16_t>::size == 2,  "uint16_t size is not correct.");
static_assert(TypeInfo< int32_t>::size == 4,   "int32_t size is not correct.");
static_assert(TypeInfo<uint32_t>::size == 4,  "uint32_t size is not correct.");
static_assert(TypeInfo< int64_t>::size == 8,   "int64_t size is not correct.");
static_assert(TypeInfo<uint64_t>::size == 8,  "uint64_t size is not correct.");
static_assert(TypeInfo<   float>::size == 4, "float32_t size is not correct.");
static_assert(TypeInfo<  double>::size == 8, "float64_t size is not correct.");
// @formatter:on

template <typename T>
TBAG_CONSTEXPR inline BexType getBexType() TBAG_NOEXCEPT
{
    return TypeInfo<T>::value;
}

template <typename T>
inline bool isTypeEquals(BexType type) TBAG_NOEXCEPT
{
    return type == getBexType<T>();
}

inline char const * getBexTypeName(BexType type) TBAG_NOEXCEPT
{
    switch (type) {
    // @formatter:off
    case BexType::BT_NONE   : return    "NONE";
    case BexType::BT_INT8   : return    "INT8";
    case BexType::BT_UINT8  : return   "UINT8";
    case BexType::BT_INT16  : return   "INT16";
    case BexType::BT_UINT16 : return  "UINT16";
    case BexType::BT_INT32  : return   "INT32";
    case BexType::BT_UINT32 : return  "UINT32";
    case BexType::BT_INT64  : return   "INT64";
    case BexType::BT_UINT64 : return  "UINT64";
    case BexType::BT_FLOAT32: return "FLOAT32";
    case BexType::BT_FLOAT64: return "FLOAT64";
    default:                  return "UNKNOWN";
    // @formatter:on
    }
}

/**
 * Bex Extend class prototype.
 *
 * @author zer0
 * @date   2018-10-22
 * @date   2018-11-07 (Rename: BagEx -> Bex)
 *
 * @remarks
 *  Only primary types are supported.
 */
class TBAG_API Bex
{
private:
    TBAG_CONSTEXPR static unsigned int const MAX_ELEMENTS_BUFFER_SIZE = 8;

public:
    TBAG_CONSTEXPR static unsigned int getMaxDims() TBAG_NOEXCEPT
    {
        return MAX_ELEMENTS_BUFFER_SIZE;
    }

public:
    template <typename T>
    using Allocator = libtbag::memory::Allocator<T>;

    template <typename T>
    using BaseBagType = libtbag::container::Bag<T, Allocator<T>, MAX_ELEMENTS_BUFFER_SIZE>;

public:
    using BagInt8    = BaseBagType<  int8_t>; ///< == char (Default string type)
    using BagUint8   = BaseBagType< uint8_t>; ///< == unsigned char (Default buffer type)
    using BagInt16   = BaseBagType< int16_t>;
    using BagUint16  = BaseBagType<uint16_t>;
    using BagInt32   = BaseBagType< int32_t>;
    using BagUint32  = BaseBagType<uint32_t>;
    using BagInt64   = BaseBagType< int64_t>;
    using BagUint64  = BaseBagType<uint64_t>;
    using BagFloat32 = BaseBagType<   float>;
    using BagFloat64 = BaseBagType<  double>;

public:
    struct FakeBag { /* EMPTY. */ };
    struct User    { /* EMPTY. */ };

    using SharedBag  = std::shared_ptr<FakeBag>;
    using SharedUser = std::shared_ptr<User>;

private:
    BexType    _type;
    SharedBag  _bag;
    SharedUser _user;

public:
    Bex() TBAG_NOEXCEPT;
    Bex(std::string const & content);
    Bex(char const * content);
    Bex(Bex const & obj) TBAG_NOEXCEPT;
    Bex(Bex && obj) TBAG_NOEXCEPT;
    ~Bex();

public:
    template <typename Type>
    Bex(std::vector<Type> const & content) : Bex()
    {
        if (isFailure(fromVector<Type>(content))) {
            throw std::bad_alloc();
        }
    }

    template <typename Type>
    Bex(std::initializer_list<Type> list) : Bex()
    {
        if (isFailure(resize<Type>(list.size()))) {
            throw std::bad_alloc();
        }
        std::copy(list.begin(), list.end(), cast<Type>());
    }

public:
    Bex & operator =(Bex const & obj) TBAG_NOEXCEPT;
    Bex & operator =(Bex && obj) TBAG_NOEXCEPT;

public:
    Bex & operator =(std::string const & content);
    Bex & operator =(char const * content);

public:
    template <typename Type>
    Bex & operator =(std::vector<Type> const & content)
    {
        if (isFailure(fromVector<Type>(content))) {
            throw std::bad_alloc();
        }
        return *this;
    }

public:
    void copy(Bex const & obj) TBAG_NOEXCEPT;
    void swap(Bex & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Bex & lh, Bex & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_bag); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline FakeBag       * get()       TBAG_NOEXCEPT { return _bag.get(); }
    inline FakeBag const * get() const TBAG_NOEXCEPT { return _bag.get(); }

    inline SharedBag       & atBag()       TBAG_NOEXCEPT { return _bag; }
    inline SharedBag const & atBag() const TBAG_NOEXCEPT { return _bag; }

    inline SharedUser       & atUser()       TBAG_NOEXCEPT { return _user; }
    inline SharedUser const & atUser() const TBAG_NOEXCEPT { return _user; }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(Bex const & x, Bex const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(Bex const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(Bex const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    template <typename T,
              typename BaseBagT   = BaseBagType<T>,
              typename SharedBagT = std::shared_ptr<BaseBagT> >
    SharedBagT bag() const TBAG_NOEXCEPT
    {
        assert(isTypeEquals<T>(_type));
        return SharedBagT(_bag, (BaseBagT*)(_bag.get()));
    }

    template <typename CastUserT>
    std::shared_ptr<CastUserT> user() const TBAG_NOEXCEPT
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(User, CastUserT);
        return std::static_pointer_cast<CastUserT>(_user);
    }

    template <typename UserT, typename ... Args>
    void createUser(Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(User, UserT);
        _user = std::make_shared<UserT>(std::forward<Args>(args) ...);
    }

public:
    inline BexType getType() const TBAG_NOEXCEPT
    { return _type; }

    inline char const * getTypeName() const TBAG_NOEXCEPT
    { return getBexTypeName(_type); }

public:
    void clear();

public:
    Err create(BexType type);

public:
    template <typename T>
    Err create()
    { return create(getBexType<T>()); }

public:
    Err resize(unsigned i0/**/, unsigned i1 = 0, unsigned i2 = 0, unsigned i3 = 0,
               unsigned i4 = 0, unsigned i5 = 0, unsigned i6 = 0, unsigned i7 = 0);

public:
    template <typename Type, typename ... Args>
    Err resize(Args && ... args)
    {
        Err const CODE = create<Type>();
        if (CODE != Err::E_ALREADY && CODE != Err::E_SUCCESS) {
            return CODE;
        }
        return resize(std::forward<Args>(args) ...);
    }

public:
    void * data();
    void const * data() const;

public:
    template <typename CastT>
    CastT * cast()
    {
        return (CastT*)data();
    }

    template <typename CastT>
    CastT const * cast() const
    {
        return (CastT const *)data();
    }

    template <typename CastT, typename ... Args>
    CastT * cast(Args && ... args)
    {
        return cast<CastT>() + offset(std::forward<Args>(args) ...);
    }

    template <typename CastT, typename ... Args>
    CastT const * cast(Args && ... args) const
    {
        return cast<CastT>() + offset(std::forward<Args>(args) ...);
    }

    template <typename CastT, typename ... Args>
    CastT & at(Args && ... args)
    {
        return *cast<CastT>(std::forward<Args>(args) ...);
    }

    template <typename CastT, typename ... Args>
    CastT const & at(Args && ... args) const
    {
        return *cast<CastT>(std::forward<Args>(args) ...);
    }

public:
    std::size_t size() const;
    std::size_t size(std::size_t index) const;
    std::size_t dims() const;
    bool empty() const;

public:
    template <typename InputT>
    void copyFrom(InputT const * data, std::size_t size, std::size_t offset = 0)
    {
        assert(static_cast<bool>(_bag));
        switch (_type) {
        // @formatter:off
        case BexType::BT_INT8   : std::copy(data, data + size, cast<  int8_t>() + offset); break;
        case BexType::BT_UINT8  : std::copy(data, data + size, cast< uint8_t>() + offset); break;
        case BexType::BT_INT16  : std::copy(data, data + size, cast< int16_t>() + offset); break;
        case BexType::BT_UINT16 : std::copy(data, data + size, cast<uint16_t>() + offset); break;
        case BexType::BT_INT32  : std::copy(data, data + size, cast< int32_t>() + offset); break;
        case BexType::BT_UINT32 : std::copy(data, data + size, cast<uint32_t>() + offset); break;
        case BexType::BT_INT64  : std::copy(data, data + size, cast< int64_t>() + offset); break;
        case BexType::BT_UINT64 : std::copy(data, data + size, cast<uint64_t>() + offset); break;
        case BexType::BT_FLOAT32: std::copy(data, data + size, cast<   float>() + offset); break;
        case BexType::BT_FLOAT64: std::copy(data, data + size, cast<  double>() + offset); break;
        default: assert(false && "Unknown type assertion.");
        // @formatter:on
        }
    }

public:
    std::string toString() const;
    std::string toHexString() const;
    std::string toHexBoxString(int line_width = 2*8) const;
    std::string toInfoString() const;
    std::string toAutoString() const;

public:
    template <typename T, typename VectorT = std::vector<T> >
    VectorT toVector() const
    {
        assert(static_cast<bool>(_bag));
        switch (_type) {
        // @formatter:off
        case BexType::BT_INT8   : return VectorT(cast<  int8_t>(), cast<  int8_t>() + size());
        case BexType::BT_UINT8  : return VectorT(cast< uint8_t>(), cast< uint8_t>() + size());
        case BexType::BT_INT16  : return VectorT(cast< int16_t>(), cast< int16_t>() + size());
        case BexType::BT_UINT16 : return VectorT(cast<uint16_t>(), cast<uint16_t>() + size());
        case BexType::BT_INT32  : return VectorT(cast< int32_t>(), cast< int32_t>() + size());
        case BexType::BT_UINT32 : return VectorT(cast<uint32_t>(), cast<uint32_t>() + size());
        case BexType::BT_INT64  : return VectorT(cast< int64_t>(), cast< int64_t>() + size());
        case BexType::BT_UINT64 : return VectorT(cast<uint64_t>(), cast<uint64_t>() + size());
        case BexType::BT_FLOAT32: return VectorT(cast<   float>(), cast<   float>() + size());
        case BexType::BT_FLOAT64: return VectorT(cast<  double>(), cast<  double>() + size());
        default: assert(false && "Unknown type assertion.");
        // @formatter:on
        }
        return VectorT();
    }

public:
    Err fromString(std::string const & content);

public:
    template <typename T>
    Err fromVector(std::vector<T> const & content)
    {
        Err const CODE = resize<T>(content.size());
        if (isFailure(CODE)) {
            return CODE;
        }
        assert(isTypeEquals<T>(_type));
        std::copy(content.begin(), content.end(), cast<T>());
        return Err::E_SUCCESS;
    }

public:
    template <typename ... Args>
    std::size_t offset(Args && ... args) const
    {
        assert(static_cast<bool>(_bag));
        switch (_type) {
        // @formatter:off
        case BexType::BT_INT8   : return ((BagInt8   *)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_UINT8  : return ((BagUint8  *)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_INT16  : return ((BagInt16  *)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_UINT16 : return ((BagUint16 *)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_INT32  : return ((BagInt32  *)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_UINT32 : return ((BagUint32 *)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_INT64  : return ((BagInt64  *)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_UINT64 : return ((BagUint64 *)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_FLOAT32: return ((BagFloat32*)_bag.get())->offset(std::forward<Args>(args) ...);
        case BexType::BT_FLOAT64: return ((BagFloat64*)_bag.get())->offset(std::forward<Args>(args) ...);
        default: assert(false && "Unknown type assertion.");
        // @formatter:on
        }
        return 0;
    }
};

// --------------
// Output Stream.
// --------------

template <class CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, Bex const & obj)
{
    return os << obj.toAutoString();
}

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BEX_HPP__

