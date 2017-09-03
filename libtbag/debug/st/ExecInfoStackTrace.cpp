/**
 * @file   ExecInfoStackTrace.cpp
 * @brief  ExecInfoStackTrace class implementation.
 * @author zer0
 * @date   2017-09-03
 */

#include <libtbag/debug/st/ExecInfoStackTrace.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

#include <cstring>
#include <iterator>

#if defined(HAVE_EXECINFO_H)
#include <execinfo.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

bool isExecInfoSupport() TBAG_NOEXCEPT
{
#if defined(HAVE_EXECINFO_H)
    return true;
#else
    return false;
#endif
}

std::vector<void*> getExecInfoStackTrace(int max_depth)
{
    std::vector<void*> result;
#if defined(HAVE_EXECINFO_H)
    std::vector<void*> stack(static_cast<std::size_t>(max_depth));
    int const SIZE = ::backtrace(stack.data(), max_depth);
    result.assign(stack.begin(), stack.begin() + SIZE);
#endif
    return result;
}

// Example:
// 0  tester          0x0000000105f84708 _ZN7testing8UnitTest3RunEv + 408
// 1  tester          0x0000000105f60011 _Z13RUN_ALL_TESTSv + 17
// 2  tester          0x0000000105f5feb2 main + 578
// 3  libdyld.dylib   0x00007fffa12ef235 start + 1
std::vector<std::string> getExecInfoSymbolize(std::vector<void*> const & stack)
{
    std::vector<std::string> result;
#if defined(HAVE_EXECINFO_H)
    int const SIZE = static_cast<int>(stack.size());
    char ** strings = ::backtrace_symbols(stack.data(), SIZE);
    if (strings != nullptr) {
        for (int i = 0; i < SIZE; i++) {
            result.emplace_back(strings[i]);
        }
    }
#endif
    return result;
}

StFrame parseExecInfoSymbolize(void const * addr, char const * symbols_format)
{
    if (symbols_format == nullptr) {
        return StFrame(addr);
    }

    static int const SYMBOL_STRINGS_COLUMN_INDEX   = 0;
    static int const SYMBOL_STRINGS_COLUMN_MODULE  = 1;
    static int const SYMBOL_STRINGS_COLUMN_ADDRESS = 2;
    static int const SYMBOL_STRINGS_COLUMN_SYMBOL  = 3;
    static int const SYMBOL_STRINGS_COLUMN_LINE    = 4;

    char const * cursor = symbols_format;
    char const * column_begin = nullptr;
    char const * column_end = nullptr;
    std::size_t column_distance = 0;
    int column = 0;

    int const BUFFER_SIZE = 32;
    char buffer[BUFFER_SIZE + 1] = {0,};
    StFrame frame(addr);

    do {
        if (*cursor == '\0') { break; }
        if (column == SYMBOL_STRINGS_COLUMN_LINE) {
            while (*cursor == ' ' || *cursor == '+') { ++cursor; }
        } else {
            while (*cursor == ' ') { ++cursor; }
        }
        if (*cursor == '\0') { break; }
        column_begin = cursor;
        while (*cursor != ' ' && *cursor != '\0') { ++cursor; }
        column_end = cursor;

        column_distance = static_cast<std::size_t>(std::distance(column_begin, column_end));
        assert(column_distance >= 1);

        if (column == SYMBOL_STRINGS_COLUMN_INDEX) {
            std::size_t const COPY_SIZE = column_distance <= BUFFER_SIZE ? column_distance : BUFFER_SIZE;
            std::memcpy(buffer, column_begin, COPY_SIZE);
            frame.index = std::atoi(buffer);

        } else if (column == SYMBOL_STRINGS_COLUMN_MODULE) {
            std::size_t const COPY_SIZE = column_distance <= StFrame::getNameMemSize() ? column_distance : StFrame::getNameMemSize();
            std::memcpy(frame.name, column_begin, COPY_SIZE);
            frame.name[COPY_SIZE] = '\0';

        } else if (column == SYMBOL_STRINGS_COLUMN_ADDRESS) {
            // SKIP.

        } else if (column == SYMBOL_STRINGS_COLUMN_SYMBOL) {
            std::size_t const COPY_SIZE = column_distance <= StFrame::getSourceMemSize() ? column_distance : StFrame::getSourceMemSize();
            std::memcpy(frame.source, column_begin, COPY_SIZE);
            frame.source[COPY_SIZE] = '\0';

        } else if (column == SYMBOL_STRINGS_COLUMN_LINE) {
            std::size_t const COPY_SIZE = column_distance <= BUFFER_SIZE ? column_distance : BUFFER_SIZE;
            std::memcpy(buffer, column_begin, COPY_SIZE);
            frame.line = std::atoi(buffer);

        } else {
            break;
        }

        ++column;
        ++cursor;
    } while (true);

    return frame;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

