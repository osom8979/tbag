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
#include <libtbag/dom/xml/Resource.hpp>
#include <libtbag/string/StringUtils.hpp>
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
    using Resource = libtbag::dom::xml::Resource;
    using Flag = Flags::Flag;

private:
    Flags _flags;

public:
    Environments();

    explicit Environments(char ** envs);
    explicit Environments(char ** envs, std::string const & delimiter);
    explicit Environments(std::string const & envs);
    explicit Environments(std::string const & envs, std::string const & delimiter);
    explicit Environments(std::vector<std::string> const & envs, std::string const & delimiter);

    explicit Environments(Flags const & flags);
    explicit Environments(Flags && flags) TBAG_NOEXCEPT;

    Environments(Environments const & obj);
    Environments(Environments && obj) TBAG_NOEXCEPT;

    virtual ~Environments();

public:
    Environments & operator =(Environments const & obj);
    Environments & operator =(Environments && obj) TBAG_NOEXCEPT;

    Environments & operator =(Flags const & flags);
    Environments & operator =(Flags && flags) TBAG_NOEXCEPT;

public:
    void swap(Environments & obj) TBAG_NOEXCEPT
    { _flags.swap(obj._flags); }
    friend void swap(Environments & lh, Environments & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline void clear() TBAG_NOEXCEPT_SP_OP(_flags.clear())
    { _flags.clear(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_flags.size())
    { return _flags.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_flags.empty())
    { return _flags.empty(); }

public:
    inline Flag & at(std::size_t index)
    { return _flags.at(index); }
    inline Flag const & at(std::size_t index) const
    { return _flags.at(index); }

public:
    void set(Flags const & flags)
    { _flags = flags; }
    void set(Flags && flags) TBAG_NOEXCEPT
    { _flags = std::move(flags); }
    void set(std::string const & key, std::string const & value)
    { _flags.set(key, value); }
    void set(std::string && key, std::string && value)
    { _flags.set(key, value); }

public:
    std::vector<std::string> keys() const
    { return _flags.keys(); }
    std::vector<std::string> values() const
    { return _flags.values(); }

public:
    bool exists(std::string const & key)
    { return _flags.existsByKey(key); }
    bool get(std::string const & key, std::string & val) const
    { return _flags.get(key, val); }

public:
    std::string opt(std::string const & key, std::string const & default_val) const
    { return _flags.opt(key, default_val); }
    std::string opt(std::string const & key) const
    { return _flags.opt(key); }

public:
    void push(Flag const & flag)
    { _flags.push(flag); }
    void push(std::string const & key, std::string const & value)
    { _flags.push(key, value); }
    void push(std::string && key, std::string && value)
    { _flags.push(std::move(key), std::move(value)); }

public:
    bool pushSystemEnvs();
    bool pushEnvs(char ** envs);
    bool pushEnvs(char ** envs, std::string const & delimiter);
    bool pushEnvs(std::string const & envs);
    bool pushEnvs(std::string const & envs, std::string const & delimiter);
    bool pushEnvs(std::vector<std::string> const & envs, std::string const & delimiter);

public:
    void readFromResource(Resource const & res);
    void readFromResourceXmlString(std::string const & xml);
    void readFromResourceXmlFile(std::string const & path);

public:
    void saveToResource(Resource & res);
    bool saveToResourceXmlString(std::string & xml);
    bool saveToResourceXmlFile(std::string const & path);

public:
    std::string convert(std::string const & source) const;
    std::string convert(std::string const & source, std::string const & regex_prefix, std::string const & regex_suffix) const;

public:
    std::vector<std::string> toStrings(std::string const & delimiter) const;
    std::vector<std::string> toStrings() const;

public:
    static std::size_t getEnvsSize(char ** envs);

public:
    TBAG_CONSTEXPR static char const * const EXE_PATH = "EXE_PATH";
    TBAG_CONSTEXPR static char const * const EXE_NAME = "EXE_NAME";
    TBAG_CONSTEXPR static char const * const EXE_DIR  = "EXE_DIR";
    TBAG_CONSTEXPR static char const * const WORK_DIR = "WORK_DIR";
    TBAG_CONSTEXPR static char const * const HOME_DIR = "HOME_DIR";

public:
    static Environments createDefaultEnvironments(bool with_system = true);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ENVIRONMENTS_HPP__

