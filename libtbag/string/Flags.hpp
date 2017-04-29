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

    using FlagVector   = std::vector<Flag>;
    using StringVector = std::vector<std::string>;
    using ArgvVector   = std::vector<char*>;

    struct Argv
    {
    public:
        friend class Flags;

    private:
        StringVector _strings;
        ArgvVector   _arguments;

    public:
        inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_arguments.size()))
        { return _arguments.size(); }

    public:
        inline int argc() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_arguments.size()))
        { return static_cast<int>(_arguments.size()); }
        inline char ** argv() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_arguments.data()))
        { return _arguments.data(); }
    };

public:
    TBAG_CONSTEXPR static char const * const DEFAULT_PREFIX = "--";
    TBAG_CONSTEXPR static char const * const DEFAULT_DELIMITER = "=";

private:
    FlagVector _flags;

public:
    Flags();

    Flags(int argc, char ** argv);
    Flags(std::string const & args, std::string const & prefix, std::string const & delimiter);
    Flags(std::string const & args);

    Flags(Flags const & obj);
    Flags(Flags && obj);

    ~Flags();

public:
    Flags & operator =(Flags const & obj);
    Flags & operator =(Flags && obj);

public:
    inline void clear() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_flags.clear()))
    { _flags.clear(); }
    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_flags.size()))
    { return _flags.size(); }
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_flags.empty()))
    { return _flags.empty(); }

public:
    inline void push(Flag const & flag)
    { _flags.push_back(flag); }

public:
    inline Flag & at(std::size_t index)
    { return _flags.at(index); }
    inline Flag const & at(std::size_t index) const
    { return _flags.at(index); }

public:
    Flag find(FlagVector::const_iterator itr) const;
    Flag findWithKey(std::string const & key) const;
    Flag findWithValue(std::string const & value) const;

public:
    bool remove(std::size_t index);
    bool remove(std::string const & key);

public:
    bool existsWithKey(std::string const & key) const;
    bool existsWithValue(std::string const & value) const;

public:
    StringVector getUnnamedValues() const;

public:
    bool parse(int argc, char ** argv);
    bool parse(int argc, char ** argv, std::string const & prefix, std::string const & delimiter);
    bool parse(std::string const & args);
    bool parse(std::string const & args, std::string const & prefix, std::string const & delimiter);
    bool parse(StringVector const & args, std::string const & prefix, std::string const & delimiter);

public:
    Argv getArgv(std::string const & prefix, std::string const & delimiter, bool last_null = false) const;
    Argv getArgv(bool last_null = false) const;

// ---------------
// Static methods.
// ---------------

public:
    static Flag convertFlag(std::string const & str, std::string const & prefix, std::string const & delimiter);
    static Flag convertFlag(std::string const & str);

    static std::string convertString(Flag const & flag, std::string const & prefix, std::string const & delimiter);
    static std::string convertString(Flag const & flag);

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

