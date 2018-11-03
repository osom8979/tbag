/**
 * @file   Storage.hpp
 * @brief  Storage class prototype.
 * @author zer0
 * @date   2018-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_STORAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_STORAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/res/DynamicAsset.hpp>
#include <libtbag/string/Environments.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * Storage class prototype.
 *
 * @author zer0
 * @date   2018-11-03
 */
class TBAG_API Storage
{
public:
    using Environments = libtbag::string::Environments;

public:
    /**
     * Storage::Impl class implementation.
     *
     * @author zer0
     * @date   2018-11-03
     */
    struct Impl : private Noncopyable
    {
        Environments envs;

        Impl() { /* EMPTY. */ }
        ~Impl() { /* EMPTY. */ }
    };

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    Storage();
    Storage(std::nullptr_t) TBAG_NOEXCEPT;
    Storage(Storage const & obj) TBAG_NOEXCEPT;
    Storage(Storage && obj) TBAG_NOEXCEPT;
    ~Storage();

public:
    Storage & operator =(Storage const & obj) TBAG_NOEXCEPT;
    Storage & operator =(Storage && obj) TBAG_NOEXCEPT;

public:
    void copy(Storage const & obj) TBAG_NOEXCEPT;
    void swap(Storage & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Storage & lh, Storage & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

    inline Impl       & operator *()       TBAG_NOEXCEPT { return *get(); }
    inline Impl const & operator *() const TBAG_NOEXCEPT { return *get(); }

    inline Impl       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline Impl const * operator ->() const TBAG_NOEXCEPT { return get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(Storage const & x, Storage const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(Storage const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(Storage const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();

public:
};

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_STORAGE_HPP__

