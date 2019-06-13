/**
 * @file   StFrame.cpp
 * @brief  StFrame class implementation.
 * @author zer0
 * @date   2017-05-28
 */

#include <libtbag/debug/st/StFrame.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <demangle.h>

#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

StFrame::StFrame() TBAG_NOEXCEPT : StFrame(nullptr)
{
    // EMPTY.
}

StFrame::StFrame(void const * addr) TBAG_NOEXCEPT : addr(addr), offset(0), index(0)
{
    clearName();
    clearSource();
}

StFrame::StFrame(StFrame const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

StFrame::StFrame(StFrame && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

StFrame::~StFrame()
{
    // EMPTY.
}

StFrame & StFrame::operator =(StFrame const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // clang-format off
        addr = obj.addr;
        std::memcpy(name  , obj.name  ,   NAME_MEM_SIZE);
        std::memcpy(source, obj.source, SOURCE_MEM_SIZE);
        offset = obj.offset;
        index  = obj.index;
        // clang-format on
    }
    return *this;
}

StFrame & StFrame::operator =(StFrame && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // clang-format off
        std::swap(addr  , obj.addr);
        std::swap(name  , obj.name);
        std::swap(source, obj.source);
        std::swap(offset, obj.offset);
        std::swap(index , obj.index);
        // clang-format on
    }
    return *this;
}

void StFrame::clearName()
{
    std::memset(name, 0x00, NAME_MEM_SIZE);
}

void StFrame::clearSource()
{
    std::memset(source, 0x00, SOURCE_MEM_SIZE);
}

std::string StFrame::toAddressString() const
{
    return string::convertAddressHexStringToString(string::convertAddressToHexString(addr));
}

void StFrame::demangleAssign(char const * symbol, std::size_t symbol_size)
{
    if (::Demangle(symbol, source, static_cast<int>(SOURCE_MEM_SIZE))) {
        return;
    }

    std::size_t const SIZE = (symbol_size <= (SOURCE_MEM_SIZE - 1) ? symbol_size : (SOURCE_MEM_SIZE - 1));
    std::memcpy(source, symbol, SIZE);
    source[SIZE] = '\0';
}

std::string StFrame::toString() const
{
    if (*source == '\0' && *name == '\0') {
        return toAddressString();
    }
    return toAddressString() + " " + source + "+" + std::to_string(offset) + " [" + name + "]";
}

StFrame StFrame::parseGccSymbolize(char const * symbols_format, void const * addr)
{
    if (/* */symbols_format == nullptr ||
            *symbols_format == '[' || // Check the case of "[(nil)]"
            *symbols_format == '\0') {
        return StFrame(addr);
    }

    char const * cursor = symbols_format;
    char const * column_begin = nullptr;
    char const * column_end = nullptr;
    std::size_t column_distance = 0;
    std::size_t copy_size = 0;
    int column = SYMBOL_STRINGS_GCC_COLUMN_MODULE;
    bool is_next = false;

    std::size_t const BUFFER_SIZE = StFrame::getSourceMemSize();
    char buffer[BUFFER_SIZE] = {0,};

    column_begin = cursor;
    ++cursor;

    StFrame frame(addr);
    do {
        while (true) {
            if (*cursor == '(') {
                column = SYMBOL_STRINGS_GCC_COLUMN_MODULE;
                column_end = cursor;
                break;
            } else if (*cursor == '+') {
                column = SYMBOL_STRINGS_GCC_COLUMN_SYMBOL;
                column_end = cursor;
                break;
            } else if (*cursor == ')') {
                column = SYMBOL_STRINGS_GCC_COLUMN_OFFSET;
                column_end = cursor;
                break;
            } else if (*cursor == ']') {
                column = SYMBOL_STRINGS_GCC_COLUMN_ADDRESS;
                column_end = cursor;
                break;
            } else if (*cursor == '\0') {
                column_end = cursor;
                break;
            } else if (*cursor == '[') {
                column_begin = (cursor + 1);
            }
            ++cursor;
        }

        if (*cursor == '\0') {
            break;
        }

        column_distance = static_cast<std::size_t>(std::distance(column_begin, column_end));
        if (column_distance > 0) {
            copy_size = column_distance <= (BUFFER_SIZE - 1) ? column_distance : (BUFFER_SIZE - 1);
            assert(copy_size >= 1);
            assert(copy_size <= (BUFFER_SIZE - 1));

            std::memcpy(buffer, column_begin, copy_size);
            buffer[copy_size] = '\0';

            if (column == SYMBOL_STRINGS_GCC_COLUMN_MODULE) {
                frame.clearName();
                std::memcpy(frame.name, buffer, copy_size);

            } else if (column == SYMBOL_STRINGS_GCC_COLUMN_SYMBOL) {
                frame.clearSource();
                frame.demangleAssign(buffer, copy_size);

            } else if (column == SYMBOL_STRINGS_GCC_COLUMN_OFFSET) {
                frame.offset = static_cast<int>(std::strtol(buffer, nullptr, 0));

            } else if (column == SYMBOL_STRINGS_GCC_COLUMN_ADDRESS) {
                if (addr == nullptr) {
                    frame.addr = reinterpret_cast<void*>(std::strtoull(buffer, nullptr, 0));
                }

            } else {
                break;
            }
        }

        ++cursor;
        ++column;
        column_begin = cursor;

    } while (true);

    return frame;
}

