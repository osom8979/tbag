/**
 * @file   CudaVersion.hpp
 * @brief  CudaVersion class prototype.
 * @author zer0
 * @date   2017-07-31
 *
 * @see <https://developer.nvidia.com/cuda-gpus>
 * @see <https://en.wikipedia.org/wiki/CUDA>
 *
 * @remarks
 *  - CUDA SDK 6.5: Last Version with support for Compute Capability 1.x (Tesla)
 *  - CUDA SDK 7.5 support for Compute Capability 2.0 – 5.x (Fermi, Kepler, Maxwell)
 *  - CUDA SDK 8.0 support for Compute Capability 2.0 – 6.x (Fermi, Kepler, Maxwell, Pascal), last version with support for Compute Capability 2.x (Fermi)
 *  - CUDA SDK 9.0 support for Compute Capability 3.0 – 7.x (Kepler, Maxwell, Pascal, Volta)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDAVERSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDAVERSION_HPP__

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

namespace gpu {

enum class NvidiaMicroArchitecture
{
    NMA_Tesla,
    NMA_Fermi,
    NMA_Kepler,
    NMA_Maxwell,
    NMA_Pascal,
    NMA_Volta,
};

#ifndef TBAG_CUDA_VERSION_INFO_MAP
#define TBAG_CUDA_VERSION_INFO_MAP(_TBAG_XX) \
    /* Column information: {def, name, arch, major, minor} */ \
    /* Compute capability: 1.0 */ \
    _TBAG_XX(GEFORCE_8800_ULTRA     , GeForce 8800 Ultra    , Tesla, 1, 0) \
    _TBAG_XX(GEFORCE_8800_GTX       , GeForce 8800 GTX      , Tesla, 1, 0) \
    _TBAG_XX(GEFORCE_8800_GTS_G80   , GeForce 8800 GTS(G80) , Tesla, 1, 0) \
    _TBAG_XX(QUADRO_FX_5600         , Quadro FX 5600        , Tesla, 1, 0) \
    _TBAG_XX(QUADRO_FX_4600         , Quadro FX 4600        , Tesla, 1, 0) \
    _TBAG_XX(QUADRO_PLEX_2100_S4    , Quadro Plex 2100 S4   , Tesla, 1, 0) \
    _TBAG_XX(TESLA_C870             , Tesla C870            , Tesla, 1, 0) \
    _TBAG_XX(TESLA_D870             , Tesla D870            , Tesla, 1, 0) \
    _TBAG_XX(TESLA_S870             , Tesla S870            , Tesla, 1, 0) \
    /* Compute capability: 1.1 */ \
    _TBAG_XX(GEFORCE_GTS_250            , GeForce GTS 250           , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_9800_GX2           , GeForce 9800 GX2          , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_9800_GTX           , GeForce 9800 GTX          , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_9800_GT            , GeForce 9800 GT           , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_8800_GTS_G92       , GeForce 8800 GTS(G92)     , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_8800_GT            , GeForce 8800 GT           , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_9600_GT            , GeForce 9600 GT           , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_9500_GT            , GeForce 9500 GT           , Tesla, 1 ,1) \
    _TBAG_XX(GEFORCE_9400_GT            , GeForce 9400 GT           , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_8600_GTS           , GeForce 8600 GTS          , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_8600_GT            , GeForce 8600 GT           , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_8500_GT            , GeForce 8500 GT           , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_G110M              , GeForce G110M             , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_9300M_GS           , GeForce 9300M GS          , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_9200M_GS           , GeForce 9200M GS          , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_9100M_G            , GeForce 9100M G           , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_8400M_GT           , GeForce 8400M GT          , Tesla, 1, 1) \
    _TBAG_XX(GEFORCE_G105M              , GeForce G105M             , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_4700_X2          , Quadro FX 4700 X2         , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_3700             , Quadro FX 3700            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_1800             , Quadro FX 1800            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_1700             , Quadro FX 1700            , Tesla, 1, 1) \
    _TBAG_XX(QuADRO_FX_580              , Quadro FX 580             , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_570              , Quadro FX 570             , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_470              , Quadro FX 470             , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_380              , Quadro FX 380             , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_370              , Quadro FX 370             , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_370_LOW_PROFILE  , Quadro FX 370 Low Profile , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_450             , Quadro NVS 450            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_420             , Quadro NVS 420            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_290             , Quadro NVS 290            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_295             , Quadro NVS 295            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_PLEX_2100_D4        , Quadro Plex 2100 D4       , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_3800M            , Quadro FX 3800M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_3700M            , Quadro FX 3700M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_3600M            , Quadro FX 3600M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_2800M            , Quadro FX 2800M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_2700M            , Quadro FX 2700M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_1700M            , Quadro FX 1700M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_1600M            , Quadro FX 1600M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_770M             , Quadro FX 770M            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_570M             , Quadro FX 570M            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_370M             , Quadro FX 370M            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_FX_360M             , Quadro FX 360M            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_320M            , Quadro NVS 320M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_160M            , Quadro NVS 160M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_150M            , Quadro NVS 150M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_140M            , Quadro NVS 140M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_135M            , Quadro NVS 135M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_130M            , Quadro NVS 130M           , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_450             , Quadro NVS 450            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_420             , Quadro NVS 420            , Tesla, 1, 1) \
    _TBAG_XX(QUADRO_NVS_295             , Quadro NVS 295            , Tesla, 1, 1) \
    /* Compute capability: 1.2 */ \
    _TBAG_XX(GEFORCE_GT_340           , GeForce GT 340           , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GT_330           , GeForce GT 330           , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GT_320           , GeForce GT 320           , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_315              , GeForce 315              , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_310              , GeForce 310              , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GT_240           , GeForce GT 240           , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GT_220           , GeForce GT 220           , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_210              , GeForce 210              , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GTS_360M         , GeForce GTS 360M         , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GTS_350M         , GeForce GTS 350M         , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GT_335M          , GeForce GT 335M          , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GT_330M          , GeForce GT 330M          , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GT_325M          , GeForce GT 325M          , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_GT_240M          , GeForce GT 240M          , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_G210M            , GeForce G210M            , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_310M             , GeForce 310M             , Tesla, 1, 2) \
    _TBAG_XX(GEFORCE_305M             , GeForce 305M             , Tesla, 1, 2) \
    _TBAG_XX(QUADRO_FX_380_LOW_PROFILE, Quadro FX 380 Low Profile, Tesla, 1, 2) \
    _TBAG_XX(NVIDIA_NVS_300           , Nvidia NVS 300           , Tesla, 1, 2) \
    _TBAG_XX(QUADRO_FX_1800M          , Quadro FX 1800M          , Tesla, 1, 2) \
    _TBAG_XX(QUADRO_FX_880M           , Quadro FX 880M           , Tesla, 1, 2) \
    _TBAG_XX(QUADRO_FX_380M           , Quadro FX 380M           , Tesla, 1, 2) \
    _TBAG_XX(NVIDIA_NVS_300           , Nvidia NVS 300           , Tesla, 1, 2) \
    _TBAG_XX(NVS_5100M                , NVS 5100M                , Tesla, 1, 2) \
    _TBAG_XX(NVS_3100M                , NVS 3100M                , Tesla, 1, 2) \
    _TBAG_XX(NVS_2100M                , NVS 2100M                , Tesla, 1, 2) \
    _TBAG_XX(ION                      , ION                      , Tesla, 1, 2) \
    /* Compute capability: 1.3 */ \
    _TBAG_XX(GEFORCE_GTX_295        , GeForce GTX 295       , Tesla, 1, 3) \
    _TBAG_XX(GTX_285                , GTX 285               , Tesla, 1, 3) \
    _TBAG_XX(GTX_280                , GTX 280               , Tesla, 1, 3) \
    _TBAG_XX(GEFORCE_GTX_275        , GeForce GTX 275       , Tesla, 1, 3) \
    _TBAG_XX(GEFORCE_GTX_260        , GeForce GTX 260       , Tesla, 1, 3) \
    _TBAG_XX(QUADRO_FX_5800         , Quadro FX 5800        , Tesla, 1, 3) \
    _TBAG_XX(QUADRO_FX_4800         , Quadro FX 4800        , Tesla, 1, 3) \
    _TBAG_XX(QUADRO_FX_4800_FOR_MAC , Quadro FX 4800 for Mac, Tesla, 1, 3) \
    _TBAG_XX(QuADRO_FX_3800         , Quadro FX 3800        , Tesla, 1, 3) \
    _TBAG_XX(QUADRO_CX              , Quadro CX             , Tesla, 1, 3) \
    _TBAG_XX(QUADRO_Plex_2200_D2    , Quadro Plex 2200 D2   , Tesla, 1, 3) \
    _TBAG_XX(TESLA_C1060            , Tesla C1060           , Tesla, 1, 3) \
    _TBAG_XX(TESLA_S1070            , Tesla S1070           , Tesla, 1, 3) \
    _TBAG_XX(TESLA_M1060            , Tesla M1060           , Tesla, 1, 3) \
    /* Compute capability: 2.0 */ \
    _TBAG_XX(GEFORCE_GTX_590    , GeForce GTX 590    , Fermi, 2, 0) \
    _TBAG_XX(GEFORCE_GTX_580    , GeForce GTX 580    , Fermi, 2, 0) \
    _TBAG_XX(GEFORCE_GTX_570    , GeForce GTX 570    , Fermi, 2, 0) \
    _TBAG_XX(GEFORCE_GTX_480    , GeForce GTX 480    , Fermi, 2, 0) \
    _TBAG_XX(GEFORCE_GTX_470    , GeForce GTX 470    , Fermi, 2, 0) \
    _TBAG_XX(GEFORCE_GTX_465    , GeForce GTX 465    , Fermi, 2, 0) \
    _TBAG_XX(GEFORCE_GTX_480M   , GeForce GTX 480M   , Fermi, 2, 0) \
    _TBAG_XX(QUADRO_6000        , Quadro 6000        , Fermi, 2, 0) \
    _TBAG_XX(QUADRO_5000        , Quadro 5000        , Fermi, 2, 0) \
    _TBAG_XX(QUADRO_4000        , Quadro 4000        , Fermi, 2, 0) \
    _TBAG_XX(QUADRO_4000_FOR_MAC, Quadro 4000 for Mac, Fermi, 2, 0) \
    _TBAG_XX(QUADRO_PLEX_7000   , Quadro Plex 7000   , Fermi, 2, 0) \
    _TBAG_XX(QUADRO_5010M       , Quadro 5010M       , Fermi, 2, 0) \
    _TBAG_XX(QUADRO_5000M       , Quadro 5000M       , Fermi, 2, 0) \
    _TBAG_XX(TESLA_C2075        , Tesla C2075        , Fermi, 2, 0) \
    _TBAG_XX(TESLA_C2050        , Tesla C2050        , Fermi, 2, 0) \
    _TBAG_XX(TESLA_C2070        , Tesla C2070        , Fermi, 2, 0) \
    _TBAG_XX(TESLA_M2050        , Tesla M2050        , Fermi, 2, 0) \
    _TBAG_XX(TESLA_M2070        , Tesla M2070        , Fermi, 2, 0) \
    _TBAG_XX(TESLA_M2075        , Tesla M2075        , Fermi, 2, 0) \
    _TBAG_XX(TESLA_M2090        , Tesla M2090        , Fermi, 2, 0) \
    /* Compute capability: 2.1 */ \
    _TBAG_XX(GEFORCE_GTX_560_TI  , GeForce GTX 560 Ti    , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_550_TI  , GeForce GTX 550 Ti    , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_460     , GeForce GTX 460       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTS_450     , GeForce GTS 450       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTS_450     , GeForce GTS 450       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_640_GDDR3, GeForce GT 640 (GDDR3), Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_630      , GeForce GT 630        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_620      , GeForce GT 620        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_610      , GeForce GT 610        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_520      , GeForce GT 520        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_440      , GeForce GT 440        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_440      , GeForce GT 440        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_430      , GeForce GT 430        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_430      , GeForce GT 430        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_420      , GeForce GT 420        , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_675M    , GeForce GTX 675M      , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_670M    , GeForce GTX 670M      , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_635M     , GeForce GT 635M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_630M     , GeForce GT 630M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_625M     , GeForce GT 625M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_720M     , GeForce GT 720M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_620M     , GeForce GT 620M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_710M        , GeForce 710M          , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_610M        , GeForce 610M          , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_820M        , GeForce 820M          , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_580M    , GeForce GTX 580M      , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_570M    , GeForce GTX 570M      , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_560M    , GeForce GTX 560M      , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_555M     , GeForce GT 555M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_550M     , GeForce GT 550M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_540M     , GeForce GT 540M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_525M     , GeForce GT 525M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_520MX    , GeForce GT 520MX      , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_520M     , GeForce GT 520M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_485M    , GeForce GTX 485M      , Fermi, 2 ,1) \
    _TBAG_XX(GEFORCE_GTX_470M    , GeForce GTX 470M      , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GTX_460M    , GeForce GTX 460M      , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_445M     , GeForce GT 445M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_435M     , GeForce GT 435M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_420M     , GeForce GT 420M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_GT_415M     , GeForce GT 415M       , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_710M        , GeForce 710M          , Fermi, 2, 1) \
    _TBAG_XX(GEFORCE_410M        , GeForce 410M          , Fermi, 2, 1) \
    _TBAG_XX(QUADRO_2000         , Quadro 2000           , Fermi, 2, 1) \
    _TBAG_XX(QUADRO_2000D        , Quadro 2000D          , Fermi, 2, 1) \
    _TBAG_XX(QUADRO_600          , Quadro 600            , Fermi, 2, 1) \
    _TBAG_XX(QUADRO_410          , Quadro 410            , Fermi, 2, 1) \
    _TBAG_XX(QUADRO_4000M        , Quadro 4000M          , Fermi, 2, 1) \
    _TBAG_XX(QUADRO_3000M        , Quadro 3000M          , Fermi, 2, 1) \
    _TBAG_XX(QUADRO_2000M        , Quadro 2000M          , Fermi, 2, 1) \
    _TBAG_XX(QUADRO_1000M        , Quadro 1000M          , Fermi, 2, 1) \
    _TBAG_XX(NVS_310             , NVS 310               , Fermi, 2, 1) \
    _TBAG_XX(NVS_315             , NVS 315               , Fermi, 2, 1) \
    _TBAG_XX(NVS_5400M           , NVS 5400M             , Fermi, 2, 1) \
    _TBAG_XX(NVS_5200M           , NVS 5200M             , Fermi, 2, 1) \
    _TBAG_XX(NVS_4200M           , NVS 4200M             , Fermi, 2, 1) \
    /* Compute capability: 3.0 */ \
    _TBAG_XX(GEFORCE_GTX_770         , GeForce GTX 770          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_760         , GeForce GTX 760          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_740          , GeForce GT 740           , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_690         , GeForce GTX 690          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_680         , GeForce GTX 680          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_670         , GeForce GTX 670          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_660_TI      , GeForce GTX 660 Ti       , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_660         , GeForce GTX 660          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_650_TI_BOOST, GeForce GTX 650 Ti BOOST , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_650_TI      , GeForce GTX 650 Ti       , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_650         , GeForce GTX 650          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_880M        , GeForce GTX 880M         , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_780M        , GeForce GTX 780M         , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_770M        , GeForce GTX 770M         , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_765M        , GeForce GTX 765M         , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_760M        , GeForce GTX 760M         , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_680MX       , GeForce GTX 680MX        , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_680M        , GeForce GTX 680M         , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_675MX       , GeForce GTX 675MX        , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_670MX       , GeForce GTX 670MX        , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GTX_660M        , GeForce GTX 660M         , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_750M         , GeForce GT 750M          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_650M         , GeForce GT 650M          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_745M         , GeForce GT 745M          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_645M         , GeForce GT 645M          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_740M         , GeForce GT 740M          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_730M         , GeForce GT 730M          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_640M         , GeForce GT 640M          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_640M_LE      , GeForce GT 640M LE       , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_735M         , GeForce GT 735M          , Kepler, 3, 0) \
    _TBAG_XX(GEFORCE_GT_730M         , GeForce GT 730M          , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K5000            , Quadro K5000             , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K4200            , Quadro K4200             , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K4000            , Quadro K4000             , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K2200            , Quadro K2200             , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K2000            , Quadro K2000             , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K2000D           , Quadro K2000D            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K600             , Quadro K600              , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K420             , Quadro K420              , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K500M            , Quadro K500M             , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K510M            , Quadro K510M             , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K610M            , Quadro K610M             , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K1000M           , Quadro K1000M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K2000M           , Quadro K2000M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K1100M           , Quadro K1100M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K2100M           , Quadro K2100M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K3000M           , Quadro K3000M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K3100M           , Quadro K3100M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K4000M           , Quadro K4000M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K5000M           , Quadro K5000M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K4100M           , Quadro K4100M            , Kepler, 3, 0) \
    _TBAG_XX(QUADRO_K5100M           , Quadro K5100M            , Kepler, 3, 0) \
    _TBAG_XX(NVS_510                 , NVS 510                  , Kepler, 3, 0) \
    _TBAG_XX(TESLA_K10               , Tesla K10                , Kepler, 3, 0) \
    _TBAG_XX(GRID_K340               , GRID K340                , Kepler, 3, 0) \
    _TBAG_XX(GRID_K520               , GRID K520                , Kepler, 3, 0) \
    /* Compute capability: 3.2 */ \
    _TBAG_XX(TEGRA_K1  , Tegra K1  , Kepler, 3, 2) \
    _TBAG_XX(JETSON_TK1, Jetson TK1, Kepler, 3, 2) \
    /* Compute capability: 3.5 */ \
    _TBAG_XX(GEFORCE_GTX_TITAN_Z        , GeForce GTX Titan Z          , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GTX_TITAN_BLACK    , GeForce GTX Titan Black      , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GTX_TITAN          , GeForce GTX Titan            , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GTX_780_TI         , GeForce GTX 780 Ti           , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GTX_780            , GeForce GTX 780              , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GT_640_GDDR5       , GeForce GT 640 (GDDR5)       , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GT_630_V2          , GeForce GT 630 v2            , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GT_730             , GeForce GT 730               , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GT_720             , GeForce GT 720               , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GT_710             , GeForce GT 710               , Kepler, 3, 5) \
    _TBAG_XX(GEFORCE_GT_740M_64BIT_DDR3 , GeForce GT 740M (64-bit DDR3), Kepler, 3, 5) \
    _TBAG_XX(QUADRO_K6000               , Quadro K6000                 , Kepler, 3, 5) \
    _TBAG_XX(QUADRO_K5200               , Quadro K5200                 , Kepler, 3, 5) \
    _TBAG_XX(TESLA_K40                  , Tesla K40                    , Kepler, 3, 5) \
    _TBAG_XX(TESLA_K20X                 , Tesla K20x                   , Kepler, 3, 5) \
    _TBAG_XX(TESLA_K20                  , Tesla K20                    , Kepler, 3, 5) \
    /* Compute capability: 3.7 */ \
    _TBAG_XX(TESLA_K80, Tesla K80, Kepler, 3, 7) \
    /* Compute capability: 5.0 */ \
    _TBAG_XX(GEFORCE_GTX_750_TI , GeForce GTX 750 Ti, Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_GTX_750    , GeForce GTX 750   , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_GTX_960M   , GeForce GTX 960M  , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_GTX_950M   , GeForce GTX 950M  , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_940M       , GeForce 940M      , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_930M       , GeForce 930M      , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_GTX_860M   , GeForce GTX 860M  , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_GTX_850M   , GeForce GTX 850M  , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_845M       , GeForce 845M      , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_840M       , GeForce 840M      , Maxwell, 5, 0) \
    _TBAG_XX(GEFORCE_830M       , GeForce 830M      , Maxwell, 5, 0) \
    _TBAG_XX(QUADRO_K1200       , Quadro K1200      , Maxwell, 5, 0) \
    _TBAG_XX(QUADRO_K620        , Quadro K620       , Maxwell, 5, 0) \
    _TBAG_XX(QUADRO_M2000M      , Quadro M2000M     , Maxwell, 5, 0) \
    _TBAG_XX(QUADRO_M1000M      , Quadro M1000M     , Maxwell, 5, 0) \
    _TBAG_XX(QUADRO_M600M       , Quadro M600M      , Maxwell, 5, 0) \
    _TBAG_XX(QUADRO_K620M       , Quadro K620M      , Maxwell, 5, 0) \
    _TBAG_XX(NVS_810            , NVS 810           , Maxwell, 5, 0) \
    _TBAG_XX(TESLA_M10          , Tesla M10         , Maxwell, 5, 0) \
    /* Compute capability: 5.2 */ \
    _TBAG_XX(GEFORCE_GTX_TITAN_X, GeForce GTX Titan X, Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_980_TI , GeForce GTX 980 Ti , Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_980    , GeForce GTX 980    , Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_970    , GeForce GTX 970    , Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_960    , GeForce GTX 960    , Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_950    , GeForce GTX 950    , Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_750_SE , GeForce GTX 750 SE , Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_980M   , GeForce GTX 980M   , Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_970M   , GeForce GTX 970M   , Maxwell, 5, 2) \
    _TBAG_XX(GEFORCE_GTX_965M   , GeForce GTX 965M   , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M6000_24GB  , Quadro M6000 24GB  , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M6000       , Quadro M6000       , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M5000       , Quadro M5000       , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M4000       , Quadro M4000       , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M2000       , Quadro M2000       , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M5500       , Quadro M5500       , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M5000M      , Quadro M5000M      , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M4000M      , Quadro M4000M      , Maxwell, 5, 2) \
    _TBAG_XX(QUADRO_M3000M      , Quadro M3000M      , Maxwell, 5, 2) \
    _TBAG_XX(TESLA_M4           , Tesla M4           , Maxwell, 5, 2) \
    _TBAG_XX(TESLA_M40          , Tesla M40          , Maxwell, 5, 2) \
    _TBAG_XX(TESLA_M6           , Tesla M6           , Maxwell, 5, 2) \
    _TBAG_XX(TESLA_M60          , Tesla M60          , Maxwell, 5, 2) \
    /* Compute capability: 5.3 */ \
    _TBAG_XX(TEGRA_X1   , Tegra X1  , Maxwell, 5, 3) \
    _TBAG_XX(JETSON_TX1 , Jetson TX1, Maxwell, 5, 3) \
    _TBAG_XX(DRIVE_CX   , DRIVE CX  , Maxwell, 5, 3) \
    _TBAG_XX(DRIVE_PX   , DRIVE PX  , Maxwell, 5, 3) \
    /* Compute capability: 6.0 */ \
    _TBAG_XX(QUADRO_GP100, Quadro GP100, Pascal, 6, 0) \
    _TBAG_XX(TESLA_P100  , Tesla P100  , Pascal, 6, 0) \
    /* Compute capability: 6.1 */ \
    _TBAG_XX(NVIDIA_TITAN_XP    , Nvidia TITAN Xp     , Pascal, 6, 1) \
    _TBAG_XX(TITAN_X            , Titan X             , Pascal, 6, 1) \
    _TBAG_XX(GEFORCE_GTX_1080_TI, GeForce GTX 1080 Ti , Pascal, 6, 1) \
    _TBAG_XX(GTX_1080           , GTX 1080            , Pascal, 6, 1) \
    _TBAG_XX(GTX_1070           , GTX 1070            , Pascal, 6, 1) \
    _TBAG_XX(GTX_1060           , GTX 1060            , Pascal, 6, 1) \
    _TBAG_XX(GTX_1050_TI        , GTX 1050 Ti         , Pascal, 6, 1) \
    _TBAG_XX(GTX_1050           , GTX 1050            , Pascal, 6, 1) \
    _TBAG_XX(GT_1030            , GT 1030             , Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P6000       , Quadro P6000        , Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P5000       , Quadro P5000        , Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P4000       , Quadro P4000        , Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P2000       , Quadro P2000        , Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P1000       , Quadro P1000        , Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P600        , Quadro P600         , Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P400        , Quadro P400         , Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P5000_MOBILE, Quadro P5000(Mobile), Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P4000_MOBILE, Quadro P4000(Mobile), Pascal, 6, 1) \
    _TBAG_XX(QUADRO_P3000_MOBILE, Quadro P3000(Mobile), Pascal, 6, 1) \
    _TBAG_XX(TESLA_P40          , Tesla P40           , Pascal, 6, 1) \
    _TBAG_XX(TESLA_P4           , Tesla P4            , Pascal, 6, 1) \
    /* Compute capability: 6.2 */ \
    _TBAG_XX(DRIVE_PX2_WITH_TEGRA_X2_T186, Drive PX2 with Tegra X2 (T186), Pascal, 6, 2) \
    _TBAG_XX(JETSON_TX2                  , Jetson TX2                    , Pascal, 6, 2) \
    /* Compute capability: 7.0 */ \
    _TBAG_XX(TESLA_V100, Tesla V100, Volta, 7, 0) \
    /* -- END -- */
#endif

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDAVERSION_HPP__

