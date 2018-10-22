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
#include <libtbag/container/Bag.hpp>

#include <cassert>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * BagEx class prototype.
 *
 * @author zer0
 * @date   2018-10-22
 */
template <typename T, typename A = std::allocator<T> >
class TBAG_API BagEx
{
public:
    struct User
    {
        User() { /* EMPTY. */ }
        virtual ~User() { /* EMPTY. */ }
    };

public:
    using BagType    = libtbag::container::Bag<T, A>;
    using SharedBag  = std::shared_ptr<BagType>;
    using SharedUser = std::shared_ptr<User>;

    using value_type        = typename BagType::value_type;
    using allocator_type    = typename BagType::allocator_type;
    using reference         = typename BagType::reference;
    using const_reference   = typename BagType::const_reference;
    using size_type         = typename BagType::size_type;
    using difference_type   = typename BagType::difference_type;
    using pointer           = typename BagType::pointer;
    using const_pointer     = typename BagType::const_pointer;

    using       iterator = typename BagType::iterator;
    using const_iterator = typename BagType::const_iterator;

    using       reverse_iterator = typename BagType::reverse_iterator;
    using const_reverse_iterator = typename BagType::const_reverse_iterator;

private:
    SharedBag  _bag;
    SharedUser _user;

public:
    BagEx() TBAG_NOEXCEPT : _bag(std::make_shared<BagType>()), _user(nullptr)
    { /* EMPTY. */ }

    explicit BagEx(std::nullptr_t) TBAG_NOEXCEPT : _bag(nullptr), _user(nullptr)
    { /* EMPTY. */ }

    explicit BagEx(SharedBag const & bag) TBAG_NOEXCEPT : _bag(bag), _user(nullptr)
    { /* EMPTY. */ }

    explicit BagEx(SharedUser const & user) TBAG_NOEXCEPT : _bag(nullptr), _user(user)
    { /* EMPTY. */ }

    explicit BagEx(SharedBag const & bag, SharedUser const & user) TBAG_NOEXCEPT : _bag(bag), _user(user)
    { /* EMPTY. */ }

    BagEx(BagEx const & obj) TBAG_NOEXCEPT : BagEx(nullptr)
    { (*this) = obj; }

    BagEx(BagEx && obj) TBAG_NOEXCEPT : BagEx(nullptr)
    { (*this) = std::move(obj); }

    ~BagEx()
    { /* EMPTY. */ }

public:
    BagEx & operator =(BagEx const & obj) TBAG_NOEXCEPT
    {
        copy(obj);
        return *this;
    }

    BagEx & operator =(BagEx && obj) TBAG_NOEXCEPT
    {
        swap(obj);
        return *this;
    }

public:
    void copy(BagEx const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _bag = obj._bag;
            _user = obj._user;;
        }
    }

    void swap(BagEx & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _bag.swap(obj._bag);
            _user.swap(obj._user);
        }
    }

public:
    inline friend void swap(BagEx & lh, BagEx & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline BagType       * get()       TBAG_NOEXCEPT { return _bag.get(); }
    inline BagType const * get() const TBAG_NOEXCEPT { return _bag.get(); }

    inline BagType       & operator *()       TBAG_NOEXCEPT { return *_bag; }
    inline BagType const & operator *() const TBAG_NOEXCEPT { return *_bag; }

    inline BagType       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline BagType const * operator ->() const TBAG_NOEXCEPT { return get(); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_bag); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    SharedBag       & bag()       TBAG_NOEXCEPT { return _bag; }
    SharedBag const & bag() const TBAG_NOEXCEPT { return _bag; }

public:
    SharedUser       & user()       TBAG_NOEXCEPT { return _user; }
    SharedUser const & user() const TBAG_NOEXCEPT { return _user; }

public:
    template <typename CastType>
    std::shared_ptr<CastType> castUser() const TBAG_NOEXCEPT
    {
        return std::static_pointer_cast<CastType>(_user);
    }

public:
    pointer data()
    {
        assert(exists());
        return _bag->data();
    }

    const_pointer data() const
    {
        assert(exists());
        return _bag->data();
    }

public:
    template <typename ... Args>
    void resize(Args && ... list)
    {
        assert(exists());
        ((BagType*)get())->resize(std::forward<Args>(list) ...);
    }

    void clear()
    {
        assert(exists());
        _bag->clear();
    }

public:
    TBAG_CONSTEXPR static size_type elem_size() TBAG_NOEXCEPT
    {
        return BagType::elem_size();
    }

    size_type max_size() const
    {
        assert(exists());
        return _bag->max_size();
    }

    size_type size() const
    {
        assert(exists());
        return _bag->size();
    }

    size_type size(size_type index) const
    {
        assert(exists());
        return _bag->size(index);
    }

    bool empty() const
    {
        assert(exists());
        return _bag->empty();
    }

public:
    reference at(size_type index)
    {
        assert(exists());
        return _bag->at(index);
    }

    const_reference at(size_type index) const
    {
        assert(exists());
        return _bag->at(index);
    }

    reference operator[](size_type index)
    {
        assert(exists());
        return _bag->at(index);
    }

    const_reference operator[](size_type index) const
    {
        assert(exists());
        return _bag->at(index);
    }

    template <typename ... Args>
    reference index(Args && ... args)
    {
        assert(exists());
        return _bag->index(std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    const_reference index(Args && ... args) const
    {
        assert(exists());
        return _bag->index(std::forward<Args>(args) ...);
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAGEX_HPP__

