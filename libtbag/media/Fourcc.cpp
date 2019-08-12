/**
 * @file   Fourcc.cpp
 * @brief  Fourcc class implementation.
 * @author zer0
 * @date   2019-08-12
 */

#include <libtbag/media/Fourcc.hpp>
#include <libtbag/tmp/ArraySize.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace media {

struct FourccAliasEntry
{
    uint32_t alias;
    uint32_t canonical;
};

TBAG_CONSTEXPR static const FourccAliasEntry FOURCC_ALIASES[] = {
        { FOURCC_IYUV, FOURCC_I420 },
        { FOURCC_YU16, FOURCC_I422 },
        { FOURCC_YU24, FOURCC_I444 },
        { FOURCC_YUYV, FOURCC_YUY2 },
        { FOURCC_YUVS, FOURCC_YUY2 },
        { FOURCC_HDYC, FOURCC_UYVY },
        { FOURCC_2VUY, FOURCC_UYVY },
        { FOURCC_JPEG, FOURCC_MJPG },  // Note: JPEG has DHT while MJPG does not.
        { FOURCC_DMB1, FOURCC_MJPG },
        { FOURCC_BA81, FOURCC_BGGR },
        { FOURCC_RGB3, FOURCC_RAW  },
        { FOURCC_BGR3, FOURCC_24BG },
        { FOURCC_CM32, FOURCC_BGRA },
        { FOURCC_CM24, FOURCC_RAW  },
};

uint32_t getCanonicalFourcc(uint32_t fourcc)
{
    for (auto i = 0; i < TBAG_ARRAY_SIZE(FOURCC_ALIASES); ++i) {
        if (FOURCC_ALIASES[i].alias == fourcc) {
            return FOURCC_ALIASES[i].canonical;
        }
    }
    return fourcc; // Not an alias, so return it as-is.
}

std::string getFourccName(uint32_t fourcc)
{
    return {static_cast<char>((fourcc      ) & 0xFF),
            static_cast<char>((fourcc >>  8) & 0xFF),
            static_cast<char>((fourcc >> 16) & 0xFF),
            static_cast<char>((fourcc >> 24) & 0xFF)};
}

} // namespace media

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

