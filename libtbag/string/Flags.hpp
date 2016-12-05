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
 */
class TBAG_API Flags
{
public:
    struct Flag
    {
        std::string key;
        std::string value;

        Flag() : key(), value()
        { /* EMPTY. */ }
        Flag(std::string k) : key(k), value()
        { /* EMPTY. */ }
        Flag(std::string k, std::string v) : key(k), value(v)
        { /* EMPTY. */ }
    };

    using FlagVector = std::vector<Flag>;

public:
    inline static TBAG_CONSTEXPR char const * const getDefaultPrefix() TBAG_NOEXCEPT
    { return "--"; }
    inline static TBAG_CONSTEXPR char const * const getDefaultDelimiter() TBAG_NOEXCEPT
    { return "="; }

private:
    FlagVector _flags;

public:
    Flags() = default;
    ~Flags() = default;

public:
    Flags(int argc, char ** argv);
    Flags(std::string const & args, std::string const & prefix, std::string const & delimiter);
    Flags(std::string const & args);

public:
    Flags(Flags const & obj) = default;
    Flags & operator =(Flags const & obj) = default;

#if defined(TBAG_HAS_DEFAULTED_FUNCTIONS) && !defined(TBAG_HAS_DEFAULTED_FUNCTIONS_BUT_NOT_MOVE_FUNCTION)
    Flags(Flags && obj) = default;
    Flags & operator =(Flags && obj) = default;
#endif

public:
    inline void clear() TBAG_NOEXCEPT
    { _flags.clear(); }
    inline std::size_t size() const TBAG_NOEXCEPT
    { return _flags.size(); }
    inline bool empty() const TBAG_NOEXCEPT
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
    Flag find(FlagVector::const_iterator itr) const;
    Flag findWithKey(std::string const & key) const;
    Flag findWithValue(std::string const & value) const;

public:
    bool existsWithKey(std::string const & key) const;
    bool existsWithValue(std::string const & value) const;

public:
    std::vector<std::string> getUnnamedValues() const;

public:
    void parse(int argc, char ** argv);
    void parse(std::string const & args, std::string const & prefix, std::string const & delimiter);
    void parse(std::string const & args);

// ---------------
// Static methods.
// ---------------

public:
    static Flag convertFlag(std::string const & str, std::string const & prefix, std::string const & delimiter);
    static Flag convertFlag(std::string const & str);

public:
    static std::vector<std::string> splitTokens(std::string const & args);

public:
    static TBAG_CONSTEXPR char const        ESCAPE = '\\';
    static TBAG_CONSTEXPR char const DOUBLE_QUOTES = '"';
    static TBAG_CONSTEXPR char const SINGLE_QUOTES = '\'';
    static TBAG_CONSTEXPR char const  SPACE_QUOTES = ' ';

    static std::string splitFirst(std::string const & args, std::size_t * process_count = nullptr);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_FLAGS_HPP__

