/**
 * @file   Flags.hpp
 * @brief  Flags class prototype.
 * @author zer0
 * @date   2016-05-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FLAGS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FLAGS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/string/Strings.hpp>

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * Flags class prototype.
 *
 * @author zer0
 * @date   2016-05-03
 */
template <typename CharType = char>
class BaseFlags
{
public:
    using ValueType  = CharType;
    using String     = std::basic_string<ValueType>;
    using FlagType   = std::pair<String, String>;
    using BufferType = std::vector<FlagType>;

private:
    BufferType _buffer;

public:
    BaseFlags() {
        __EMPTY_BLOCK__
    }

    BaseFlags(BaseFlags const & obj) {
        this->copy(obj);
    }

    BaseFlags(BaseFlags && obj) {
        this->swap(obj);
    }

    ~BaseFlags() {
        __EMPTY_BLOCK__
    }

public:
    BaseFlags & operator =(BaseFlags const & obj) {
        return this->copy(obj);
    }

    BaseFlags & operator =(BaseFlags && obj) {
        this->swap(obj);
        return *this;
    }

public:
    BaseFlags & copy(BaseFlags const & obj) {
        if (this != &obj) {
            this->_buffer = obj._buffer;
        }
        return *this;
    }

    void swap(BaseFlags & obj) {
        if (this != &obj) {
            this->_buffer.swap(obj._buffer);
        }
    }

public:
    void clear() {
        this->_buffer.clear();
    }

    std::size_t size() const noexcept {
        return this->_buffer.size();
    }

    void push(FlagType const & flag) {
        this->_buffer.push_back(flag);
    }

    FlagType & at(int index) {
        return this->_buffer.at(index);
    }

    FlagType const & at(int index) const {
        return this->_buffer.at(index);
    }

public:
    FlagType findWithKey(String const & key) const {
        auto result = std::find_if(this->_buffer.begin(), this->_buffer.end(), [&key](FlagType const & flag) -> bool {
            return (flag.first == key);
        });

        if (result == this->_buffer.end()) {
            return FlagType();
        }

        return *result;
    }

    FlagType findWithValue(String const & value) const {
        auto result = std::find_if(this->_buffer.begin(), this->_buffer.end(), [&value](FlagType const & flag) -> bool {
            return (flag.second == value);
        });

        if (result == this->_buffer.end()) {
            return FlagType();
        }

        return *result;
    }

public:
    void parse(int argc, ValueType ** argv) {
        clear();
        for (int index = 0; index < argc; ++index) {
            push(convertFlag(String(argv[index])));
        }
    }

    void parse(String const & args) {
        clear();
        for (auto cursor : splitTokens(args)) {
            push(convertFlag(cursor));
        }
    }

public:
    static FlagType convertFlag(String const & str, String const & prefix, String const & delimiter) {
        if (str.substr(0, prefix.size()) == prefix) {
            // ENABLE KEY.
            std::size_t delimiter_pos = str.find(delimiter);
            String key = str.substr(prefix.size(), delimiter_pos - prefix.size());

            if (delimiter_pos == String::npos) {
                // ONLY KEY.
                return std::make_pair(key, String());
            } else {
                // KEY & VALUE.
                String value = str.substr(delimiter_pos + 1);
                return std::make_pair(key, value);
            }
        }

        // ONLY VALUE.
        return std::make_pair(String(), String(str));
    }

    static FlagType convertFlag(String const & str) {
        return convertFlag(str, getDefaultPrefix(), getDefaultDelimiter());
    }

    inline static String getDefaultPrefix() {
        return CHAR_OR_WIDECHAR(ValueType, "--");
    }

    inline static String getDefaultDelimiter() {
        return CHAR_OR_WIDECHAR(ValueType, "=");
    }

public:
    static std::vector<String> splitTokens(String const & args) {
        String trim_right_args = BaseStrings<ValueType>::trimRight(args);
        std::size_t args_size = trim_right_args.size();
        std::size_t all_process_count     = 0U;
        std::size_t current_process_count = 0U;

        std::vector<String> result;

        while (all_process_count < args_size) {
            result.push_back(splitFirst(trim_right_args.substr(all_process_count), &current_process_count));
            all_process_count += current_process_count;
        }

        return result;
    }

public:
    static constexpr ValueType const        ESCAPE = '\\';
    static constexpr ValueType const DOUBLE_QUOTES = '"';
    static constexpr ValueType const SINGLE_QUOTES = '\'';
    static constexpr ValueType const  SPACE_QUOTES = ' ';

    static String splitFirst(String const & args, std::size_t * process_count = nullptr) {
        String trim_left_args = BaseStrings<ValueType>::trimLeft(args);
        if (trim_left_args.empty()) {
            return String();
        }

        String result;

        auto itr = trim_left_args.begin();

        ValueType quotation_mark;
        if (*itr == DOUBLE_QUOTES) {
            quotation_mark = DOUBLE_QUOTES;
        } else if (*itr == SINGLE_QUOTES) {
            quotation_mark = SINGLE_QUOTES;
        } else {
            quotation_mark = SPACE_QUOTES;
            result.push_back(*itr);
        }
        ++itr;

        bool force_push_back = false;
        for (; itr != trim_left_args.end(); ++itr) {
            if (force_push_back) {
                result.push_back(*itr);
                force_push_back = false;
            } else {
                if (*itr == ESCAPE) {
                    // Skip, escape character (\).
                    force_push_back = true;
                } else if (*itr == quotation_mark) {
                    break;
                } else {
                    result.push_back(*itr);
                }
            }
        }

        if (process_count != nullptr) {
            *process_count = (args.size() - trim_left_args.size())      // Trim size.
                           + std::distance(trim_left_args.begin(), itr) // Size of string processing.
                           + 1; // Last, quotation_mark size.
        }

        return result;
    }
};

using Flags = BaseFlags<char>;
using WideFlags = BaseFlags<wchar_t>;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FLAGS_HPP__

