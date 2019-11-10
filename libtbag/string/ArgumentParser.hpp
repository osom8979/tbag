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
#include <libtbag/Type.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include <iterator>
#include <type_traits>

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
    struct InitParams;
    struct Param;
    struct ArgumentResult;

    using Params = std::vector<Param>;
    using ErrArgumentResult = ErrPair<ArgumentResult>;

    using Optionals = std::multimap<std::string, std::string>;
    using Positionals = std::map<std::string, std::string>;

    struct ArgumentResult
    {
        std::string program_name;
        Optionals optional;
        Positionals positional;
        std::vector<std::string> remain;

        std::size_t countOptional(std::string const & key) const
        { return optional.count(key); }

        std::vector<std::string> getOptionals(std::string const & key) const
        {
            std::vector<std::string> result;
            auto const range = optional.equal_range(key);
            for (auto itr = range.first; itr != range.second; ++itr) {
                result.emplace_back(itr->second);
            }
            return result;
        }

        std::string getOptional(std::string const & key, std::size_t index = 0, Err * code = nullptr) const
        {
            auto const range = optional.equal_range(key);
            auto const size = std::distance(range.first, range.second);
            if (index >= size) {
                if (code) {
                    *code = E_INDEX;
                }
                return {};
            } else {
                if (code) {
                    *code = E_SUCCESS;
                }
                return std::next(range.first, index)->second;
            }
        }

        template <typename ResultT>
        ResultT optOptional(std::string const & key, std::size_t index = 0, ResultT const & def = ResultT{}) const
        {
            Err code = E_UNKNOWN;
            auto const value = getOptional(key, index, &code);
            if (isFailure(code)) {
                return def;
            } else {
                return libtbag::string::toValue<ResultT>(value, def);
            }
        }

        std::string getPositional(std::string const & key) const
        {
            auto itr = positional.find(key);
            if (itr == positional.end()) {
                return {};
            }
            return itr->second;
        }
    };

    struct Formatter
    {
        Formatter() { /* EMPTY. */ }
        virtual ~Formatter() { /* EMPTY. */ }

        virtual std::string print(InitParams const & init, Params const & params) = 0;
    };

    using SharedFormatter = std::shared_ptr<Formatter>;

    struct TBAG_API DefaultFormatter : public Formatter
    {
        int padding_size = 2;

        DefaultFormatter(int p = 0) : padding_size(p) { /* EMPTY. */ }
        virtual ~DefaultFormatter() { /* EMPTY. */ }

        std::string print(InitParams const & init, Params const & params) override;
    };

    TBAG_CONSTEXPR static char const * const ARGUMENT_DELIMITER = " ";
    TBAG_CONSTEXPR static char const * const KEY_VAL_DELIMITER = "=";

    TBAG_CONSTEXPR static char const DEFAULT_PREFIX = '-';
    TBAG_CONSTEXPR static char const DEFAULT_FROM_FILE_PREFIX = '@';

    struct InitParams
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

        /**
         * This stores a list, and appends each argument value to the list.
         * This is useful to allow an option to be specified multiple times.
         */
        AT_APPEND,
    };

    TBAG_CONSTEXPR static char const * const ACTION_TYPE_STORE = "store";
    TBAG_CONSTEXPR static char const * const ACTION_TYPE_STORE_CONST = "store_const";
    TBAG_CONSTEXPR static char const * const ACTION_TYPE_APPEND = "append";
    TBAG_CONSTEXPR static char const * const VAL_TRUE_TEXT = "true";
    TBAG_CONSTEXPR static char const * const VAL_FALSE_TEXT = "false";

    static char const * getActionTypeName(ActionType type) TBAG_NOEXCEPT;
    static ActionType getActionType(std::string const & action_name);

    struct ParamElem
    {
        std::string key;
        std::string val;
    };

    struct ParamKey
    {
        char const * key;

        ParamElem operator =(char const * v) const
        { return ParamElem{key, v}; }
        ParamElem operator =(std::string const & v) const
        { return ParamElem{key, v}; }
        ParamElem operator =(std::string && v) const
        { return ParamElem{key, std::move(v)}; }

        // clang-format off
        ParamElem operator =(bool           v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(char           v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(short          v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(int            v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(long           v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(unsigned char  v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(unsigned short v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(unsigned int   v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(unsigned long  v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(float          v) const { return ParamElem{key, std::to_string(v)}; }
        ParamElem operator =(double         v) const { return ParamElem{key, std::to_string(v)}; }
        // clang-format on
    };

    // clang-format on
    TBAG_CONSTEXPR static char const * const ARG_NAME_KEY = "name";
    TBAG_CONSTEXPR static char const * const ARG_DEST_KEY = "dest";
    TBAG_CONSTEXPR static char const * const ARG_ACTION_KEY = "action";
    TBAG_CONSTEXPR static char const * const ARG_STORE_KEY = "store";
    TBAG_CONSTEXPR static char const * const ARG_STORE_CONST_KEY = "store_const";
    TBAG_CONSTEXPR static char const * const ARG_APPEND = "append";
    TBAG_CONSTEXPR static char const * const ARG_DEFAULT_KEY = "default";
    TBAG_CONSTEXPR static char const * const ARG_CONST_KEY = "const";
    TBAG_CONSTEXPR static char const * const ARG_HELP_KEY = "help";
    TBAG_CONSTEXPR static char const * const ARG_META_KEY = "meta";
    // clang-format off

    /**
     * @defgroup __DOXYGEN_GROUP__ARGUMENT_PARSER_DICTIONARY__ ArgumentParser Dictionary
     * @{
     */
    static ParamKey  const name;
    static ParamKey  const dest;
    static ParamKey  const action;
    static ParamKey  const default_value;
    static ParamKey  const const_value;
    static ParamKey  const help;
    static ParamKey  const meta;
    static ParamElem const store;
    static ParamElem const store_const;
    static ParamElem const append;
    /** @} */

    struct Param TBAG_FINAL
    {
        /**
         * Either a name or a list of option strings,
         * e.g. foo or <code>-f</code>, <code>--foo</code>.
         */
        std::vector<std::string> names;

        /** The basic type of action to be taken when this argument is encountered at the command line. */
        ActionType action = ActionType::AT_STORE;

        /** The name of the attribute to be added. */
        std::string dest;

        /** The value produced if the argument is absent from the command line. */
        std::string default_value;

        /** A constant value required by some action and nargs selections. */
        std::string const_value;

        /** A brief description of what the argument does. */
        std::string help;

        /** A name for the argument in usage messages. */
        std::string meta;

        friend Param & operator << (Param & a, ParamElem const & elem)
        {
            if (elem.key == ARG_NAME_KEY) {
                a.names.emplace_back(elem.val);
            } else if (elem.key == ARG_DEST_KEY) {
                a.dest = elem.val;
            } else if (elem.key == ARG_ACTION_KEY) {
                a.action = getActionType(elem.val);
            } else if (elem.key == ARG_STORE_KEY) {
                a.action = ActionType::AT_STORE;
            } else if (elem.key == ARG_STORE_CONST_KEY) {
                a.action = ActionType::AT_STORE_CONST;
            } else if (elem.key == ARG_APPEND) {
                a.action = ActionType::AT_APPEND;
            } else if (elem.key == ARG_DEFAULT_KEY) {
                a.default_value = elem.val;
            } else if (elem.key == ARG_CONST_KEY) {
                a.const_value = elem.val;
            } else if (elem.key == ARG_HELP_KEY) {
                a.help = elem.val;
            } else if (elem.key == ARG_META_KEY) {
                a.meta = elem.val;
            }
            return a;
        }
    };

private:
    InitParams _init;
    Params _params;

public:
    ArgumentParser();
    explicit ArgumentParser(InitParams const & params);
    explicit ArgumentParser(InitParams && params);
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
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_params.empty())
    { return _params.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_params.size())
    { return _params.size(); }

public:
    void clear();

public:
    static std::size_t calcMaxNameSize(Params const & params);
    static void getOptionalsAndPositionals(Params const & params,
                                           char prefix,
                                           Params & optionals,
                                           Params & positionals);

public:
    Params::iterator findParameter(std::string const & option_name);
    Params::const_iterator findConstantParameter(std::string const & option_name) const;

public:
    std::string removePrefix(std::string const & text);
    Err updateParam(Param & arg);

public:
    bool insertToOptional(Optionals & optional, std::string const & dest_name, std::string const & val) const;

private:
    Err add_param(Param const & arg);
    Err add_param(Param && arg);
    Err add_elems(std::vector<ParamElem> const & elems);

    struct add_param_t { /* EMPTY. */ };
    struct add_first_string_param_t { /* EMPTY. */ };
    struct add_param_elems_t { /* EMPTY. */ };

    template <typename ... ArgsT>
    Err add_select(add_param_elems_t, ArgsT && ... args)
    {
        return add_elems({std::forward<ArgsT>(args) ...});
    }

    template <typename ... ArgsT>
    Err add_select(add_first_string_param_t, std::string const & name, ArgsT && ... args)
    {
        return add_param(Param{{name}, std::forward<ArgsT>(args) ...});
    }

    template <typename ... ArgsT>
    Err add_select(add_param_t, ArgsT && ... args)
    {
        return add_param(Param{std::forward<ArgsT>(args) ...});
    }

    template <typename ... T>
    struct is_all_param_elem;

    template <typename Head, typename ... Tail>
    struct is_all_param_elem<Head, Tail...>
    {
        TBAG_CONSTEXPR static bool const value = is_all_param_elem<Head>::value && is_all_param_elem<Tail ...>::value;
    };

    template <typename T>
    struct is_all_param_elem<T>
    {
        TBAG_CONSTEXPR static bool const value = std::is_same<
                typename libtbag::remove_cr<T>::type,
                ParamElem
        >::value;
    };

    template <typename ... T>
    struct is_first_string;

    template <typename Head, typename ... Tail>
    struct is_first_string<Head, Tail...>
    {
        TBAG_CONSTEXPR static bool const value = is_first_string<Head>::value;
    };

    template <typename T>
    struct is_first_string<T>
    {
        TBAG_CONSTEXPR static bool const value =
                std::is_same<typename remove_cr<T>::type, std::string>::value ||
                is_string_literal<T>::value;
    };

public:
    template <typename ... ArgsT>
    Err add(ArgsT && ... args)
    {
        using __select_t = typename std::conditional<
                is_all_param_elem<ArgsT ...>::value,
                add_param_elems_t,
                typename std::conditional<
                        is_first_string<ArgsT ...>::value,
                        add_first_string_param_t,
                        add_param_t
                >::type
        >::type;
        static_assert(sizeof...(ArgsT) >= 1u, "Greater one template argument is required.");
        return add_select(__select_t{}, std::forward<ArgsT>(args) ...);
    }

    Err add(Param const & arg)
    {
        return add_param(arg);
    }

    Err add(Param && arg)
    {
        return add_param(std::move(arg));
    }

public:
    ErrArgumentResult parse(int argc, char ** argv) const;
    ErrArgumentResult parse(std::string const & argv) const;
    ErrArgumentResult parse(std::vector<std::string> const & argv) const;

public:
    std::string print() const;

private:
    enum class ParseResultCode
    {
        PRC_ERROR,
        PRC_FROM_FILE_ERROR,
        PRC_SKIP,
        PRC_SINGLE_PREFIX,
        PRC_DOUBLE_PREFIX,
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

TBAG_CONSTEXPR char const * const TBAG_MAIN_ARGUMENT_PARSER_KEY = "libtbag::string::ArgumentParser";

TBAG_API std::weak_ptr<ArgumentParser> getMainArgumentParser();

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTPARSER_HPP__

