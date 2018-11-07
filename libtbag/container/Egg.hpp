/**
 * @file   Egg.hpp
 * @brief  Egg class prototype.
 * @author zer0
 * @date   2018-10-22
 * @date   2018-11-07 (Rename: BagEx -> Egg)
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
 * Type table of Egg class.
 *
 * @author zer0
 * @date   2018-11-07
 */
enum class EggTypeTable : int
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

template <typename T, EggTypeTable egg_type>
struct EggInfo : public std::true_type
{
    using type = T;
    TBAG_CONSTEXPR static EggTypeTable const value = egg_type;
    TBAG_CONSTEXPR static std::size_t const size = sizeof(T);
};

template <>
struct EggInfo<void, EggTypeTable::BT_NONE> : public std::false_type
{
    using type = void;
    TBAG_CONSTEXPR static EggTypeTable const value = EggTypeTable::BT_NONE;
    TBAG_CONSTEXPR static std::size_t const size = 0;
};

// @formatter:off
template <typename T> struct EggTypeInfo : public EggInfo<    void, EggTypeTable::BT_NONE   > { /* EMPTY. */ };
template <> struct EggTypeInfo<  int8_t> : public EggInfo<  int8_t, EggTypeTable::BT_INT8   > { /* EMPTY. */ };
template <> struct EggTypeInfo< uint8_t> : public EggInfo< uint8_t, EggTypeTable::BT_UINT8  > { /* EMPTY. */ };
template <> struct EggTypeInfo< int16_t> : public EggInfo< int16_t, EggTypeTable::BT_INT16  > { /* EMPTY. */ };
template <> struct EggTypeInfo<uint16_t> : public EggInfo<uint16_t, EggTypeTable::BT_UINT16 > { /* EMPTY. */ };
template <> struct EggTypeInfo< int32_t> : public EggInfo< int32_t, EggTypeTable::BT_INT32  > { /* EMPTY. */ };
template <> struct EggTypeInfo<uint32_t> : public EggInfo<uint32_t, EggTypeTable::BT_UINT32 > { /* EMPTY. */ };
template <> struct EggTypeInfo< int64_t> : public EggInfo< int64_t, EggTypeTable::BT_INT64  > { /* EMPTY. */ };
template <> struct EggTypeInfo<uint64_t> : public EggInfo<uint64_t, EggTypeTable::BT_UINT64 > { /* EMPTY. */ };
template <> struct EggTypeInfo<   float> : public EggInfo<   float, EggTypeTable::BT_FLOAT32> { /* EMPTY. */ };
template <> struct EggTypeInfo<  double> : public EggInfo<  double, EggTypeTable::BT_FLOAT64> { /* EMPTY. */ };
// @formatter:on