StFrame StFrame::parseClangSymbolize(char const * symbols_format, void const * addr)
{
    if (symbols_format == nullptr) {
        return StFrame(addr);
    }

    char const * cursor = symbols_format;
    char const * column_begin = nullptr;
    char const * column_end = nullptr;
    std::size_t column_distance = 0;
    std::size_t copy_size = 0;
    int column = SYMBOL_STRINGS_CLANG_COLUMN_INDEX;

    std::size_t const BUFFER_SIZE = StFrame::getSourceMemSize();
    char buffer[BUFFER_SIZE] = {0,};

    StFrame frame(addr);
    do {
        if (*cursor == '\0') { break; }
        if (column == SYMBOL_STRINGS_CLANG_COLUMN_OFFSET) {
            while (*cursor == ' ' || *cursor == '+') { ++cursor; }
        } else {
            while (*cursor == ' ') { ++cursor; }
        }
        if (*cursor == '\0') { break; }
        column_begin = cursor;
        while (*cursor != ' ' && *cursor != '\0') { ++cursor; }
        column_end = cursor;

        column_distance = static_cast<std::size_t>(std::distance(column_begin, column_end));
        if (column_distance > 0) {
            copy_size = column_distance <= (BUFFER_SIZE - 1) ? column_distance : (BUFFER_SIZE - 1);
            assert(copy_size >= 1);
            assert(copy_size <= (BUFFER_SIZE - 1));

            std::memcpy(buffer, column_begin, copy_size);
            buffer[copy_size] = '\0';

            if (column == SYMBOL_STRINGS_CLANG_COLUMN_INDEX) {
                frame.index = std::atoi(buffer);

            } else if (column == SYMBOL_STRINGS_CLANG_COLUMN_MODULE) {
                frame.clearName();
                std::memcpy(frame.name, buffer, copy_size);

            } else if (column == SYMBOL_STRINGS_CLANG_COLUMN_ADDRESS) {
                if (addr == nullptr) {
                    frame.addr = reinterpret_cast<void*>(std::strtoull(buffer, nullptr, 0));
                }

            } else if (column == SYMBOL_STRINGS_CLANG_COLUMN_SYMBOL) {
                frame.clearSource();
                frame.demangleAssign(buffer, copy_size);

            } else if (column == SYMBOL_STRINGS_CLANG_COLUMN_OFFSET) {
                frame.offset = std::atoi(buffer);

            } else {
                break;
            }
        }

        ++column;
        ++cursor;
    } while (true);

    return frame;
}

StFrame StFrame::parseSymbolize(char const * symbols_format, void const * addr)
{
#if defined(TBAG_COMP_CLANG)
    return parseClangSymbolize(symbols_format, addr);
#elif defined(TBAG_COMP_GNUC_CXX)
    return parseGccSymbolize(symbols_format, addr);
#else
    return StFrame(addr);
#endif
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

