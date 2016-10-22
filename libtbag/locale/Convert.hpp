/**
 * @file   Convert.hpp
 * @brief  Convert class prototype.
 * @author zer0
 * @date   2016-07-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_CONVERT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_CONVERT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <exception>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale  {

/**
 * Convert exception class.
 *
 * @author zer0
 * @date   2016-07-03
 */
struct ConvertException : public std::exception
{
    std::string _message;

    ConvertException(std::string const & msg) : _message(msg)
    { /* EMPTY. */ }
    virtual ~ConvertException()
    { /* EMPTY. */ }

    virtual const char * what() const noexcept override
    { return _message.c_str(); }
};

TBAG_EXPORTS std::vector<std::string> getAvailableConverterNames();

/**
 * Convert from UTF-8 string.
 */
TBAG_EXPORTS std::string convertFromUtf8(std::string const & utf8
                                       , std::string const & to_charset) throw (ConvertException);

/**
 * Convert to UTF-8 string.
 */
TBAG_EXPORTS std::string convertToUtf8(std::string const & from_string
                                     , std::string const & from_charset) throw (ConvertException);

} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_CONVERT_HPP__

