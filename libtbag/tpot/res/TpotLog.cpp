/**
 * @file   TpotLog.cpp
 * @brief  TpotLog class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/res/TpotLog.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace res  {

TBAG_CONSTEXPR char const * const TPOT_LOG_ROOT = "log";

TpotLog::TpotLog()
{
    // EMPTY.
}

TpotLog::~TpotLog()
{
    // EMPTY.
}

TpotLog::String TpotLog::name() const
{
    return TPOT_LOG_ROOT;
}

void TpotLog::setup()
{
    std::string const NAME        = log::TBAG_DEFAULT_LOGGER_NAME;
    std::string const SINK        = "file";
    std::string const DESTINATION = std::string("${EXE_DIR}/tpot.log");
    std::string const AUTO_FLUSH  = "true";
    std::string const MULTITHREAD = "false";
    std::string const MUTEX       = "true";
    std::string const SEVERITY    = "info";
    std::string const GENERATOR   = "default";

    atInfos().clear();
    atInfos().push_back(getLogInfo(NAME, SINK, DESTINATION, AUTO_FLUSH, MULTITHREAD, MUTEX, SEVERITY, GENERATOR));
}

} // namespace res
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

