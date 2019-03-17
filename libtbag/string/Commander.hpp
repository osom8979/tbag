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
 *
 * @remarks
 *  Callback loop:
 *  <pre>
 *   +- Arguments foreach
 *   |        |
 *   |   onRequest() *--{return_yes}--> skip callback *----------------------------------------------------------->+
 *   |               |                                                                                             |
 *   |               +--{return_no}---> find command callback *----{find_callback}-----> call command callback *-->+
 *   |                                                        |                                                    |
 *   |                                                        +--{not_found_callback}--> call default callback *-->+
 *   |                                                                                                             |
 *   +------------------------------------------------- loop end <-------------------------------------------------+
 *  </pre>
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

    bool _call_once;

public:
    Commander();
    Commander(std::string const & prefix, std::string const & delimiter);
    Commander(Callback const & default_callback);
    Commander(Commander const & obj);
    Commander(Commander && obj) TBAG_NOEXCEPT;
    virtual ~Commander();

public:
    Commander & operator =(Commander const & obj);
    Commander & operator =(Commander && obj) TBAG_NOEXCEPT;

public:
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_commands.empty())
    { return _commands.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_commands.size())
    { return _commands.size(); }

    inline void setDefaultCallback(Callback const & callback)
    { _default = callback; }

    inline void setPrefix(std::string const & prefix) { _prefix = prefix; }
    inline void setDelimiter(std::string const & delimiter) { _delimiter = delimiter; }

    inline void setDefaultPrefix() { _prefix = DEFAULT_PREFIX; }
    inline void setDefaultDelimiter() { _delimiter = DEFAULT_DELIMITER; }

    inline std::string getPrefix() const { return _prefix; }
    inline std::string getDelimiter() const { return _delimiter; }

    inline void setCallOnce(bool flag = true) { _call_once = flag; }
    inline bool isCallOnce() const TBAG_NOEXCEPT { return _call_once; }

public:
    void clear();
    bool insert(std::string const & command, Callback const & callback);

public:
    static ArgsVector parseArguments(Flags const & flags);
    static ArgsVector parseArguments(std::string const & arguments,
                                     std::string const & prefix = DEFAULT_PREFIX,
                                     std::string const & delimiter = DEFAULT_DELIMITER);
    static ArgsVector parseArguments(std::vector<std::string> const & arguments,
                                     std::string const & prefix = DEFAULT_PREFIX,
                                     std::string const & delimiter = DEFAULT_DELIMITER);

public:
    void setDefaultCallbackForLeftArguments(std::vector<std::string> * left_arguments,
                                            std::vector<std::string> * unknown_flags = nullptr);

public:
    std::size_t request(ArgsVector const & args_vector);
    std::size_t request(Flags const & flags);
    std::size_t request(std::string const & arguments, std::string const & prefix, std::string const & delimiter);
    std::size_t request(std::string const & arguments);

    /**
     * @param[in] argc
     *      Count of arguments.
     * @param[in] argv
     *      Variables of arguments.
     * @param[in] prefix
     *      Prefix of commander.
     * @param[in] delimiter
     *      Delimiter of commander.
     * @param[in] ignore_first
     *      The argv[0] passed to the main() function is the program path. @n
     *      Usually this value is not used and can be IGNORE processed.
     */
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

