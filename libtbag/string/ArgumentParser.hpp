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
#include <libtbag/ErrPair.hpp>
#include <libtbag/string/Flags.hpp>

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
    struct Params;
    struct Arg;
    using Args = std::vector<Arg>;
    using ErrFlags = ErrPair<Flags>;

    struct Formatter
    {
        Formatter() { /* EMPTY. */ }
        virtual ~Formatter() { /* EMPTY. */ }

        virtual std::string print(Params const & params, Args const & args) = 0;
    };

    using SharedFormatter = std::shared_ptr<Formatter>;

    struct TBAG_API DefaultFormatter : public Formatter
    {
        DefaultFormatter() { /* EMPTY. */ }
        virtual ~DefaultFormatter() { /* EMPTY. */ }

        std::string print(Params const & params, Args const & args) override;
    };

    struct Params
    {
        /** The name of the program. */
        std::string program_name;

        /** The string describing the program usage. */
        std::string usage;

        /** Text to display before the argument help. */
        std::string description;

        /** Text to display after the argument help. */
        std::string epilog;

        /** A class for customizing the help output. */
        SharedFormatter formatter;

        /** The set of characters that prefix optional arguments. */
        std::string prefix;

        /** The set of characters that prefix files from which additional arguments should be read. */
        std::string fromfile_prefix;

        /** Add a -h/--help option to the parser (default: True) */
        bool add_help = true;

        /** Add a <code>--</code> option to the parser (default: True) */
        bool add_stop_parsing = true;

        /** Allows long options to be abbreviated if the abbreviation is unambiguous. */
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
    };

private:
    Params _params;
    Args _args;

public:
    ArgumentParser();
    explicit ArgumentParser(Params const & params);
    explicit ArgumentParser(Params && params);
    ArgumentParser(ArgumentParser const & obj);
    ArgumentParser(ArgumentParser && obj) TBAG_NOEXCEPT;
    virtual ~ArgumentParser();

public:
    ArgumentParser & operator =(ArgumentParser const & obj);
    ArgumentParser & operator =(ArgumentParser && obj) TBAG_NOEXCEPT;

public:
    void swap(ArgumentParser & obj) TBAG_NOEXCEPT;
    inline friend void swap(ArgumentParser & lh, ArgumentParser & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    Params const & params() const TBAG_NOEXCEPT
    { return _params; }
    Args const & args() const TBAG_NOEXCEPT
    { return _args; }

public:
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_args.empty())
    { return _args.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_args.size())
    { return _args.size(); }

public:
    void clearArgs();
    void addArg(Arg const & arg);
    void addArg(Arg && arg);

public:
    ErrFlags parse(int argc, char ** argv, bool ignore_first = true);
    ErrFlags parse(std::string const & argv, bool ignore_first = true);
    ErrFlags parse(std::vector<std::string> const & argv, bool ignore_first = true);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTPARSER_HPP__

