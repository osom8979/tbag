/**
 * @file   Resolution.hpp
 * @brief  Graphics display resolution.
 * @author zer0
 * @date   2018-07-10
 * * @see <https://en.wikipedia.org/wiki/Graphics_display_resolution>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_RESOLUTION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_RESOLUTION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

# /****************************************************/
# /* Parameter syntax:                                */
# /*   _XX(define, name, width, height, aspect_ratio) */
# /****************************************************/

#ifndef HIGH_DEFINITION_MAP
#define HIGH_DEFINITION_MAP(_XX) \
    _XX(        NHD,     nHD,  640,  360, 16:9) \
    _XX(        QHD,     qHD,  960,  540, 16:9) \
    _XX(         HD,      HD, 1280,  720, 16:9) \
    _XX(    HD_PLUS,     HD+, 1600,  900, 16:9) \
    _XX(        FHD,     FHD, 1920, 1080, 16:9) \
    _XX(       WQHD,    WQHD, 2560, 1440, 16:9) \
    _XX(   QHD_PLUS,    QHD+, 3200, 1800, 16:9) \
    _XX(     UHD_4K,  4K UHD, 3840, 2160, 16:9) \
    _XX(UHD_PLUS_5K, 5K UHD+, 5120, 2880, 16:9) \
    _XX(     UHD_8K,  8K UHD, 7680, 4320, 16:9) \
    /* -- END -- */
#endif

#ifndef VIDEO_GRAPHICS_ARRAY_MAP
#define VIDEO_GRAPHICS_ARRAY_MAP(_XX) \
    _XX(         QQVGA, QQVGA,  160, 120,   4:3 ) \
    _XX( HQVGA_240_160, HQVGA,  240, 160,   3:2 ) \
    _XX( HQVGA_256_160, HQVGA,  256, 160,  16:10) \
    _XX(          QVGA,  QVGA,  320, 240,   4:3 ) \
    _XX( WQVGA_384_240, WQVGA,  384, 240,  16:10) \
    _XX( WQVGA_360_240, WQVGA,  360, 240,   3:2 ) \
    _XX( WQVGA_400_240, WQVGA,  400, 240,   5:3 ) \
    _XX(          HVGA,  HVGA,  480, 320,   3:2 ) \
    _XX(           VGA,   VGA,  640, 480,   4:3 ) \
    _XX(  WVGA_768_480,  WVGA,  768, 480,  16:10) \
    _XX(  WVGA_720_480,  WVGA,  720, 480,   3:2 ) \
    _XX(  WVGA_800_480,  WVGA,  800, 480,   5:3 ) \
    _XX(         FWVGA, FWVGA,  854, 480,  16:9 ) \
    _XX(          SVGA,  SVGA,  800, 600,   4:3 ) \
    _XX(          DVGA,  DVGA,  960, 640,   3:2 ) \
    _XX(WSVGA_1024_576, WSVGA, 1024, 576,  16:9 ) \
    _XX(WSVGA_1024_600, WSVGA, 1024, 600, 128:75) \
    /* -- END -- */
#endif

#ifndef EXTENDED_GRAPHICS_ARRAY_MAP
#define EXTENDED_GRAPHICS_ARRAY_MAP(_XX) \
    _XX(          XGA,    XGA, 1024,  768,  4:3 ) \
    _XX(WXGA_1152_768,   WXGA, 1152,  768,  3:2 ) \
    _XX(WXGA_1280_768,   WXGA, 1280,  768,  5:3 ) \
    _XX(WXGA_1280_800,   WXGA, 1280,  800, 16:10) \
    _XX(WXGA_1360_768,   WXGA, 1360,  768, 16:9 ) \
    _XX(        FWXGA,  FWXGA, 1366,  768, 16:9 ) \
    _XX(     XGA_PLUS,   XGA+, 1152,  864,  4:3 ) \
    _XX(    WXGA_PLUS,  WXGA+, 1440,  900, 16:10) \
    _XX(        WSXGA,  WSXGA, 1440,  960,  3:2 ) \
    _XX(         SXGA,   SXGA, 1280, 1024,  5:4 ) \
    _XX(    SXGA_PLUS,  SXGA+, 1400, 1050,  4:3 ) \
    _XX(   WSXGA_PLUS, WSXGA+, 1680, 1050, 16:10) \
    _XX(         UXGA,   UXGA, 1600, 1200,  4:3 ) \
    _XX(        WUXGA,  WUXGA, 1920, 1200, 16:10) \
    /* -- END -- */
#endif

