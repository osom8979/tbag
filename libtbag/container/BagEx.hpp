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
#include <memory>
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
    BagEx(BagEx const & obj) TBAG_NOEXCEPT;
    BagEx(BagEx && obj) TBAG_NOEXCEPT;
    ~BagEx();

public:
    BagEx & operator =(BagEx const & obj) TBAG_NOEXCEPT;
    BagEx & operator =(BagEx && obj) TBAG_NOEXCEPT;

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
    inline SharedBag       & atBag()       TBAG_NOEXCEPT { return _bag; }
    inline SharedBag const & atBag() const TBAG_NOEXCEPT { return _bag; }

    inline SharedUser       & atUser()       TBAG_NOEXCEPT { return _user; }
    inline SharedUser const & atUser() const TBAG_NOEXCEPT { return _user; }

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

    template <typename UserType>
    void createUser(UserType * user)
    {
        _user.reset(user, [](UserType * u){ delete ((UserType*)u); });
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
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAGEX_HPP__

