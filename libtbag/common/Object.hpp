/**
 * @file   Object.hpp
 * @brief  Object class prototype.
 * @author zer0
 * @date   2016-06-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_COMMON_OBJECT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_COMMON_OBJECT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace common {

#define IMPLEMENT_GET_CLASS_NAME(name) \
    static constexpr char const * const getClassName() noexcept { \
        return #name; \
    }

/**
 * Object class prototype.
 *
 * @author zer0
 * @date   2016-06-05
 */
class Object
{
public:
    IMPLEMENT_GET_CLASS_NAME(Object);

public:
    typedef std::size_t Id;

private:
    Id _id;

public:
    Object() noexcept
    {
        _id = reinterpret_cast<Id>(this);
    }

    Object(Object const & obj) noexcept : Object()
    {
        // EMPTY.
    }

    Object(Object && obj) noexcept : Object()
    {
        // EMPTY.
    }

    virtual ~Object() noexcept
    {
        // EMPTY.
    }

public:
    inline Object & operator =(Object const & obj) noexcept
    {
        return *this;
    }

    inline Object & operator =(Object && obj) noexcept
    {
        return *this;
    }

public:
    inline bool operator ==(Object const & obj) noexcept
    {
        return (this == &obj);
    }

    inline bool operator !=(Object const & obj) noexcept
    {
        return (this != &obj);
    }

public:
    inline Id getId() const noexcept
    {
        return _id;
    }
};

} // namespace common

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_COMMON_OBJECT_HPP__

