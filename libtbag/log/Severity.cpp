/**
 * @file   Severity.cpp
 * @brief  Severity class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/Severity.hpp>
#include <libtbag/string/StringUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

char const * const getTextWithLevelStep(int level) TBAG_NOEXCEPT
{
    // clang-format off
    switch (level) {
    case       OFF_LEVEL: return       OFF_TEXT;
    case EMERGENCY_LEVEL: return EMERGENCY_TEXT;
    case     ALERT_LEVEL: return     ALERT_TEXT;
    case  CRITICAL_LEVEL: return  CRITICAL_TEXT;
    case     ERROR_LEVEL: return     ERROR_TEXT;
    case   WARNING_LEVEL: return   WARNING_TEXT;
    case    NOTICE_LEVEL: return    NOTICE_TEXT;
    case      INFO_LEVEL: return      INFO_TEXT;
    case     DEBUG_LEVEL: return     DEBUG_TEXT;
    case   UNKNOWN_LEVEL: TBAG_FALLTHROUGH
    default:
        return UNKNOWN_TEXT;
    }
    // clang-format on
}

char const * const getDetailsWithLevelStep(int level) TBAG_NOEXCEPT
{
    // clang-format off
    switch (level) {
    case       OFF_LEVEL: return       OFF_DETAILS;
    case EMERGENCY_LEVEL: return EMERGENCY_DETAILS;
    case     ALERT_LEVEL: return     ALERT_DETAILS;
    case  CRITICAL_LEVEL: return  CRITICAL_DETAILS;
    case     ERROR_LEVEL: return     ERROR_DETAILS;
    case   WARNING_LEVEL: return   WARNING_DETAILS;
    case    NOTICE_LEVEL: return    NOTICE_DETAILS;
    case      INFO_LEVEL: return      INFO_DETAILS;
    case     DEBUG_LEVEL: return     DEBUG_DETAILS;
    case   UNKNOWN_LEVEL: TBAG_FALLTHROUGH
    default:
        return UNKNOWN_DETAILS;
    }
    // clang-format on
}

Severity getSeverityWithLevelStep(int level) TBAG_NOEXCEPT
{
    // clang-format off
    switch (level) {
    case       OFF_LEVEL: return       OFF_SEVERITY;
    case EMERGENCY_LEVEL: return EMERGENCY_SEVERITY;
    case     ALERT_LEVEL: return     ALERT_SEVERITY;
    case  CRITICAL_LEVEL: return  CRITICAL_SEVERITY;
    case     ERROR_LEVEL: return     ERROR_SEVERITY;
    case   WARNING_LEVEL: return   WARNING_SEVERITY;
    case    NOTICE_LEVEL: return    NOTICE_SEVERITY;
    case      INFO_LEVEL: return      INFO_SEVERITY;
    case     DEBUG_LEVEL: return     DEBUG_SEVERITY;
    case   UNKNOWN_LEVEL: TBAG_FALLTHROUGH
    default:
        return UNKNOWN_SEVERITY;
    }
    // clang-format on
}

Severity findSeverity(std::string const & name)
{
    if (name.empty()) {
        return UNKNOWN_SEVERITY;
    }

    std::string const UPPER = libtbag::string::upper(libtbag::string::trim(name));
    std::size_t const UPPER_LENGTH = UPPER.size();
    assert(UPPER_LENGTH >= 1);

    int const UNKNOWN_SEVERITY_STRING = -1;
    int const LEVEL = libtbag::string::toValue<int>(UPPER, UNKNOWN_SEVERITY_STRING);
    if (LEVEL != UNKNOWN_SEVERITY_STRING) {
        // clang-format off
        if (LEVEL ==       OFF_LEVEL) { return       OFF_SEVERITY; }
        if (LEVEL == EMERGENCY_LEVEL) { return EMERGENCY_SEVERITY; }
        if (LEVEL ==     ALERT_LEVEL) { return     ALERT_SEVERITY; }
        if (LEVEL ==  CRITICAL_LEVEL) { return  CRITICAL_SEVERITY; }
        if (LEVEL ==     ERROR_LEVEL) { return     ERROR_SEVERITY; }
        if (LEVEL ==   WARNING_LEVEL) { return   WARNING_SEVERITY; }
        if (LEVEL ==    NOTICE_LEVEL) { return    NOTICE_SEVERITY; }
        if (LEVEL ==      INFO_LEVEL) { return      INFO_SEVERITY; }
        if (LEVEL ==     DEBUG_LEVEL) { return     DEBUG_SEVERITY; }
        if (LEVEL ==   UNKNOWN_LEVEL) { return   UNKNOWN_SEVERITY; }
        // clang-format on
        return UNKNOWN_SEVERITY;
    }

    // clang-format off
    if (UPPER ==       OFF_TEXT) { return       OFF_SEVERITY; }
    if (UPPER == EMERGENCY_TEXT) { return EMERGENCY_SEVERITY; }
    if (UPPER ==     ALERT_TEXT) { return     ALERT_SEVERITY; }
    if (UPPER ==  CRITICAL_TEXT) { return  CRITICAL_SEVERITY; }
    if (UPPER ==     ERROR_TEXT) { return     ERROR_SEVERITY; }
    if (UPPER ==   WARNING_TEXT) { return   WARNING_SEVERITY; }
    if (UPPER ==    NOTICE_TEXT) { return    NOTICE_SEVERITY; }
    if (UPPER ==      INFO_TEXT) { return      INFO_SEVERITY; }
    if (UPPER ==     DEBUG_TEXT) { return     DEBUG_SEVERITY; }
    if (UPPER ==   UNKNOWN_TEXT) { return   UNKNOWN_SEVERITY; }
    // clang-format on

    // clang-format off
    if (UPPER[0] == 'O') { return       OFF_SEVERITY; }
    if (UPPER[0] == 'A') { return     ALERT_SEVERITY; }
    if (UPPER[0] == 'C') { return  CRITICAL_SEVERITY; }
    if (UPPER[0] == 'W') { return   WARNING_SEVERITY; }
    if (UPPER[0] == 'N') { return    NOTICE_SEVERITY; }
    if (UPPER[0] == 'I') { return      INFO_SEVERITY; }
    if (UPPER[0] == 'D') { return     DEBUG_SEVERITY; }
    if (UPPER[0] == 'U') { return   UNKNOWN_SEVERITY; }
    // clang-format on

    if (UPPER[0] == 'E') {
        if (UPPER_LENGTH >= 2) {
            if (UPPER[1] == 'R') {
                return ERROR_SEVERITY;
            }
            if (UPPER[1] == 'M') {
                return EMERGENCY_SEVERITY;
            }
        }
        return ERROR_SEVERITY;
    }

    // clang-format off
    if (UPPER[0] == '0') { return       OFF_SEVERITY; }
    if (UPPER[0] == '1') { return EMERGENCY_SEVERITY; }
    if (UPPER[0] == '2') { return     ALERT_SEVERITY; }
    if (UPPER[0] == '3') { return  CRITICAL_SEVERITY; }
    if (UPPER[0] == '4') { return     ERROR_SEVERITY; }
    if (UPPER[0] == '5') { return   WARNING_SEVERITY; }
    if (UPPER[0] == '6') { return    NOTICE_SEVERITY; }
    if (UPPER[0] == '7') { return      INFO_SEVERITY; }
    if (UPPER[0] == '8') { return     DEBUG_SEVERITY; }
    if (UPPER[0] == '9') { return   UNKNOWN_SEVERITY; }
    // clang-format on

    return UNKNOWN_SEVERITY;
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

