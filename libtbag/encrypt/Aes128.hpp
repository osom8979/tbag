/**
 * @file   Aes128.hpp
 * @brief  Aes128 class prototype.
 * @author zer0
 * @date   2016-11-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_AES128_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_AES128_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

/**
 * Aes128 class prototype.
 *
 * @author zer0
 * @date   2016-11-18
 */
class TBAG_API Aes128 : public Noncopyable
{
public:
    Aes128();
    ~Aes128();
};

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ENCRYPT_AES128_HPP__