// @formatter:off
template <EggTypeTable egg_type> struct EggEnumInfo      : public EggInfo<    void, EggTypeTable::BT_NONE   > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_INT8   > : public EggInfo<  int8_t, EggTypeTable::BT_INT8   > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_UINT8  > : public EggInfo< uint8_t, EggTypeTable::BT_UINT8  > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_INT16  > : public EggInfo< int16_t, EggTypeTable::BT_INT16  > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_UINT16 > : public EggInfo<uint16_t, EggTypeTable::BT_UINT16 > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_INT32  > : public EggInfo< int32_t, EggTypeTable::BT_INT32  > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_UINT32 > : public EggInfo<uint32_t, EggTypeTable::BT_UINT32 > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_INT64  > : public EggInfo< int64_t, EggTypeTable::BT_INT64  > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_UINT64 > : public EggInfo<uint64_t, EggTypeTable::BT_UINT64 > { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_FLOAT32> : public EggInfo<   float, EggTypeTable::BT_FLOAT32> { /* EMPTY. */ };
template <> struct EggEnumInfo<EggTypeTable::BT_FLOAT64> : public EggInfo<  double, EggTypeTable::BT_FLOAT64> { /* EMPTY. */ };
// @formatter:on

// @formatter:off
static_assert(EggTypeInfo<  int8_t>::size == 1,    "int8_t size is not correct.");
static_assert(EggTypeInfo< uint8_t>::size == 1,   "uint8_t size is not correct.");
static_assert(EggTypeInfo< int16_t>::size == 2,   "int16_t size is not correct.");
static_assert(EggTypeInfo<uint16_t>::size == 2,  "uint16_t size is not correct.");
static_assert(EggTypeInfo< int32_t>::size == 4,   "int32_t size is not correct.");
static_assert(EggTypeInfo<uint32_t>::size == 4,  "uint32_t size is not correct.");
static_assert(EggTypeInfo< int64_t>::size == 8,   "int64_t size is not correct.");
static_assert(EggTypeInfo<uint64_t>::size == 8,  "uint64_t size is not correct.");
static_assert(EggTypeInfo<   float>::size == 4, "float32_t size is not correct.");
static_assert(EggTypeInfo<  double>::size == 8, "float64_t size is not correct.");
// @formatter:on

template <typename T>
TBAG_CONSTEXPR inline EggTypeTable getEggType() TBAG_NOEXCEPT
{
    return EggTypeInfo<T>::value;
}

template <typename T>
inline bool isTypeEquals(EggTypeTable type) TBAG_NOEXCEPT
{
    return type == getEggType<T>();
}

inline char const * getEggTypeName(EggTypeTable type) TBAG_NOEXCEPT
{
    switch (type) {
    // @formatter:off
    case EggTypeTable::BT_NONE   : return    "NONE";
    case EggTypeTable::BT_INT8   : return    "INT8";
    case EggTypeTable::BT_UINT8  : return   "UINT8";
    case EggTypeTable::BT_INT16  : return   "INT16";
    case EggTypeTable::BT_UINT16 : return  "UINT16";
    case EggTypeTable::BT_INT32  : return   "INT32";
    case EggTypeTable::BT_UINT32 : return  "UINT32";
    case EggTypeTable::BT_INT64  : return   "INT64";
    case EggTypeTable::BT_UINT64 : return  "UINT64";
    case EggTypeTable::BT_FLOAT32: return "FLOAT32";
    case EggTypeTable::BT_FLOAT64: return "FLOAT64";
    default:                  return "UNKNOWN";
    // @formatter:on
    }
}

/**
 * Egg is an extension class of Bag.
 *
 * @author zer0
 * @date   2018-10-22
 * @date   2018-11-07 (Rename: BagEx -> Egg)
 *
 * @remarks
 *  Only primary types are supported.
 */
class TBAG_API Egg
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
    EggTypeTable    _type;
    SharedBag  _bag;
    SharedUser _user;

public:
    Egg() TBAG_NOEXCEPT;
    Egg(std::string const & content);
    Egg(char const * content);
    Egg(Egg const & obj) TBAG_NOEXCEPT;
    Egg(Egg && obj) TBAG_NOEXCEPT;
    ~Egg();

public:
    template <typename Type>
    Egg(std::vector<Type> const & content) : Egg()
    {
        if (isFailure(fromVector<Type>(content))) {
            throw std::bad_alloc();
        }
    }

    template <typename Type>
    Egg(std::initializer_list<Type> list) : Egg()
    {
        if (isFailure(resize<Type>(list.size()))) {
            throw std::bad_alloc();
        }
        std::copy(list.begin(), list.end(), cast<Type>());
    }

public:
    Egg & operator =(Egg const & obj) TBAG_NOEXCEPT;
    Egg & operator =(Egg && obj) TBAG_NOEXCEPT;

public:
    Egg & operator =(std::string const & content);
    Egg & operator =(char const * content);

public:
    template <typename Type>
    Egg & operator =(std::vector<Type> const & content)
    {
        if (isFailure(fromVector<Type>(content))) {
            throw std::bad_alloc();
        }
        return *this;
    }

public:
    void copy(Egg const & obj) TBAG_NOEXCEPT;
    void swap(Egg & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Egg & lh, Egg & rh) TBAG_NOEXCEPT { lh.swap(rh); }

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
    friend inline bool operator <(Egg const & x, Egg const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(Egg const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(Egg const & obj) const TBAG_NOEXCEPT
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
    inline EggTypeTable getType() const TBAG_NOEXCEPT
    { return _type; }

    inline char const * getTypeName() const TBAG_NOEXCEPT
    { return getEggTypeName(_type); }

public:
    void clear();

public:
    Err create(EggTypeTable type);

public:
    template <typename T>
    Err create()
    { return create(getEggType<T>()); }

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
        case EggTypeTable::BT_INT8   : std::copy(data, data + size, cast<  int8_t>() + offset); break;
        case EggTypeTable::BT_UINT8  : std::copy(data, data + size, cast< uint8_t>() + offset); break;
        case EggTypeTable::BT_INT16  : std::copy(data, data + size, cast< int16_t>() + offset); break;
        case EggTypeTable::BT_UINT16 : std::copy(data, data + size, cast<uint16_t>() + offset); break;
        case EggTypeTable::BT_INT32  : std::copy(data, data + size, cast< int32_t>() + offset); break;
        case EggTypeTable::BT_UINT32 : std::copy(data, data + size, cast<uint32_t>() + offset); break;
        case EggTypeTable::BT_INT64  : std::copy(data, data + size, cast< int64_t>() + offset); break;
        case EggTypeTable::BT_UINT64 : std::copy(data, data + size, cast<uint64_t>() + offset); break;
        case EggTypeTable::BT_FLOAT32: std::copy(data, data + size, cast<   float>() + offset); break;
        case EggTypeTable::BT_FLOAT64: std::copy(data, data + size, cast<  double>() + offset); break;
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
        case EggTypeTable::BT_INT8   : return VectorT(cast<  int8_t>(), cast<  int8_t>() + size());
        case EggTypeTable::BT_UINT8  : return VectorT(cast< uint8_t>(), cast< uint8_t>() + size());
        case EggTypeTable::BT_INT16  : return VectorT(cast< int16_t>(), cast< int16_t>() + size());
        case EggTypeTable::BT_UINT16 : return VectorT(cast<uint16_t>(), cast<uint16_t>() + size());
        case EggTypeTable::BT_INT32  : return VectorT(cast< int32_t>(), cast< int32_t>() + size());
        case EggTypeTable::BT_UINT32 : return VectorT(cast<uint32_t>(), cast<uint32_t>() + size());
        case EggTypeTable::BT_INT64  : return VectorT(cast< int64_t>(), cast< int64_t>() + size());
        case EggTypeTable::BT_UINT64 : return VectorT(cast<uint64_t>(), cast<uint64_t>() + size());
        case EggTypeTable::BT_FLOAT32: return VectorT(cast<   float>(), cast<   float>() + size());
        case EggTypeTable::BT_FLOAT64: return VectorT(cast<  double>(), cast<  double>() + size());
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
        case EggTypeTable::BT_INT8   : return ((BagInt8   *)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_UINT8  : return ((BagUint8  *)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_INT16  : return ((BagInt16  *)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_UINT16 : return ((BagUint16 *)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_INT32  : return ((BagInt32  *)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_UINT32 : return ((BagUint32 *)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_INT64  : return ((BagInt64  *)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_UINT64 : return ((BagUint64 *)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_FLOAT32: return ((BagFloat32*)_bag.get())->offset(std::forward<Args>(args) ...);
        case EggTypeTable::BT_FLOAT64: return ((BagFloat64*)_bag.get())->offset(std::forward<Args>(args) ...);
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
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, Egg const & obj)
{
    return os << obj.toAutoString();
}

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BEX_HPP__

