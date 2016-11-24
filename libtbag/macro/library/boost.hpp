/**
 * @file   boost.hpp
 * @brief  boost library config.
 * @author zer0
 * @date   2016-11-24
 *
 * @remarks
 *  Predefine collection header: <boost/predef.h>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LIBRARY_BOOST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LIBRARY_BOOST_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// Don't using BOOST_ALL_DYN_LINK macro.
//#define BOOST_ALL_DYN_LINK

#if 0
# define BOOST_FILESYSTEM_DYN_LINK
# define BOOST_LOG_DYN_LINK
#endif

#if 0
# error "Don't using boost.test from the main project."
# define BOOST_TEST_DYN_LINK
# define BOOST_TEST_MODULE WorldUnitTester
#endif

#if 0
/**
 * @def BOOST_LOG_WITHOUT_EVENT_LOG
 *
 * IMPORTANT: In case if message compiler detection fails for some reason,
 * you can explicitly disable support for event log backend by defining the
 * BOOST_LOG_WITHOUT_EVENT_LOG configuration macro when building the library.
 *
 * @see http://www.boost.org/doc/libs/1_57_0_b1/libs/log/doc/html/log/installation.html
 */
# define BOOST_LOG_WITHOUT_EVENT_LOG
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LIBRARY_BOOST_HPP__

