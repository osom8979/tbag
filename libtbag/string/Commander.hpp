/**
 * @file   Commander.hpp
 * @brief  Commander class prototype.
 * @author zer0
 * @date   2016-10-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_COMMANDER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_COMMANDER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/Arguments.hpp>
#include <libtbag/string/Flags.hpp>

#include <string>
#include <vector>
#include <map>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * Commander class prototype.
 *
 * @author zer0
 * @date   2016-10-01
 */
class TBAG_API Commander
{
public:
    using ArgsVector = std::vector<Arguments>;
    using Callback   = std::function<void(Arguments const &)>;

    using CommandMap  = std::map<std::string, Callback>;
    using CommandPair = CommandMap::value_type;

public:
    TBAG_CONSTEXPR static char const * const DEFAULT_PREFIX = "--";
    TBAG_CONSTEXPR static char const * const DEFAULT_DELIMITER = "=";

private:
    std::string _prefix;
    std::string _delimiter;

    Callback   _default;
    CommandMap _commands;

public:
    Commander();
    Commander(std::string const & prefix, std::string const & delimiter);
    Commander(Callback const & default_callback);
    Commander(Commander const & obj);
    Commander(Commander && obj);
    virtual ~Commander();

public:
    Commander & operator =(Commander const & obj);
    Commander & operator =(Commander && obj);

public:
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_commands.empty()))
    { return _commands.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_commands.size()))
    { return _commands.size(); }

    inline void setDefaultCallback(Callback const & callback)
    { _default = callback; }

    inline void setPrefix(std::string const & prefix) { _prefix = prefix; }
    inline void setDelimiter(std::string const & delimiter) { _delimiter = delimiter; }

    inline void setDefaultPrefix() { _prefix = DEFAULT_PREFIX; }
    inline void setDefaultDelimiter() { _delimiter = DEFAULT_DELIMITER; }

    inline std::string getPrefix() const { return _prefix; }
    inline std::string getDelimiter() const { return _delimiter; }

public:
    void clear();
    bool insert(std::string const & command, Callback const & callback);

public:
    static ArgsVector parseArguments(Flags const & flags);
    static ArgsVector parseArguments(std::string const & arguments,
                                     std::string const & prefix = DEFAULT_PREFIX,
                                     std::string const & delimiter = DEFAULT_DELIMITER);

public:
    std::size_t request(ArgsVector const & args_vector);
    std::size_t request(Flags const & flags);
    std::size_t request(std::string const & arguments, std::string const & prefix, std::string const & delimiter);
    std::size_t request(std::string const & arguments);
    std::size_t request(int argc, char ** argv, std::string const & prefix, std::string const & delimiter, bool ignore_first = true);
    std::size_t request(int argc, char ** argv, bool ignore_first = true);

public:
    virtual bool onRequest(std::size_t index, std::size_t size, Arguments const & arguments);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_COMMANDER_HPP__

