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
#include <libtbag/string/Argv.hpp>
#include <libtbag/string/StringUtils.hpp>

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
 *
 * @remarks
 *  Example:
 *  @code{.cpp}
 *   Flags flags;
 *   flags.parse("file.exe --arg1 --arg2=val2");
 *   flags.at(0).key;   // ""
 *   flags.at(0).value; // "file.exe"
 *   flags.at(1).key;   // "arg1"
 *   flags.at(1).value; // ""
 *   flags.at(2).key;   // "arg2"
 *   flags.at(2).value; // "val2"
 *  @endcode
 */
class TBAG_API Flags
{
public:
    struct Flag TBAG_FINAL
    {
        std::string key;
        std::string value;

        explicit Flag() : key(), value()
        { /* EMPTY. */ }
        explicit Flag(std::string const & k) : key(k), value()
        { /* EMPTY. */ }
        explicit Flag(std::string const & k, std::string const & v) : key(k), value(v)
        { /* EMPTY. */ }

        explicit Flag(std::string && k) TBAG_NOEXCEPT : key(std::move(k)), value()
        { /* EMPTY. */ }
        explicit Flag(std::string && k, std::string && v) TBAG_NOEXCEPT : key(std::move(k)), value(std::move(v))
        { /* EMPTY. */ }
    };

public:
    TBAG_CONSTEXPR static char const * const DEFAULT_PREFIX = "--";
    TBAG_CONSTEXPR static char const * const DEFAULT_DELIMITER = "=";

public:
    using FlagVector = std::vector<Flag>;

private:
    FlagVector _flags;

public:
    Flags();

    explicit Flags(int argc, char ** argv);
    explicit Flags(std::string const & args, std::string const & prefix, std::string const & delimiter);
    explicit Flags(std::string const & args);
    explicit Flags(std::vector<std::string> const & args, std::string const & prefix, std::string const & delimiter);
    explicit Flags(std::vector<std::string> const & args);

    Flags(Flags const & obj);
    Flags(Flags && obj) TBAG_NOEXCEPT;

    ~Flags();

public:
    Flags & operator =(Flags const & obj);
    Flags & operator =(Flags && obj) TBAG_NOEXCEPT;

public:
    inline void clear() TBAG_NOEXCEPT_SP_OP(_flags.clear())
    { _flags.clear(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_flags.size())
    { return _flags.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_flags.empty())
    { return _flags.empty(); }

public:
    inline void push(Flag const & flag)
    { _flags.emplace_back(flag); }
    inline void push(Flag && flag)
    { _flags.emplace_back(std::move(flag)); }
    inline void push(std::string const & key, std::string const & val)
    { _flags.emplace_back(key, val); }
    inline void push(std::string && key, std::string && val)
    { _flags.emplace_back(std::move(key), std::move(val)); }

public:
    inline Flag & at(std::size_t index)
    { return _flags.at(index); }
    inline Flag const & at(std::size_t index) const
    { return _flags.at(index); }

public:
    void swap(Flags & obj) TBAG_NOEXCEPT;

public:
    friend void swap(Flags & lh, Flags & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    Flag findByKey(std::string const & key) const;
    Flag findByValue(std::string const & value) const;

public:
    bool existsByKey(std::string const & key) const;
    bool existsByValue(std::string const & value) const;

public:
    bool get(std::string const & key, std::string & val) const;
    void set(std::string const & key, std::string const & val);

    std::string opt(std::string const & key, std::string const & default_val) const;
    std::string opt(std::string const & key) const;

public:
    bool remove(std::size_t index);
    bool remove(std::string const & key);

public:
    std::vector<std::string> keys() const;
    std::vector<std::string> values() const;
    std::vector<std::string> getUnnamedValues() const;

public:
    bool parse(int argc, char ** argv);
    bool parse(int argc, char ** argv, std::string const & prefix, std::string const & delimiter);
    bool parse(std::string const & args);
    bool parse(std::string const & args, std::string const & prefix, std::string const & delimiter);
    bool parse(std::vector<std::string> const & args);
    bool parse(std::vector<std::string> const & args, std::string const & prefix, std::string const & delimiter);

public:
    Argv argv(std::string const & prefix, std::string const & delimiter) const;
    Argv argv() const;

public:
    /**
     * String to flag.
     *
     * Input:
     *  - str: <code>"--verbose=true"</code>
     *  - prefix: <code>"--"</code>
     *  - delimiter: <code>"="</code>
     * Result:
     *  - <code>Flag{key="verbose", value="true"}</code>
     */
    static Flag convertStringToFlag(std::string const & str, std::string const & prefix, std::string const & delimiter);
    static Flag convertStringToFlag(std::string const & str);

    /**
     * Flag to string.
     *
     * Input:
     *  - flag: <code>Flag{key="verbose", value="true"}</code>
     *  - prefix: <code>"--"</code>
     *  - delimiter: <code>"="</code>
     * Return:
     *  - <code>"--verbose=true"</code>
     */
    static std::string convertFlagToString(Flag const & flag, std::string const & prefix, std::string const & delimiter);
    static std::string convertFlagToString(Flag const & flag);

public:
    static std::vector<std::string> splitTokens(std::string const & args);

public:
    TBAG_CONSTEXPR static char const ESCAPE = '\\';
    TBAG_CONSTEXPR static char const DOUBLE_QUOTES = '"';
    TBAG_CONSTEXPR static char const SINGLE_QUOTES = '\'';
    TBAG_CONSTEXPR static char const SPACE_QUOTES = ' ';

    static std::string splitFirst(std::string const & args, std::size_t * process_count = nullptr);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_FLAGS_HPP__

