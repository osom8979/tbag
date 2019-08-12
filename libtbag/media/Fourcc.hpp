/**
 * @file   Fourcc.hpp
 * @brief  Fourcc class prototype.
 * @author zer0
 * @date   2019-08-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEDIA_FOURCC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEDIA_FOURCC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>
#include <string>

/**
 * @def TBAG_FOURCC
 *
 * Convert four characters to a FourCC code.
 *
 * @see <http://www.fourcc.org/yuv.php>
 * @see <http://v4l2spec.bytesex.org/spec/book1.htm>
 * @see <http://developer.apple.com/quicktime/icefloe/dispatch020.html>
 * @see <http://msdn.microsoft.com/library/windows/desktop/dd206750.aspx#nv12>
 * @see <http://people.xiph.org/~xiphmont/containers/nut/nut4cc.txt>
 */
#ifndef TBAG_FOURCC
#define TBAG_FOURCC(a, b, c, d)       \
    ((static_cast<uint32_t>(a))     | \
     (static_cast<uint32_t>(b)<< 8) | \
     (static_cast<uint32_t>(c)<<16) | \
     (static_cast<uint32_t>(d)<<24))
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace media {

/**
* @defgroup __DOXYGEN_GROUP__FOURCC__ FOURCC.
* @brief List of FourCC.
* @{
*/

// 9 Primary YUV formats: 5 planar, 2 biplanar, 2 packed.
TBAG_CONSTEXPR uint32_t const FOURCC_I420 = TBAG_FOURCC('I', '4', '2', '0');
TBAG_CONSTEXPR uint32_t const FOURCC_I422 = TBAG_FOURCC('I', '4', '2', '2');
TBAG_CONSTEXPR uint32_t const FOURCC_I444 = TBAG_FOURCC('I', '4', '4', '4');
TBAG_CONSTEXPR uint32_t const FOURCC_I411 = TBAG_FOURCC('I', '4', '1', '1');
TBAG_CONSTEXPR uint32_t const FOURCC_I400 = TBAG_FOURCC('I', '4', '0', '0');
TBAG_CONSTEXPR uint32_t const FOURCC_NV21 = TBAG_FOURCC('N', 'V', '2', '1');
TBAG_CONSTEXPR uint32_t const FOURCC_NV12 = TBAG_FOURCC('N', 'V', '1', '2');
TBAG_CONSTEXPR uint32_t const FOURCC_YUY2 = TBAG_FOURCC('Y', 'U', 'Y', '2');
TBAG_CONSTEXPR uint32_t const FOURCC_UYVY = TBAG_FOURCC('U', 'Y', 'V', 'Y');

// 2 Secondary YUV formats: row biplanar.
TBAG_CONSTEXPR uint32_t const FOURCC_M420 = TBAG_FOURCC('M', '4', '2', '0');

// 9 Primary RGB formats: 4 32 bpp, 2 24 bpp, 3 16 bpp.
TBAG_CONSTEXPR uint32_t const FOURCC_ARGB = TBAG_FOURCC('A', 'R', 'G', 'B');
TBAG_CONSTEXPR uint32_t const FOURCC_BGRA = TBAG_FOURCC('B', 'G', 'R', 'A');
TBAG_CONSTEXPR uint32_t const FOURCC_ABGR = TBAG_FOURCC('A', 'B', 'G', 'R');
TBAG_CONSTEXPR uint32_t const FOURCC_24BG = TBAG_FOURCC('2', '4', 'B', 'G');
TBAG_CONSTEXPR uint32_t const FOURCC_RAW  = TBAG_FOURCC('r', 'a', 'w', ' ');
TBAG_CONSTEXPR uint32_t const FOURCC_RGBA = TBAG_FOURCC('R', 'G', 'B', 'A');
TBAG_CONSTEXPR uint32_t const FOURCC_RGBP = TBAG_FOURCC('R', 'G', 'B', 'P');  // bgr565.
TBAG_CONSTEXPR uint32_t const FOURCC_RGBO = TBAG_FOURCC('R', 'G', 'B', 'O');  // abgr1555.
TBAG_CONSTEXPR uint32_t const FOURCC_R444 = TBAG_FOURCC('R', '4', '4', '4');  // argb4444.

// 4 Secondary RGB formats: 4 Bayer Patterns.
TBAG_CONSTEXPR uint32_t const FOURCC_RGGB = TBAG_FOURCC('R', 'G', 'G', 'B');
TBAG_CONSTEXPR uint32_t const FOURCC_BGGR = TBAG_FOURCC('B', 'G', 'G', 'R');
TBAG_CONSTEXPR uint32_t const FOURCC_GRBG = TBAG_FOURCC('G', 'R', 'B', 'G');
TBAG_CONSTEXPR uint32_t const FOURCC_GBRG = TBAG_FOURCC('G', 'B', 'R', 'G');

// 1 Primary Compressed YUV format.
TBAG_CONSTEXPR uint32_t const FOURCC_MJPG = TBAG_FOURCC('M', 'J', 'P', 'G');

// 5 Auxiliary YUV variations: 3 with U and V planes are swapped, 1 Alias.
TBAG_CONSTEXPR uint32_t const FOURCC_YV12 = TBAG_FOURCC('Y', 'V', '1', '2');
TBAG_CONSTEXPR uint32_t const FOURCC_YV16 = TBAG_FOURCC('Y', 'V', '1', '6');
TBAG_CONSTEXPR uint32_t const FOURCC_YV24 = TBAG_FOURCC('Y', 'V', '2', '4');
TBAG_CONSTEXPR uint32_t const FOURCC_YU12 = TBAG_FOURCC('Y', 'U', '1', '2');  // Linux version of I420.
TBAG_CONSTEXPR uint32_t const FOURCC_J420 = TBAG_FOURCC('J', '4', '2', '0');
TBAG_CONSTEXPR uint32_t const FOURCC_J400 = TBAG_FOURCC('J', '4', '0', '0');

// 14 Auxiliary aliases. getCanonicalFourCC() maps these to canonical FOURCC.
TBAG_CONSTEXPR uint32_t const FOURCC_IYUV = TBAG_FOURCC('I', 'Y', 'U', 'V');  // Alias for I420.
TBAG_CONSTEXPR uint32_t const FOURCC_YU16 = TBAG_FOURCC('Y', 'U', '1', '6');  // Alias for I422.
TBAG_CONSTEXPR uint32_t const FOURCC_YU24 = TBAG_FOURCC('Y', 'U', '2', '4');  // Alias for I444.
TBAG_CONSTEXPR uint32_t const FOURCC_YUYV = TBAG_FOURCC('Y', 'U', 'Y', 'V');  // Alias for YUY2.
TBAG_CONSTEXPR uint32_t const FOURCC_YUVS = TBAG_FOURCC('y', 'u', 'v', 's');  // Alias for YUY2 on Mac.
TBAG_CONSTEXPR uint32_t const FOURCC_HDYC = TBAG_FOURCC('H', 'D', 'Y', 'C');  // Alias for UYVY.
TBAG_CONSTEXPR uint32_t const FOURCC_2VUY = TBAG_FOURCC('2', 'v', 'u', 'y');  // Alias for UYVY on Mac.
TBAG_CONSTEXPR uint32_t const FOURCC_JPEG = TBAG_FOURCC('J', 'P', 'E', 'G');  // Alias for MJPG.
TBAG_CONSTEXPR uint32_t const FOURCC_DMB1 = TBAG_FOURCC('d', 'm', 'b', '1');  // Alias for MJPG on Mac.
TBAG_CONSTEXPR uint32_t const FOURCC_BA81 = TBAG_FOURCC('B', 'A', '8', '1');  // Alias for BGGR.
TBAG_CONSTEXPR uint32_t const FOURCC_RGB3 = TBAG_FOURCC('R', 'G', 'B', '3');  // Alias for RAW.
TBAG_CONSTEXPR uint32_t const FOURCC_BGR3 = TBAG_FOURCC('B', 'G', 'R', '3');  // Alias for 24BG.
TBAG_CONSTEXPR uint32_t const FOURCC_CM32 = TBAG_FOURCC(0, 0, 0, 32);  // BGRA kCMPixelFormat_32ARGB
TBAG_CONSTEXPR uint32_t const FOURCC_CM24 = TBAG_FOURCC(0, 0, 0, 24);  // RAW kCMPixelFormat_24RGB

// 1 Auxiliary compressed YUV format set aside for capturer.
TBAG_CONSTEXPR uint32_t const FOURCC_H264 = TBAG_FOURCC('H', '2', '6', '4');

// ETC.
TBAG_CONSTEXPR uint32_t const FOURCC_ANY = 0xFFFFFFFF;

/**
 * @}
 */

/**
 * Converts fourcc aliases into canonical ones.
 */
TBAG_API uint32_t getCanonicalFourcc(uint32_t fourcc);

/**
 * Get FourCC code as a string.
 */
TBAG_API std::string getFourccName(uint32_t fourcc);

} // namespace media

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEDIA_FOURCC_HPP__

