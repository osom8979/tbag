/**
 * @file   Resolution.hpp
 * @brief  Resolution class prototype.
 * @author zer0
 * @date   2018-07-10
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

#ifndef RESOLUTION_MAP
#define RESOLUTION_MAP(_XX) \
    /* _XX(category, def, name, width, height, brief) */ \
    _XX(CIF , SUB_QCIF       , Sub-QCIF,  128,   96, Common Intermediate Format   ) \
    _XX(CIF , QCIF_176_144   , QCIF    ,  176,  144, Common Intermediate Format   ) \
    _XX(CIF , QCIF_220_176   , QCIF    ,  220,  176, Common Intermediate Format   ) \
    _XX(CIF , CIF            , CIF     ,  352,  288, Common Intermediate Format   ) \
    _XX(CIF , CIF4           , 4CIF    ,  704,  576, Common Intermediate Format   ) \
    _XX(CIF , CIF16          , 16CIF   , 1408, 1152, Common Intermediate Format   ) \
    _XX(CGA , CGA_320_200    , CGA     ,  320,  200, Color Graphics Adaptor       ) \
    _XX(CGA , CGA_640_200    , CGA     ,  640,  200, Color Graphics Adaptor       ) \
    _XX(EGA , EGA_320_200    , EGA     ,  320,  200, Enhanced Graphics Adaptor    ) \
    _XX(EGA , EGA_640_200    , EGA     ,  640,  200, Enhanced Graphics Adaptor    ) \
    _XX(EGA , EGA_640_350    , EGA     ,  640,  350, Enhanced Graphics Adaptor    ) \
    _XX(HGC , HGC_640_400    , HGC     ,  640,  400, Hercules Graphics Card       ) \
    _XX(HGC , HGC_720_348    , HGC     ,  720,  348, Hercules Graphics Card       ) \
    _XX(VGA , QQVGA          , qqVGA   ,  160,  120, quarter quarter VGA          ) \
    _XX(VGA , HQVGA          , HqVGA   ,  240,  160, Half quarter VGA             ) \
    _XX(VGA , QVGA           , qVGA    ,  320,  240, quarter VGA                  ) \
    _XX(VGA , WQVGA          , WqVGA   ,  400,  240, Wide quarter VGA             ) \
    _XX(VGA , HVGA           , HVGA    ,  480,  320, Half-size VGA                ) \
    _XX(VGA , VGA            , VGA     ,  640,  480, Video Graphics Array         ) \
    _XX(VGA , WVGA           , WVGA    ,  800,  480, Wide VGA                     ) \
    _XX(VGA , FWVGA          , FWVGA   ,  854,  480, Full Wide VGA                ) \
    _XX(VGA , SVGA           , SVGA    ,  800,  600, Super VGA                    ) \
    _XX(VGA , DVGA           , DVGA    ,  960,  640, Double-size VGA              ) \
    _XX(VGA , WSVGA_1024_576 , WSVGA   , 1024,  576, Wide Super VGA               ) \
    _XX(VGA , WSVGA_1024_600 , WSVGA   , 1024,  600, Wide Super VGA               ) \
    _XX(XGA , XGA            , XGA     , 1024,  768, eXtended Graphics Array      ) \
    _XX(XGA , WXGA_1280_768  , WXGA    , 1280,  768, Wide XGA                     ) \
    _XX(XGA , WXGA_1280_800  , WXGA    , 1280,  800, Wide XGA                     ) \
    _XX(XGA , FWXGA          , FWXGA   , 1366,  768, Full Wide XGA                ) \
    _XX(XGA , XGA_PLUS       , XGA+    , 1152,  864, XGA Plus                     ) \
    _XX(XGA , WXGA_PLUS      , WXGA+   , 1440,  900, Wide XGA Plus                ) \
    _XX(XGA , WSXGA          , WSXGA   , 1680, 1050, Widescreen Super XGA         ) \
    _XX(XGA , SXGA           , SXGA    , 1280, 1024, Super XGA                    ) \
    _XX(XGA , SXGA_PLUS      , SXGA+   , 1400, 1050, Super XGA                    ) \
    _XX(XGA , UXGA           , UXGA    , 1600, 1200, Ultra XGA                    ) \
    _XX(XGA , WUXGA          , WUXGA   , 1920, 1200, Wide Ultra XGA               ) \
    _XX(QXGA, QWXGA          , QWXGA   , 2048, 1152, Quad Wide XGA                ) \
    _XX(QXGA, QXGA           , QXGA    , 2048, 1536, Quad XGA                     ) \
    _XX(QXGA, WQXGA_2560_1600, WQXGA   , 2560, 1600, Wide Quad XGA                ) \
    _XX(QXGA, WQXGA_2880_1800, WQXGA   , 2880, 1800, Wide Quad XGA                ) \
    _XX(QXGA, QWXGA          , QWXGA   , 2560, 2048, Quad Wide XGA                ) \
    _XX(QXGA, WQSXGA         , WQSXGA  , 3200, 2048, Wide Quad Super XGA          ) \
    _XX(QXGA, QUXGA          , QUXGA   , 3200, 2400, Quad Ultra XGA               ) \
    _XX(QXGA, WQUXGA         , WQUXGA  , 3840, 2400, Wide Quad Ultra XGA          ) \
    _XX(HXGA, HXGA           , HXGA    , 4096, 3072, Hex XGA                      ) \
    _XX(HXGA, WHXGA          , WHXGA   , 5120, 3200, Wide Hex XGA                 ) \
    _XX(HXGA, HSXGA          , HSXGA   , 5120, 4096, Hex Super XGA                ) \
    _XX(HXGA, WHSXGA         , WHSXGA  , 6400, 4096, Wide Hex Super XGA           ) \
    _XX(HXGA, HUXGA          , HUXGA   , 6400, 4800, Hex Ultra XGA                ) \
    _XX(HXGA, WHUXGA         , WHUXGA  , 7680, 4800, Wide Hex Ultra XGA           ) \
    _XX(HD  , NHD            , nHD     ,  640,  360, one ninth of a Full HD frame ) \
    _XX(HD  , QHD            , qHD     ,  960,  540, one quarter of Full-HD       ) \
    _XX(HD  , HD             , HD      , 1280,  720, HD                           ) \
    _XX(HD  , HD_PLUS        , HD+     , 1600,  900, HD Plus                      ) \
    _XX(HD  , FHD            , FHD     , 1920, 1080, Full HD                      ) \
    _XX(HD  , WQHD           , WQHD/QHD, 2560, 1440, Wide Quad HD/Quad HD         ) \
    _XX(HD  , QHD_PLUS       , QHD+    , 3200, 1800, Quad HD Plus                 ) \
    _XX(HD  , UHD            , UHD     , 3840, 2160, Ultra HD/4K                  ) \
    _XX(HD  , UHD_PLUS       , UHD+    , 5120, 2880, Ultra HD Plus/5K             ) \
    _XX(HD  , QUHD           , QUHD    , 7680, 4320, Quad Ultra HD/8K             ) \
    /* -- END -- */
#endif

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_RESOLUTION_HPP__

