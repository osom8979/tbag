/**
 * @file   Exception.hpp
 * @brief  Exception class prototype.
 * @author zer0
 * @date   2016-04-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_EXCEPTION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_EXCEPTION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <exception>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

#ifndef __CREATE_EXCEPTION_CLASS
#define __CREATE_EXCEPTION_CLASS(class_name, message)           \
    struct class_name : public std::exception                   \
    {                                                           \
        static constexpr char const * const MESSAGE = message;  \
        virtual const char * what() const noexcept override {   \
            return MESSAGE;                                     \
        }                                                       \
    };
#endif

/**
 * @defgroup __DOXYGEN_GROUP__EXCEPTION_CLASS__ Exception classes
 * @brief List of exception classes.
 * @{
 */

__CREATE_EXCEPTION_CLASS(BadCastException,         "Bad casting exception.");
__CREATE_EXCEPTION_CLASS(DivideByZeroException,    "Divide by zero exception.");
__CREATE_EXCEPTION_CLASS(OutOfRangeException,      "Out of range exception.");
__CREATE_EXCEPTION_CLASS(ContainerEmptyException,  "Container empty exception.");
__CREATE_EXCEPTION_CLASS(NotFoundException,        "Not found exception.");
__CREATE_EXCEPTION_CLASS(NullPointerException,     "Null pointer exception.");
__CREATE_EXCEPTION_CLASS(IllegalArgumentException, "Illegal argument exception.");

/**
 * @}
 */

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_EXCEPTION_EXCEPTION_HPP__

