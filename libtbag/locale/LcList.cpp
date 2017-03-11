/**
 * @file   LcList.cpp
 * @brief  LcList class implementation.
 * @author zer0
 * @date   2017-03-11
 */

#include <libtbag/locale/LcList.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale {

// ---------------
namespace __impl {
// ---------------

/** Lists all public locales. */
TBAG_CONSTEXPR static char const * const LOCALE_ALL_PUBLIC_LOCALE[] = {
        "af_ZA",
        "af_ZA.ISO8859-1",
        "af_ZA.ISO8859-15",
        "af_ZA.UTF-8",
        "am_ET",
        "am_ET.UTF-8",
        "be_BY",
        "be_BY.CP1131",
        "be_BY.CP1251",
        "be_BY.ISO8859-5",
        "be_BY.UTF-8",
        "bg_BG",
        "bg_BG.CP1251",
        "bg_BG.UTF-8",
        "ca_ES",
        "ca_ES.ISO8859-1",
        "ca_ES.ISO8859-15",
        "ca_ES.UTF-8",
        "cs_CZ",
        "cs_CZ.ISO8859-2",
        "cs_CZ.UTF-8",
        "da_DK",
        "da_DK.ISO8859-1",
        "da_DK.ISO8859-15",
        "da_DK.UTF-8",
        "de_AT",
        "de_AT.ISO8859-1",
        "de_AT.ISO8859-15",
        "de_AT.UTF-8",
        "de_CH",
        "de_CH.ISO8859-1",
        "de_CH.ISO8859-15",
        "de_CH.UTF-8",
        "de_DE",
        "de_DE.ISO8859-1",
        "de_DE.ISO8859-15",
        "de_DE.UTF-8",
        "el_GR",
        "el_GR.ISO8859-7",
        "el_GR.UTF-8",
        "en_AU",
        "en_AU.ISO8859-1",
        "en_AU.ISO8859-15",
        "en_AU.US-ASCII",
        "en_AU.UTF-8",
        "en_CA",
        "en_CA.ISO8859-1",
        "en_CA.ISO8859-15",
        "en_CA.US-ASCII",
        "en_CA.UTF-8",
        "en_GB",
        "en_GB.ISO8859-1",
        "en_GB.ISO8859-15",
        "en_GB.US-ASCII",
        "en_GB.UTF-8",
        "en_IE",
        "en_IE.UTF-8",
        "en_NZ",
        "en_NZ.ISO8859-1",
        "en_NZ.ISO8859-15",
        "en_NZ.US-ASCII",
        "en_NZ.UTF-8",
        "en_US",
        "en_US.ISO8859-1",
        "en_US.ISO8859-15",
        "en_US.US-ASCII",
        "en_US.UTF-8",
        "es_ES",
        "es_ES.ISO8859-1",
        "es_ES.ISO8859-15",
        "es_ES.UTF-8",
        "et_EE",
        "et_EE.ISO8859-15",
        "et_EE.UTF-8",
        "eu_ES",
        "eu_ES.ISO8859-1",
        "eu_ES.ISO8859-15",
        "eu_ES.UTF-8",
        "fi_FI",
        "fi_FI.ISO8859-1",
        "fi_FI.ISO8859-15",
        "fi_FI.UTF-8",
        "fr_BE",
        "fr_BE.ISO8859-1",
        "fr_BE.ISO8859-15",
        "fr_BE.UTF-8",
        "fr_CA",
        "fr_CA.ISO8859-1",
        "fr_CA.ISO8859-15",
        "fr_CA.UTF-8",
        "fr_CH",
        "fr_CH.ISO8859-1",
        "fr_CH.ISO8859-15",
        "fr_CH.UTF-8",
        "fr_FR",
        "fr_FR.ISO8859-1",
        "fr_FR.ISO8859-15",
        "fr_FR.UTF-8",
        "he_IL",
        "he_IL.UTF-8",
        "hi_IN.ISCII-DEV",
        "hr_HR",
        "hr_HR.ISO8859-2",
        "hr_HR.UTF-8",
        "hu_HU",
        "hu_HU.ISO8859-2",
        "hu_HU.UTF-8",
        "hy_AM",
        "hy_AM.ARMSCII-8",
        "hy_AM.UTF-8",
        "is_IS",
        "is_IS.ISO8859-1",
        "is_IS.ISO8859-15",
        "is_IS.UTF-8",
        "it_CH",
        "it_CH.ISO8859-1",
        "it_CH.ISO8859-15",
        "it_CH.UTF-8",
        "it_IT",
        "it_IT.ISO8859-1",
        "it_IT.ISO8859-15",
        "it_IT.UTF-8",
        "ja_JP",
        "ja_JP.eucJP",
        "ja_JP.SJIS",
        "ja_JP.UTF-8",
        "kk_KZ",
        "kk_KZ.PT154",
        "kk_KZ.UTF-8",
        "ko_KR",
        "ko_KR.CP949",
        "ko_KR.eucKR",
        "ko_KR.UTF-8",
        "lt_LT",
        "lt_LT.ISO8859-13",
        "lt_LT.ISO8859-4",
        "lt_LT.UTF-8",
        "nl_BE",
        "nl_BE.ISO8859-1",
        "nl_BE.ISO8859-15",
        "nl_BE.UTF-8",
        "nl_NL",
        "nl_NL.ISO8859-1",
        "nl_NL.ISO8859-15",
        "nl_NL.UTF-8",
        "no_NO",
        "no_NO.ISO8859-1",
        "no_NO.ISO8859-15",
        "no_NO.UTF-8",
        "pl_PL",
        "pl_PL.ISO8859-2",
        "pl_PL.UTF-8",
        "pt_BR",
        "pt_BR.ISO8859-1",
        "pt_BR.UTF-8",
        "pt_PT",
        "pt_PT.ISO8859-1",
        "pt_PT.ISO8859-15",
        "pt_PT.UTF-8",
        "ro_RO",
        "ro_RO.ISO8859-2",
        "ro_RO.UTF-8",
        "ru_RU",
        "ru_RU.CP1251",
        "ru_RU.CP866",
        "ru_RU.ISO8859-5",
        "ru_RU.KOI8-R",
        "ru_RU.UTF-8",
        "sk_SK",
        "sk_SK.ISO8859-2",
        "sk_SK.UTF-8",
        "sl_SI",
        "sl_SI.ISO8859-2",
        "sl_SI.UTF-8",
        "sr_YU",
        "sr_YU.ISO8859-2",
        "sr_YU.ISO8859-5",
        "sr_YU.UTF-8",
        "sv_SE",
        "sv_SE.ISO8859-1",
        "sv_SE.ISO8859-15",
        "sv_SE.UTF-8",
        "tr_TR",
        "tr_TR.ISO8859-9",
        "tr_TR.UTF-8",
        "uk_UA",
        "uk_UA.ISO8859-5",
        "uk_UA.KOI8-U",
        "uk_UA.UTF-8",
        "zh_CN",
        "zh_CN.eucCN",
        "zh_CN.GB18030",
        "zh_CN.GB2312",
        "zh_CN.GBK",
        "zh_CN.UTF-8",
        "zh_HK",
        "zh_HK.Big5HKSCS",
        "zh_HK.UTF-8",
        "zh_TW",
        "zh_TW.Big5",
        "zh_TW.UTF-8",
        "C",
        "POSIX"
};

TBAG_CONSTEXPR static std::size_t const LOCALE_ALL_PUBLIC_LOCALE_COUNT =
        sizeof(LOCALE_ALL_PUBLIC_LOCALE) / sizeof(LOCALE_ALL_PUBLIC_LOCALE[0]);

// ------------------
} // namespace __impl
// ------------------

std::size_t const getAllLocaleCount() TBAG_NOEXCEPT
{
    return __impl::LOCALE_ALL_PUBLIC_LOCALE_COUNT;
}

std::vector<std::string> getAllLocale()
{
    std::vector<std::string> result(__impl::LOCALE_ALL_PUBLIC_LOCALE_COUNT);
    for (std::size_t i = 0; i < __impl::LOCALE_ALL_PUBLIC_LOCALE_COUNT; ++i) {
        result[i].assign(__impl::LOCALE_ALL_PUBLIC_LOCALE[i]);
    }
    return result;
}

} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

