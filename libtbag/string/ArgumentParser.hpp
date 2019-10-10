/**
 * @file   ArgumentParser.hpp
 * @brief  ArgumentParser class prototype.
 * @author zer0
 * @date   2019-10-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTPARSER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTPARSER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <vector>
#include <string>
#include <functional>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * ArgumentParser class prototype.
 *
 * @author zer0
 * @date   2019-10-09
 */
class TBAG_API ArgumentParser
{
public:
    struct Formatter
    {
        Formatter() { /* EMPTY. */ }
        virtual ~Formatter() { /* EMPTY. */ }

        virtual std::string print() = 0;
    };

    using SharedFormatter = std::shared_ptr<Formatter>;

    struct TBAG_API DefaultFormatter : public Formatter
    {
        DefaultFormatter() { /* EMPTY. */ }
        virtual ~DefaultFormatter() { /* EMPTY. */ }

        std::string print() override;
    };

    struct Params
    {
        /** The name of the program. */
        std::string program_name;

        /** The string describing the program usage (default: generated from arguments added to parser) */
        std::string usage;

        /** Text to display before the argument help (default: none) */
        std::string description;

        /** Text to display after the argument help (default: none) */
        std::string epilog;

        /** A class for customizing the help output */
        SharedFormatter formatter;

        /** The set of characters that prefix optional arguments (default: '--') */
        std::string prefix;

        /** The set of characters that prefix files from which additional arguments should be read (default: None) */
        std::string fromfile_prefix;

        /** Add a -h/--help option to the parser (default: True) */
        bool add_help = true;

        /** Allows long options to be abbreviated if the abbreviation is unambiguous. (default: True) */
        bool allow_abbrev = true;
    };

    enum class ActionType
    {
        /** This just stores the argument's value. This is the default action. */
        AT_STORE,

        /** This stores the value specified by the const keyword argument. */
        AT_STORE_CONST,
        AT_STORE_TRUE,
        AT_STORE_FALSE,

        /** This stores a list, and appends each argument value to the list. */
        AT_APPEND,
        AT_APPEND_CONST,

        /** This counts the number of times a keyword argument occurs. */
        AT_COUNT,

        AT_LAMBDA,
        AT_HELP,
        AT_VERSION,
    };

    struct Arg
    {
        /** Either a name or a list of option strings, e.g. foo or -f, --foo. */
        std::vector<std::string> names;

        /** The basic type of action to be taken when this argument is encountered at the command line. */
        ActionType action = ActionType::AT_STORE;

        /** The number of command-line arguments that should be consumed. */
        std::string nargs;

        /** A constant value required by some action and nargs selections. */
        std::string const_value;

        /** The value produced if the argument is absent from the command line. */
        std::string default_value;

        /** The type to which the command-line argument should be converted. */
        std::string type;

        /** A container of the allowable values for the argument. */
        std::vector<std::string> choices;

        /** Whether or not the command-line option may be omitted (optionals only). */
        bool required = false;

        /** A brief description of what the argument does. */
        std::string help;

        /** A name for the argument in usage messages. */
        std::string meta;

        /** The name of the attribute to be added. */
        std::string dest;

        std::function<void(void)> callback;
    };

public:
    ArgumentParser();
    virtual ~ArgumentParser();
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTPARSER_HPP__

