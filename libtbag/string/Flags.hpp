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
    struct Flag
    {
        std::string key;
        std::string value;

        Flag() : key(), value()
        { /* EMPTY. */ }
        Flag(std::string const & k) : key(k), value()
        { /* EMPTY. */ }
        Flag(std::string const & k, std::string const & v) : key(k), value(v)
        { /* EMPTY. */ }
    };

    using FlagVector   = std::vector<Flag>;
    using ArgvVector   = std::vector<char*>;

    struct Argv
    {
    public:
        friend class Flags;

    private:
        std::vector<std::string> _strings;
        ArgvVector _arguments;

    public:
        inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_arguments.size())
        { return _arguments.size(); }

    public:
        inline int argc() const TBAG_NOEXCEPT_SP_OP(_arguments.size())
        { return static_cast<int>(_arguments.size()); }
        inline char ** argv() TBAG_NOEXCEPT_SP_OP(_arguments.data())
        { return _arguments.data(); }
    };

public:
    TBAG_CONSTEXPR static char const * const DEFAULT_PREFIX = "--";
    TBAG_CONSTEXPR static char const * const DEFAULT_DELIMITER = "=";

private:
    FlagVector _flags;

public:
    Flags();

    explicit Flags(int argc, char ** argv);
    explicit Flags(std::string const & args, std::string const & prefix, std::string const & delimiter);
    explicit Flags(std::string const & args);

    Flags(Flags const & obj);
    Flags(Flags && obj);

    ~Flags();

public:
    Flags & operator =(Flags const & obj);
    Flags & operator =(Flags && obj);

public:
    inline void        clear()       TBAG_NOEXCEPT_SP_OP(_flags.clear()) {        _flags.clear(); }
    inline std::size_t  size() const TBAG_NOEXCEPT_SP_OP(_flags.size ()) { return _flags.size (); }
    inline bool        empty() const TBAG_NOEXCEPT_SP_OP(_flags.empty()) { return _flags.empty(); }

public:
    inline void push(Flag const & flag) { _flags.push_back(flag); }

public:
    inline Flag       & at(std::size_t index)       { return _flags.at(index); }
    inline Flag const & at(std::size_t index) const { return _flags.at(index); }

public:
    void swap(Flags & obj);

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
    std::vector<std::string> getUnnamedValues() const;

public:
    bool parse(int argc, char ** argv);
    bool parse(int argc, char ** argv, std::string const & prefix, std::string const & delimiter);
    bool parse(std::string const & args);
    bool parse(std::string const & args, std::string const & prefix, std::string const & delimiter);
    bool parse(std::vector<std::string> const & args, std::string const & prefix, std::string const & delimiter);

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

