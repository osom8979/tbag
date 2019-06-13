/**
 * @file   Format.hpp
 * @brief  Extended formatting string.
 * @author zer0
 * @date   2017-10-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_FORMAT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_FORMAT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/string/fmt/format.h>
#include <libtbag/string/fmt/ostream.h>

#include <cstdarg>
#include <string>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

TBAG_CONSTEXPR std::size_t const DEFAULT_FORMAT_BUFFER_SIZE = 1024;
TBAG_CONSTEXPR std::size_t const MAX_FORMAT_RESIZE_LOOP_COUNT = 5;

/**
 * Variable-arguments style formatting string.
 */
TBAG_API std::string vformat(char const * f, std::size_t buffer_size_step, va_list & l);

#ifndef TBAG_FORMAT_VA_LIST
#define TBAG_FORMAT_VA_LIST(result, format_string, last_argument_name, buffer_size)     \
    do {                                                                                \
        va_list __args_list__;                                                          \
        va_start(__args_list__, last_argument_name);                                    \
        result = ::libtbag::string::vformat(format_string, buffer_size, __args_list__); \
        va_end(__args_list__);                                                          \
    } while (0)
#endif

/**
 * Printf style formatting string.
 */
TBAG_API std::string format(char const * f, ...);

/**
 * Fmt style formatting string.
 */
template <typename ... Args>
std::string fformat(std::string const & f, Args && ... args)
{
    return ::fmt::format(f, std::forward<Args>(args) ...);
}

/**
 * Extended formatting string.
 *
 * @author zer0
 * @date   2017-10-21
 */
class TBAG_API Format
{
public:
    TBAG_CONSTEXPR static char const DEFAULT_ESCAPE = '$';

public:
    enum class FormatMode
    {
        FM_NORMAL,
        FM_ESCAPE,
    };

private:
    char _escape;

public:
    Format();
    Format(char escape);
    Format(Format const & obj);
    Format(Format && obj);
    virtual ~Format();

public:
    inline void setEscape(char escape) TBAG_NOEXCEPT { _escape = escape; }
    inline char getEscape() const TBAG_NOEXCEPT { return _escape; }

public:
    Format & operator =(Format const & obj);
    Format & operator =(Format && obj);

public:
    void swap(Format & obj);

public:
    std::string convert(std::string const & source) const;

public:
    template <typename ... Args>
    inline std::string format(std::string const & f, Args && ... args) const
    {
        return libtbag::string::format(convert(f).c_str(), std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    inline std::string fformat(std::string const & f, Args && ... args) const
    {
        return libtbag::string::fformat(convert(f), std::forward<Args>(args) ...);
    }

public:
    virtual int onEscape(std::string const & source, std::size_t index, std::string & output) const;
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_FORMAT_HPP__

