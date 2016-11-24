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
#include <libtbag/predef.hpp>
#include <libtbag/id/Id.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace common {

#ifndef IMPLEMENT_GET_CLASS_NAME
#define IMPLEMENT_GET_CLASS_NAME(name, qualify) \
    static TBAG_CONSTEXPR char const * const getClassName() TBAG_NOEXCEPT \
    { return #name; } \
    virtual char const * const getName() qualify \
    { return getClassName(); }
#endif

#ifndef IMPLEMENT_GET_PARENT_CLASS_NAME
#define IMPLEMENT_GET_PARENT_CLASS_NAME(name) \
    IMPLEMENT_GET_CLASS_NAME(name, const TBAG_NOEXCEPT)
#endif

#ifndef IMPLEMENT_GET_CHILD_CLASS_NAME
#define IMPLEMENT_GET_CHILD_CLASS_NAME(name) \
    IMPLEMENT_GET_CLASS_NAME(name, const TBAG_NOEXCEPT override)
#endif

/**
 * Object class prototype.
 *
 * @author zer0
 * @date   2016-06-05
 */
class TBAG_API Object
{
public:
    IMPLEMENT_GET_PARENT_CLASS_NAME(Object);

public:
    using Id = id::Id;

private:
    Id _id;

public:
    Object() TBAG_NOEXCEPT : _id(reinterpret_cast<Id>(this))
    { /* EMPTY. */ }
    Object(Object const & obj) TBAG_NOEXCEPT : Object()
    { /* EMPTY. */ }
    Object(Object && obj) TBAG_NOEXCEPT : Object()
    { /* EMPTY. */ }
    virtual ~Object() TBAG_NOEXCEPT
    { /* EMPTY. */ }

public:
    inline Object & operator =(Object const & obj) TBAG_NOEXCEPT
    { return *this; }
    inline Object & operator =(Object && obj) TBAG_NOEXCEPT
    { return *this; }

public:
    inline bool operator ==(Object const & obj) TBAG_NOEXCEPT
    { return (this == &obj); }
    inline bool operator !=(Object const & obj) TBAG_NOEXCEPT
    { return (this != &obj); }

public:
    inline Id getId() const TBAG_NOEXCEPT
    { return _id; }
};

} // namespace common

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_COMMON_OBJECT_HPP__