#ifndef QUAD_EXTENDED_GRAPHICS_ARRAY_MAP
#define QUAD_EXTENDED_GRAPHICS_ARRAY_MAP(_XX) \
    _XX(          QWXGA,  QWXGA, 2048, 1152, 16:9 ) \
    _XX(           QXGA,   QXGA, 2048, 1536,  4:3 ) \
    _XX(WQXGA_2560_1600,  WQXGA, 2560, 1600, 16:10) \
    _XX(WQXGA_2880_1800,  WQXGA, 2880, 1800, 16:10) \
    _XX(          QSXGA,  QSXGA, 2560, 2048,  5:4 ) \
    _XX(         WQSXGA, WQSXGA, 3200, 2048, 25:16) \
    _XX(          QUXGA,  QUXGA, 3200, 2400,  4:3 ) \
    _XX(         WQUXGA, WQUXGA, 3840, 2400, 16:10) \
    /* -- END -- */
#endif

#ifndef HYPER_EXTENDED_GRAPHICS_ARRAY_MAP
#define HYPER_EXTENDED_GRAPHICS_ARRAY_MAP(_XX) \
    _XX(  HXGA,   HXGA, 4096, 3072,  4:3 ) \
    _XX( WHXGA,  WHXGA, 5120, 3200, 16:10) \
    _XX( HSXGA,  HSXGA, 5120, 4096,  5:4 ) \
    _XX(WHSXGA, WHSXGA, 6400, 4096, 25:16) \
    _XX( HUXGA,  HUXGA, 6400, 4800,  4:3 ) \
    _XX(WHUXGA, WHUXGA, 7680, 4800, 16:10) \
    /* -- END -- */
#endif

#ifndef STANDARD_DISPLAY_RESOLUTIONS_MAP
#define STANDARD_DISPLAY_RESOLUTIONS_MAP(_XX) \
    _XX( 160,  120,  4:3 ) \
    _XX( 320,  200,  8:5 ) \
    _XX( 640,  200, 16:5 ) \
    _XX( 640,  350, 64:35) \
    _XX( 640,  480,  4:3 ) \
    _XX( 720,  348, 60:29) \
    _XX( 800,  600,  4:3 ) \
    _XX(1024,  768,  4:3 ) \
    _XX(1152,  864,  4:3 ) \
    _XX(1280, 1024,  5:4 ) \
    _XX(1400, 1050,  4:3 ) \
    _XX(1600, 1200,  4:3 ) \
    _XX(2048, 1536,  4:3 ) \
    _XX(2560, 2048,  5:4 ) \
    _XX(3200, 2400,  4:3 ) \
    _XX(4096, 3072,  4:3 ) \
    _XX(5120, 4096,  5:4 ) \
    _XX(6400, 4800,  4:3 ) \
    /* -- END -- */
#endif

#ifndef WIDESCREEN_DISPLAY_RESOLUTIONS_MAP
#define WIDESCREEN_DISPLAY_RESOLUTIONS_MAP(_XX) \
    _XX( 240,  160,   3:2  ) \
    _XX( 320,  240,   4:3  ) \
    _XX( 432,  240,   9:5  ) \
    _XX( 480,  270,  16:9  ) \
    _XX( 480,  320,   3:2  ) \
    _XX( 640,  400,   8:5  ) \
    _XX( 800,  480,   5:3  ) \
    _XX( 854,  480, 427:240) \
    _XX(1024,  576,  16:9  ) \
    _XX(1280,  720,  16:9  ) \
    _XX(1280,  768,   5:3  ) \
    _XX(1280,  800,   8:5  ) \
    _XX(1366,  768, 683:384) \
    _XX(1366,  900, 683:450) \
    _XX(1440,  900,   8:5  ) \
    _XX(1600,  900,  16:9  ) \
    _XX(1680,  945,  16:9  ) \
    _XX(1680, 1050,   8:5  ) \
    _XX(1920, 1080,  16:9  ) \
    _XX(1920, 1200,   8:5  ) \
    _XX(2048, 1152,  16:9  ) \
    _XX(2560, 1440,  16:9  ) \
    _XX(2560, 1600,   8:5  ) \
    _XX(3200, 2048,  25:16 ) \
    _XX(3840, 2160,  16:9  ) \
    _XX(3200, 2400,   4:3  ) \
    _XX(5120, 2880,  16:9  ) \
    _XX(5120, 3200,   8:5  ) \
    _XX(5760, 3240,  16:9  ) \
    _XX(6400, 4096,  25:16 ) \
    _XX(7680, 4320,  16:9  ) \
    _XX(7680, 4800,   8:5  ) \
    /* -- END -- */
#endif

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_RESOLUTION_HPP__

