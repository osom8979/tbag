/**
 * @file   StackTrace.cpp
 * @brief  StackTrace class implementation.
 * @author zer0
 * @date   2016-12-25
 */

#include <libtbag/debug/StackTrace.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/debug/st/ExecInfoStackTrace.hpp>
#include <libtbag/debug/st/LibunwindStackTrace.hpp>
#include <libtbag/debug/st/UnwindStackTrace.hpp>
#include <libtbag/debug/st/WindbgStackTrace.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

st::StFrameArray getStackTrace(int skip)
{
    st::StFrameArray result;
    std::vector<void*> stack;

    if (st::isWindbgSupport()) {
        stack = st::getWindbgStackTrace();
        return result;
    }

    if (st::isExecInfoSupport()) {
        stack = st::getExecInfoStackTrace();
    } else if (st::isLibunwindSupport()) {
        stack = st::getLibunwindStackTrace();
    } else if (st::isUnwindSupport()) {
        stack = st::getUnwindStackTrace();
    } else {
        return result;
    }

    if (skip < 0) {
        skip = 0;
    }

    if (st::isExecInfoSupport()) {
        auto strings = st::getExecInfoSymbolize(stack);
        for (std::size_t i = skip; i < stack.size(); ++i) {
            result[i - skip] = st::StFrame::parseSymbolize(strings[i].c_str(), stack[i]);
        }
    } else {
        for (std::size_t i = 0; i < stack.size(); ++i) {
            result[i - skip] = st::StFrame(stack[i]);
        }
    }
    return result;
}

std::string getStackTraceString(int skip)
{
    std::stringstream ss;
    auto frames = getStackTrace(skip);

    for (std::size_t i = 0; i < frames.size(); ++i) {
        if (frames[i].empty()) {
            break;
        }
        ss << " #";
        if (i < 10) {
            ss << '0';
        }
        ss << i << ' ' << frames[i].toString() << std::endl;
    }
    return ss.str();
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

