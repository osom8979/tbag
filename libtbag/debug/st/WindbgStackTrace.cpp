/**
 * @file   WindbgStackTrace.cpp
 * @brief  WindbgStackTrace class implementation.
 * @author zer0
 * @date   2017-06-06
 */

#include <libtbag/debug/st/WindbgStackTrace.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>
#include <libtbag/lib/ComInitializer.hpp>
#include <libtbag/lib/ComHolder.hpp>
#include <libtbag/functional/HashCombine.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>

#if defined(TBAG_PLATFORM_WINDOWS) && defined(TBAG_HAVE_DBGENG_H)
# include <windows.h>
# include <Dbgeng.h>
# pragma comment(lib, "ole32.lib")
# pragma comment(lib, "Dbgeng.lib")
#else
# include <libtbag/dummy/Win32.hpp>
using namespace libtbag::dummy::win32;
#endif

#include <cassert>
#include <array>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

using namespace libtbag::lib;

bool isWindbgSupport() TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_WINDOWS) && defined(TBAG_HAVE_DBGENG_H)
    return true;
#else
    return false;
#endif
}

static bool tryInitCom(ComHolder<IDebugSymbols> & idebug) TBAG_NOEXCEPT
{
    ComHolder<IDebugClient> iclient;
    DebugCreate(__uuidof(IDebugClient), iclient.toVoidPtrPtr());

    ComHolder<IDebugControl> icontrol;
    iclient->QueryInterface(__uuidof(IDebugControl), icontrol.toVoidPtrPtr());

    auto const ATTACH_FLAGS = DEBUG_ATTACH_NONINVASIVE | DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND;
    auto const ATTACH_RESULT = iclient->AttachProcess(0, GetCurrentProcessId(), ATTACH_FLAGS);
    if (ATTACH_RESULT != S_OK) {
        return false;
    }

    auto const WAIT_RESULT = icontrol->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE);
    if (WAIT_RESULT != S_OK) {
        return false;
    }

    auto const QUERY_RESULT = iclient->QueryInterface(__uuidof(IDebugSymbols), idebug.toVoidPtrPtr());
    if (QUERY_RESULT != S_OK) {
        return false;
    }
    return true;
}

std::vector<void*> getWindbgStackTrace(int skip, int max_depth)
{
    DWORD const FRAMES_TO_SKIP    = static_cast<DWORD>(skip);
    DWORD const FRAMES_TO_CAPTURE = static_cast<DWORD>(max_depth);

    std::vector<void*> result(static_cast<std::size_t>(max_depth - skip));
    ULONG back_trace_hash = 0;

    USHORT const FRAMES_COUNT = CaptureStackBackTrace(FRAMES_TO_SKIP, FRAMES_TO_CAPTURE, result.data(), &back_trace_hash);
    assert(FRAMES_COUNT <= max_depth - skip);
    result.resize(FRAMES_COUNT);

#if 0
    std::size_t hash_code;
    functional::hashCombine(hash_code, back_trace_hash);
#endif
    return result;
}

std::string getWindbgName(void const * addr)
{
    ComHolder<IDebugSymbols> idebug;
    if (!tryInitCom(idebug)) {
        return std::string();
    }

    ULONG64 const OFFSET = reinterpret_cast<ULONG64>(addr);

    int const BUFFER_SIZE = 256;
    std::array<char, BUFFER_SIZE> name = {0, };
    ULONG size = 0;
    HRESULT code = idebug->GetNameByOffset(OFFSET, name.data(), name.size(), &size, nullptr);

    std::string result;
    if (code != S_OK && size != 0) {
        result.resize(size);
        code = idebug->GetNameByOffset(OFFSET, &result[0], static_cast<ULONG>(result.size()), &size, nullptr);
    } else if (code == S_OK) {
        result = name.data();
    }

    if (code != S_OK) {
        result.clear();
    }
    return result;
}

std::string getWindbgSource(void const * addr)
{
    ComHolder<IDebugSymbols> idebug;
    if (!tryInitCom(idebug)) {
        return std::string();
    }

    ULONG64 const OFFSET = reinterpret_cast<ULONG64>(addr);

    int const BUFFER_SIZE = 256;
    std::array<char, BUFFER_SIZE> name = {0, };
    ULONG size = 0;
    HRESULT code = idebug->GetLineByOffset(OFFSET, nullptr, name.data(), name.size(), &size, nullptr);

    std::string result;
    if (code != S_OK && size != 0) {
        result.resize(size);
        code = idebug->GetLineByOffset(OFFSET, nullptr, &result[0], static_cast<ULONG>(result.size()), &size, nullptr);
    } else if (code == S_OK) {
        result = name.data();
    }

    if (code != S_OK) {
        result.clear();
    }
    return result;
}

std::size_t getWindbgLine(void const * addr)
{
    ComHolder<IDebugSymbols> idebug;
    if (!tryInitCom(idebug)) {
        return 0;
    }

    ULONG64 const OFFSET = reinterpret_cast<ULONG64>(addr);
    ULONG line_number = 0;
    auto const CODE = idebug->GetLineByOffset(OFFSET, &line_number, nullptr, 0, nullptr, nullptr);
    if (CODE == S_OK) {
        return line_number;
    } else {
        return 0;
    }
}

std::vector<std::string> getWindbgSymbolize(std::vector<void*> const & stack)
{
    std::vector<std::string> result;
    for (auto & cursor : stack) {
        auto const INFO = string::fformat("{}({}) [{}] {}",
                                          getWindbgSource(cursor),
                                          getWindbgLine(cursor),
                                          string::convertAddressToString(cursor),
                                          getWindbgName(cursor));
        result.emplace_back(std::move(INFO));
    }
    return result;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

