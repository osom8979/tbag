/**
 * @file   Bag.hpp
 * @brief  Bag class prototype.
 * @author zer0
 * @date   2017-12-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/container/bag/BaseBag.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * Bag class prototype.
 *
 * @author zer0
 * @date   2017-12-08
 */
template <typename T,
          typename ExtType   = void,
          typename UserType  = void,
          typename Allocator = std::allocator<T> >
class Bag
{
public:
    using Type      = T;
    using Extension = ExtType;
    using User      = UserType;
    using TypeTable = type::TypeTable;
    using TypeInfo  = type::TypeInfo<Type>;
    using BaseBag   = bag::BaseBag<Type, Allocator>;

    using SharedBaseBag = std::shared_ptr<BaseBag>;
    using SharedExtMem  = std::shared_ptr<Extension>;
    using SharedUserMem = std::shared_ptr<User>;

    using Dimension              = typename BaseBag::Dimension;
    using Vector                 = typename BaseBag::Vector;
    using dim_type               = typename BaseBag::dim_type;
    using value_type             = typename BaseBag::value_type;
    using allocator_type         = typename BaseBag::allocator_type;
    using reference              = typename BaseBag::reference;
    using const_reference        = typename BaseBag::const_reference;
    using size_type              = typename BaseBag::size_type;
    using difference_type        = typename BaseBag::difference_type;
    using pointer                = typename BaseBag::pointer;
    using const_pointer          = typename BaseBag::const_pointer;
    using iterator               = typename BaseBag::iterator;
    using const_iterator         = typename BaseBag::const_iterator;
    using reverse_iterator       = typename BaseBag::reverse_iterator;
    using const_reverse_iterator = typename BaseBag::const_reverse_iterator;
    using DimensionList          = typename BaseBag::DimensionList;

public:
    TBAG_CONSTEXPR static char const * const type_name() TBAG_NOEXCEPT { return TypeInfo::name(); }
    TBAG_CONSTEXPR static int type_size() TBAG_NOEXCEPT { return TypeInfo::size(); }

    TBAG_CONSTEXPR static int       type_index() TBAG_NOEXCEPT { return TypeInfo::index(); }
    TBAG_CONSTEXPR static TypeTable type_table() TBAG_NOEXCEPT { return TypeInfo::table(); }

    TBAG_CONSTEXPR static Type type_maximum() TBAG_NOEXCEPT { return TypeInfo::maximum(); }
    TBAG_CONSTEXPR static Type type_minimum() TBAG_NOEXCEPT { return TypeInfo::minimum(); }

private:
    SharedBaseBag  _base;
    SharedExtMem   _ext;
    SharedUserMem  _user;

public:
    Bag() : _base(), _ext(), _user()
    { /* EMPTY. */ }

    Bag(Bag const & obj) : Bag()
    { *this = obj; }

    Bag(Bag && obj) : Bag()
    { *this = std::move(obj); }

    virtual ~Bag()
    { /* EMPTY. */ }

public:
    Bag & operator =(Bag const & obj)
    {
        if (this != &obj) {
            _base = obj._base;
            _ext  = obj._ext;
            _user = obj._user;
        }
        return *this;
    }

    Bag & operator =(Bag && obj)
    {
        swap(obj);
        return *this;
    }

public:
    void swap(Bag & obj)
    {
        if (this != &obj) {
            _base.swap(obj._base);
            _ext .swap(obj._ext);
            _user.swap(obj._user);
        }
    }

    inline friend void swap(Bag & lh, Bag & rh) { lh.swap(rh); }
};

using Bagc   = Bag<char>;
using Baguc  = Bag<unsigned char>;
using Bags   = Bag<short>;
using Bagus  = Bag<unsigned short>;
using Bagi   = Bag<int>;
using Bagui  = Bag<unsigned int>;
using Bagl   = Bag<long>;
using Bagul  = Bag<unsigned long>;
using Bagll  = Bag<long long>;
using Bagull = Bag<unsigned long long>;
using Bagf   = Bag<float>;
using Bagd   = Bag<double>;
using Bagld  = Bag<long double>;

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__

