/**
 * @file   Environments.hpp
 * @brief  Environments class prototype.
 * @author zer0
 * @date   2017-04-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_ENVIRONMENTS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_ENVIRONMENTS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/Flags.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * Environments class prototype.
 *
 * @author zer0
 * @date   2017-04-29
 */
class TBAG_API Environments
{
public:
    TBAG_CONSTEXPR static char const * const DEFAULT_REGEX_PREFIX = "\\$\\{";
    TBAG_CONSTEXPR static char const * const DEFAULT_REGEX_SUFFIX = "\\}";

    TBAG_CONSTEXPR static char const * const DEFAULT_DELIMITER = "=";

public:
    using Flag = Flags::Flag;
    using StringVector = Flags::StringVector;

private:
    Flags _flags;

public:
    Environments();

    explicit Environments(char ** envs);
    explicit Environments(char ** envs, std::string const & delimiter);
    explicit Environments(std::string const & envs);
    explicit Environments(std::string const & envs, std::string const & delimiter);
    explicit Environments(StringVector const & envs, std::string const & delimiter);

    explicit Environments(Flags const & flags);
    explicit Environments(Flags && flags);

    Environments(Environments const & obj);
    Environments(Environments && obj);

    virtual ~Environments();

public:
    Environments & operator =(Environments const & obj);
    Environments & operator =(Environments && obj);
    Environments & operator =(Flags const & flags);
    Environments & operator =(Flags && flags);

public:
    inline void clear() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_flags.clear()))
    { _flags.clear(); }
    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_flags.size()))
    { return _flags.size(); }
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_flags.empty()))
    { return _flags.empty(); }

public:
    inline void push(std::string const & key, std::string const & value)
    { _flags.push(Flag(key, value)); }
    inline void push(Flag const & flag)
    { _flags.push(flag); }

public:
    inline Flag & at(int index)
    { return _flags.at(index); }
    inline Flag const & at(int index) const
    { return _flags.at(index); }

public:
    void assign(Flags const & flags);
    void assign(Flags && flags);

public:
    bool parse(char ** envs);
    bool parse(char ** envs, std::string const & delimiter);
    bool parse(std::string const & envs);
    bool parse(std::string const & envs, std::string const & delimiter);
    bool parse(StringVector const & envs, std::string const & delimiter);

public:
    std::string convert(std::string const & source) const;
    std::string convert(std::string const & source, std::string const & regex_prefix, std::string const & regex_suffix) const;

public:
    static std::size_t getEnvsSize(char ** envs);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ENVIRONMENTS_HPP__

