/**
 * @file   Preferences.hpp
 * @brief  Preferences class prototype.
 * @author zer0
 * @date   2019-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREF_PREFERENCES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREF_PREFERENCES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref {

/**
 * Preferences class prototype.
 *
 * All frequently used settings can be passed as parameters.
 *
 * <code>config.xml</code> format:
 * @code{.xml}
 * <?xml version='1.0' encoding='utf-8' ?>
 * <application>
 *   <loggers>
 *     <!-- insert your logger -->
 *     <logger>
 *       <name>test-logger-file</name>
 *       <sink>stdout</sink>
 *     </logger>
 *     <!-- ... -->
 *   </loggers>
 *
 *   <values>
 *     <!-- insert your properties -->
 *     <value name='key1'>value1</value>
 *     <value name='key2'>value2</value>
 *     <value name='key3'>value3</value>
 *     <!-- ... -->
 *   </values>
 *
 *   <extra>
 *     <!-- insert your custom xml -->
 *     <!-- ... -->
 *   </extra>
 *
 *   <storage root='directory_path'>
 *     <!-- ... -->
 *   </storage>
 * </application>
 * @endcode
 *
 * @author zer0
 * @date   2019-11-03
 */
class TBAG_API Preferences
{
public:
    TBAG_CONSTEXPR static char const * const TAG_APPLICATION = "application";
    TBAG_CONSTEXPR static char const * const TAG_LOGGERS     = "loggers";
    TBAG_CONSTEXPR static char const * const TAG_VALUES      = "values";
    TBAG_CONSTEXPR static char const * const TAG_VALUE       = "value";
    TBAG_CONSTEXPR static char const * const TAG_EXTRA       = "extra";
    TBAG_CONSTEXPR static char const * const TAG_STORAGE     = "storage";

public:
    Preferences();
    Preferences(Preferences const & obj);
    Preferences(Preferences && obj) TBAG_NOEXCEPT;
    ~Preferences();

public:
    Preferences & operator =(Preferences const & obj);
    Preferences & operator =(Preferences && obj) TBAG_NOEXCEPT;

public:
    void copy(Preferences const & obj);
    void swap(Preferences & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Preferences & lh, Preferences & rh) TBAG_NOEXCEPT { lh.swap(rh); }
};

} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREF_PREFERENCES_HPP__

