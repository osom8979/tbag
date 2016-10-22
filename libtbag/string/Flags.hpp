/**
 * @file   Flags.hpp
 * @brief  Flags class prototype.
 * @author zer0
 * @date   2016-05-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_FLAGS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_FLAGS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/Strings.hpp>

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

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
    using Value  = CharType;
    using String = std::basic_string<Value>;

public:
    struct Flag
    {
        String key;
        String value;

        Flag(String k = "", String v = "") : key(k), value(v)
        { /* EMPTY. */ }
    };

public:
    using FlagVector   = std::vector<Flag>;
    using StringVector = std::vector<String>;

private:
    FlagVector _flags;

public:
    BaseFlags(int argc, Value ** argv)
    {
        parse(argc, argv);
    }

    BaseFlags(String const & args, String const & prefix, String const & delimiter)
    {
        parse(args, prefix, delimiter);
    }

    BaseFlags(String const & args)
    {
        parse(args);
    }

public:
    BaseFlags() = default;
    ~BaseFlags() = default;

public:
    BaseFlags(BaseFlags const & obj) = default;
    BaseFlags(BaseFlags && obj) = default;

public:
    BaseFlags & operator =(BaseFlags const & obj) = default;
    BaseFlags & operator =(BaseFlags && obj) = default;

public:
    inline void clear() noexcept
    { _flags.clear(); }
    inline std::size_t size() const noexcept
    { return _flags.size(); }
    inline bool empty() const noexcept
    { return _flags.empty(); }

public:
    inline void push(Flag const & flag)
    { _flags.push_back(flag); }

public:
    inline Flag & at(int index)
    { return _flags.at(index); }
    inline Flag const & at(int index) const
    { return _flags.at(index); }

public:
    inline Flag find(typename FlagVector::const_iterator itr) const
    {
        if (itr == _flags.end()) {
            return Flag();
        }
        return *itr;
    }

    inline Flag findWithKey(String const & key) const
    {
        return find(std::find_if(_flags.begin(), _flags.end(), [&key](Flag const & flag) -> bool {
            return (flag.key == key);
        }));
    }

    inline Flag findWithValue(String const & value) const
    {
        return find(std::find_if(_flags.begin(), _flags.end(), [&value](Flag const & flag) -> bool {
            return (flag.value == value);
        }));
    }

public:
    inline bool existsWithKey(String const & key) const
    {
        return !findWithKey(key).key.empty();
    }

    inline bool existsWithValue(String const & value) const
    {
        return !findWithValue(value).value.empty();
    }

public:
    inline StringVector getUnnamedValues() const
    {
        StringVector result;
        for (auto & cursor : _flags) {
            if (cursor.key == "" && cursor.value.empty() == false) {
                result.push_back(cursor.value);
            }
        }
        return result;
    }

public:
    void parse(int argc, Value ** argv)
    {
        for (int index = 0; index < argc; ++index) {
            push(convertFlag(String(argv[index])));
        }
    }

    void parse(String const & args, String const & prefix, String const & delimiter)
    {
        for (auto & cursor : splitTokens(args)) {
            push(convertFlag(cursor, prefix, delimiter));
        }
    }

    void parse(String const & args)
    {
        parse(args, getDefaultPrefix(), getDefaultDelimiter());
    }

// ---------------
// Static methods.
// ---------------

public:
    static Flag convertFlag(String const & str, String const & prefix, String const & delimiter)
    {
        if (str.substr(0, prefix.size()) == prefix) {
            // ENABLE KEY.
            std::size_t delimiter_pos = str.find(delimiter);
            String key = str.substr(prefix.size(), delimiter_pos - prefix.size());

            if (delimiter_pos == String::npos) {
                // ONLY KEY.
                return Flag(key);
            } else {
                // KEY & VALUE.
                String value = str.substr(delimiter_pos + 1);
                return Flag(key, value);
            }
        }

        // ONLY VALUE.
        return Flag(String(), String(str));
    }

    static Flag convertFlag(String const & str)
    {
        return convertFlag(str, getDefaultPrefix(), getDefaultDelimiter());
    }

    static constexpr String getDefaultPrefix()
    {
        return CHAR_OR_WIDECHAR(Value, "--");
    }

    static constexpr String getDefaultDelimiter()
    {
        return CHAR_OR_WIDECHAR(Value, "=");
    }

public:
    static std::vector<String> splitTokens(String const & args)
    {
        String trim_right_args = BaseStrings<Value>::trimRight(args);
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
    static constexpr Value const        ESCAPE = '\\';
    static constexpr Value const DOUBLE_QUOTES = '"';
    static constexpr Value const SINGLE_QUOTES = '\'';
    static constexpr Value const  SPACE_QUOTES = ' ';

    static String splitFirst(String const & args, std::size_t * process_count = nullptr)
    {
        String trim_left_args = BaseStrings<Value>::trimLeft(args);
        if (trim_left_args.empty()) {
            return String();
        }

        String result;

        auto itr = trim_left_args.begin();

        Value quotation_mark;
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

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_FLAGS_HPP__

