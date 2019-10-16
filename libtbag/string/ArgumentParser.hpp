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

#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <map>

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
    struct Arguments;

    using Args = std::vector<Arg>;
    using ErrArguments = ErrPair<Arguments>;
    using ErrString = ErrPair<std::string>;

    struct Arguments
    {
        std::map<std::string, std::string> optional;
        std::map<std::string, std::string> positional;
        std::vector<std::string> remain;
    };

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

    TBAG_CONSTEXPR static char const * const ARGUMENT_DELIMITER = " ";
    TBAG_CONSTEXPR static char const * const KEY_VAL_DELIMITER = "=";

    TBAG_CONSTEXPR static char const DEFAULT_PREFIX = '-';
    TBAG_CONSTEXPR static char const DEFAULT_FROM_FILE_PREFIX = '@';

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
        char prefix = DEFAULT_PREFIX;

        /** The set of characters that prefix files from which additional arguments should be read. */
        char from_file_prefix = DEFAULT_FROM_FILE_PREFIX;

        /** Add a <code>--</code> option to the parser. */
        bool add_stop_parsing = true;

        /** The first argument is the program name. */
        bool first_argument_is_program_name = true;
    };

    enum class ActionType
    {
        AT_NONE,

        /** This just stores the argument's value. This is the default action. */
        AT_STORE,

        /** This stores the value specified by the const keyword argument. */
        AT_STORE_CONST,
    };

    TBAG_CONSTEXPR static char const * const ACTION_TYPE_STORE = "store";
    TBAG_CONSTEXPR static char const * const ACTION_TYPE_STORE_CONST = "store_const";

    static char const * getActionTypeName(ActionType type) TBAG_NOEXCEPT;
    static ActionType getActionType(std::string const & name);

    struct arg_key_val
    {
        std::string key;
        std::string val;

        arg_key_val(std::string const & k) : key(k) { /* EMPTY. */ }
        ~arg_key_val() { /* EMPTY. */ }

        arg_key_val & operator =(std::string const & v)
        {
            val = v;
            return *this;
        }
    };

    TBAG_CONSTEXPR static char const * const ARG_NAME_KEY    = "name";
    TBAG_CONSTEXPR static char const * const ARG_DEST_KEY    = "dest";
    TBAG_CONSTEXPR static char const * const ARG_ACTION_KEY  = "action";
    TBAG_CONSTEXPR static char const * const ARG_DEFAULT_KEY = "default";
    TBAG_CONSTEXPR static char const * const ARG_CONST_KEY   = "const";
    TBAG_CONSTEXPR static char const * const ARG_HELP_KEY    = "help";
    TBAG_CONSTEXPR static char const * const ARG_META_KEY    = "meta";

    // clang-format on
    struct name          : public arg_key_val { name         () : arg_key_val(ARG_NAME_KEY   ) {} };
    struct dest          : public arg_key_val { dest         () : arg_key_val(ARG_DEST_KEY   ) {} };
    struct action        : public arg_key_val { action       () : arg_key_val(ARG_ACTION_KEY ) {} };
    struct default_value : public arg_key_val { default_value() : arg_key_val(ARG_DEFAULT_KEY) {} };
    struct const_value   : public arg_key_val { const_value  () : arg_key_val(ARG_CONST_KEY  ) {} };
    struct help          : public arg_key_val { help         () : arg_key_val(ARG_HELP_KEY   ) {} };
    struct meta          : public arg_key_val { meta         () : arg_key_val(ARG_META_KEY   ) {} };
    // clang-format off

    struct Arg
    {
        /** Either a name or a list of option strings, e.g. foo or -f, --foo. */
        std::vector<std::string> names;

        /** The name of the attribute to be added. */
        std::string dest;

        /** The basic type of action to be taken when this argument is encountered at the command line. */
        ActionType action = ActionType::AT_STORE;

        /** The value produced if the argument is absent from the command line. */
        std::string default_value;

        /** A constant value required by some action and nargs selections. */
        std::string const_value;

        /** A brief description of what the argument does. */
        std::string help;

        /** A name for the argument in usage messages. */
        std::string meta;

        friend Arg & operator << (Arg & a, arg_key_val const & kv)
        {
            if (kv.key == ARG_NAME_KEY) {
                a.names.emplace_back(kv.val);
            } else if (kv.key == ARG_DEST_KEY) {
                a.dest = kv.val;
            } else if (kv.key == ARG_ACTION_KEY) {
                a.action = getActionType(kv.val);
            } else if (kv.key == ARG_DEFAULT_KEY) {
                a.default_value = kv.val;
            } else if (kv.key == ARG_CONST_KEY) {
                a.const_value = kv.val;
            } else if (kv.key == ARG_HELP_KEY) {
                a.help = kv.val;
            } else if (kv.key == ARG_META_KEY) {
                a.meta = kv.val;
            }
            return a;
        }
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
    inline Params       & params()       TBAG_NOEXCEPT { return _params; }
    inline Params const & params() const TBAG_NOEXCEPT { return _params; }

public:
    inline Args       & args()       TBAG_NOEXCEPT { return _args; }
    inline Args const & args() const TBAG_NOEXCEPT { return _args; }

public:
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_args.empty())
    { return _args.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_args.size())
    { return _args.size(); }

public:
    void clear();

public:
    void add(Arg const & arg);
    void add(Arg && arg);

public:
    void addPairs(std::vector<arg_key_val> const & kvs);

public:
    template <typename ... ArgKeyValT>
    void addPairs(ArgKeyValT && ... args)
    {
        add({std::forward<ArgKeyValT>(args) ...});
    }

public:
    ErrArguments parse(int argc, char ** argv) const;
    ErrArguments parse(std::string const & argv) const;
    ErrArguments parse(std::vector<std::string> const & argv) const;

public:
    enum class ParseResultCode
    {
        PRC_ERROR,
        PRC_FROM_FILE_ERROR,
        PRC_SKIP,
        PRC_STDIN,
        PRC_STOP_PARSING,
        PRC_KEY_VAL,
        PRC_KEY,
        PRC_VAL,
    };

    struct ParseResult
    {
        ParseResultCode code;
        std::string key;
        std::string value;
    };

    ParseResult parseSingleArgument(std::string const & arg) const;
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTPARSER_HPP__

