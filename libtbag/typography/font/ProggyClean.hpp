/**
 * @file   ProggyClean.hpp
 * @brief  ProggyClean class prototype.
 * @author zer0
 * @date   2017-11-18
 *
 * @remarks
 *  ProggyClean.ttf font binary.
 *
 *  This file is Zip compressed and then Base64 encoded.
 *
 *  MIT license:
 *  @code
 *   Copyright (c) 2004, 2005 Tristan Grimmer
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *   DEALINGS IN THE SOFTWARE.
 *  @endcode
 *
 * @see <http://upperbounds.net>
 * @see <http://www.upperbounds.net/download/ProggyClean.ttf.zip>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_PROGGYCLEAN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_PROGGYCLEAN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {
namespace font       {

TBAG_API std::string  getCompressedProggyClean();
TBAG_API std::string  getProggyCleanMd5();
TBAG_API util::Buffer getProggyClean();

} // namespace font
} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_PROGGYCLEAN_HPP__

