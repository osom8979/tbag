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

#include <string>

#define IMPLEMENT_GET_INSTANCE_NAME_AND_QUALIFIER(name, qualifier) \
    virtual std::string getInstanceName() qualifier \
    { \
        static std::string const INSTANCE_NAME = name; \
        return INSTANCE_NAME; \
    }

#define IMPLEMENT_GET_INSTANCE_NAME(name) \
    IMPLEMENT_GET_INSTANCE_NAME_AND_QUALIFIER(name, const override)

#define IMPLEMENT_GET_INSTANCE_CURRENT_FILE_NAME \
    IMPLEMENT_GET_INSTANCE_NAME(CURRENT_FILE_NAME_WITH_REMOVED_EXTENSION)

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// TODO: Forward declaration.

/**
 * Object class prototype.
 *
 * @author zer0
 * @date   2016-06-05
 */
class Object /** : public Noncopyable */
{
private:
    // TODO: Insert member variables.

public:
    Object();
    Object(Object const & obj);
    Object(Object && obj);
    virtual ~Object();

public:
    Object & operator =(Object const & obj);
    Object & operator =(Object && obj);

public:
    Object & copy(Object const & obj);
    void swap(Object & obj);
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_COMMON_OBJECT_HPP__

