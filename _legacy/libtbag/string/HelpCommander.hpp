/**
 * @file   HelpCommander.hpp
 * @brief  HelpCommander class prototype.
 * @author zer0
 * @date   2017-05-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_HELPCOMMANDER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_HELPCOMMANDER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/Commander.hpp>
#include <libtbag/Type.hpp>

#include <cassert>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * HelpCommander class prototype.
 *
 * @author zer0
 * @date   2017-05-26
 */
class TBAG_API HelpCommander : public Commander
{
public:
    TBAG_CONSTEXPR static char const * const DEFAULT_HELP_COMMAND = "help";

public:
    using HelpMap  = std::map<std::string, std::string>;
    using HelpPair = HelpMap::value_type;

private:
    std::string _synopsis;
    std::string _remarks;

    HelpMap _helps;
    HelpMap _params;

public:
    HelpCommander();
    HelpCommander(std::string const & prefix, std::string const & delimiter);
    HelpCommander(std::string const & prefix, std::string const & delimiter,
                  std::string const & synopsis, std::string const & remarks);
    HelpCommander(Commander::Callback const & default_callback);
    HelpCommander(HelpCommander const & obj);
    HelpCommander(HelpCommander && obj);
    virtual ~HelpCommander();

public:
    HelpCommander & operator =(HelpCommander const & obj);
    HelpCommander & operator =(HelpCommander && obj);

public:
    inline std::string getSynopsis() const { return _synopsis; }
    inline std::string getRemarks () const { return _remarks ; }

    inline HelpMap getHelps () const { return _helps;  }
    inline HelpMap getParams() const { return _params; }

    inline void setSynopsis(std::string const & content) { _synopsis = content; }
    inline void setRemarks (std::string const & content) { _remarks  = content; }

    inline void info(std::string const & prefix,
                     std::string const & delimiter,
                     std::string const & synopsis,
                     std::string const & remarks)
    {
        setPrefix(prefix);
        setDelimiter(delimiter);
        setSynopsis(synopsis);
        setRemarks(remarks);
    }

public:
    void clear();

public:
    bool insertHelpCommand(Callback const & callback);
    bool insert(std::string const & command, Callback const & callback, std::string const & help_msg);
    bool insert(std::string const & command, Callback const & callback, std::string const & help_msg, std::string const & help_param);

public:
    template <typename ValueType>
    bool insertDefault(std::string const & command,
                       ValueType * value_pointer,
                       typename std::remove_reference<ValueType>::type default_value,
                       std::string const & help_msg = std::string(),
                       std::string const & help_param = std::string(),
                       std::size_t argument_index = 0)
    {
        static_assert(std::is_const<ValueType>::value == false, "The ValueType must not be const type.");
        assert(value_pointer != nullptr);
        return insert(command, [value_pointer, default_value, argument_index](Arguments const & args){
            if (args.opt(argument_index, value_pointer) == false) {
                *value_pointer = default_value;
            }
        }, help_msg, help_param);
    }

public:
    std::string help(std::string const & head, bool auto_padding = false) const;
    std::string help(bool auto_padding = false) const;

public:
    static std::size_t getMaxCommandSize(HelpMap const & helps);
    static std::string getPaddingCommandHelp(std::string const & prefix, HelpMap const & helps, int padding_size = 2);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_HELPCOMMANDER_HPP__

