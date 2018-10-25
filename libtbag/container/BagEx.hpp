/**
 * @file   BagEx.hpp
 * @brief  BagEx class prototype.
 * @author zer0
 * @date   2018-10-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAGEX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAGEX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/container/Bag.hpp>

#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <algorithm>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * BagEx class prototype.
 *
 * @author zer0
 * @date   2018-10-22
 *
 * @remarks
 *  Only primary types are supported.
 */
class TBAG_API BagEx
{
public:
    using TypeTable = libtbag::type::TypeTable;

public:
    TBAG_CONSTEXPR static unsigned int const MAX_ELEMENTS_BUFFER_SIZE = 8;

public:
    template <typename T>
    using Allocator = std::allocator<T>;

    template <typename T>
    using BaseBagType = libtbag::container::Bag<T, Allocator<T>, MAX_ELEMENTS_BUFFER_SIZE>;

#define _TBAG_XX(name, symbol, type) \
    using BAG_##name##_TYPE = BaseBagType<type>;
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX

    struct FakeBag { /* EMPTY. */ };

public:
    struct User
    {
        User() { /* EMPTY. */ }
        virtual ~User() { /* EMPTY. */ }
    };

public:
    using SharedBag  = std::shared_ptr<FakeBag>;
    using SharedUser = std::shared_ptr<User>;

private:
    TypeTable  _type;
    SharedBag  _bag;
    SharedUser _user;

public:
    BagEx() TBAG_NOEXCEPT;
    BagEx(std::string const & content);
    BagEx(BagEx const & obj) TBAG_NOEXCEPT;
    BagEx(BagEx && obj) TBAG_NOEXCEPT;
    ~BagEx();

public:
    template <typename Type>
    explicit BagEx(std::vector<Type> const & content) : BagEx()
    {
        if (isFailure(fromVector<Type>(content))) {
            throw std::bad_alloc();
        }
    }

public:
    BagEx & operator =(BagEx const & obj) TBAG_NOEXCEPT;
    BagEx & operator =(BagEx && obj) TBAG_NOEXCEPT;
    BagEx & operator =(std::string const & content);

public:
    template <typename Type>
    BagEx & operator =(std::vector<Type> const & content)
    {
        if (isFailure(fromVector<Type>(content))) {
            throw std::bad_alloc();
        }
        return *this;
    }

public:
    void copy(BagEx const & obj) TBAG_NOEXCEPT;
    void swap(BagEx & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(BagEx & lh, BagEx & rh) TBAG_NOEXCEPT { lh.swap(rh); }

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
    friend inline bool operator <(BagEx const & x, BagEx const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(BagEx const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

public:
    template <typename Type, typename _CastBagType = BaseBagType<Type> >
    std::shared_ptr<_CastBagType> bag() const TBAG_NOEXCEPT
    {
        assert(static_cast<bool>(_bag));

        bool is_same = false;
        switch (_type) {
#define _TBAG_XX(name, symbol, type) \
        case TypeTable::TT_##name:   \
            is_same = std::is_same<Type, type>::value; \
            break;
        TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
        case TypeTable::TT_UNKNOWN:
        default:
            assert(false && "Unknown type.");
        }
        assert(is_same);

        return std::shared_ptr<_CastBagType>(_bag, (_CastBagType*)(_bag.get()));
    }

    template <typename CastUserType>
    std::shared_ptr<CastUserType> user() const TBAG_NOEXCEPT
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(User, CastUserType);
        return std::static_pointer_cast<CastUserType>(_user);
    }

    template <typename UserType, typename ... Args>
    void createUser(Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(User, UserType);
        _user = std::make_shared<UserType>(std::forward<Args>(args) ...);
    }

public:
    inline TypeTable getType() const TBAG_NOEXCEPT
    { return _type; }

    inline char const * getTypeName() const TBAG_NOEXCEPT
    { return libtbag::type::getTypeName(_type); }

    inline std::size_t getTypeSize() const TBAG_NOEXCEPT
    { return libtbag::type::getTypeSize(_type); }

public:
    void clear();

public:
    Err create(TypeTable type);

    template <typename Type>
    Err create()
    {
        return create(libtbag::type::getTypeTable<Type>());
    }

public:
    Err resize(std::size_t i0/**/, std::size_t i1 = 0, std::size_t i2 = 0, std::size_t i3 = 0,
               std::size_t i4 = 0, std::size_t i5 = 0, std::size_t i6 = 0, std::size_t i7 = 0);

public:
    // @formatter:off
    void       * data();
    void const * data() const;
    // @formatter:on

public:
    // @formatter:off
    template <typename CastType> CastType       * castData()       { return (CastType       *)data(); }
    template <typename CastType> CastType const * castData() const { return (CastType const *)data(); }
    // @formatter:on

public:
    std::size_t size() const;
    std::size_t size(std::size_t index) const;
    std::size_t dims() const;
    bool empty() const;

public:
    std::string toString() const;
    std::string toHexString() const;
    std::string toHexBoxString(int line_width = 2*8) const;
    std::string toInfoString() const;
    std::string toAutoString() const;

    template <typename Type>
    std::vector<Type> toVector() const
    {
        auto const * BUFFER = castData<Type>();
        return std::vector<Type>(BUFFER, BUFFER + size());
    }

public:
    Err fromString(std::string const & content);

    template <typename Type>
    Err fromVector(std::vector<Type> const & content)
    {
        static_assert(std::is_pod<Type>::value, "Only primary types are supported.");

        auto code = create<Type>();
        if (isFailure(code)) {
            return code;
        }
        code = resize(content.size());
        if (isFailure(code)) {
            return code;
        }
        std::copy(content.begin(), content.end(), castData<Type>());
        return Err::E_SUCCESS;
    }
};

// --------------
// Output Stream.
// --------------

template <class CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BagEx const & obj)
{
    return os << obj.toAutoString();
}

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAGEX_HPP__

