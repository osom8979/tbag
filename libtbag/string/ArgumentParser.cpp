/**
 * @file   ArgumentParser.cpp
 * @brief  ArgumentParser class implementation.
 * @author zer0
 * @date   2019-10-09
 */

#include <libtbag/string/ArgumentParser.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/algorithm/MinMax.hpp>

#include <cassert>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <queue>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

std::string ArgumentParser::DefaultFormatter::print(InitParams const & init, Params const & params)
{
    Params optionals;
    Params positionals;
    ArgumentParser::getOptionalsAndPositionals(params, init.prefix, optionals, positionals);
    auto const padding_string = std::string(padding_size, ' ');

    std::stringstream ss;
    if (!init.usage.empty()) {
        ss << init.usage << std::endl;
    }

    if (!init.description.empty()) {
        ss << init.description << std::endl;
    }

    ss << "Optionals:" << std::endl;
    for (auto const & p : optionals) {
        auto const name_size = p.names.size();
        if (name_size == 0u) {
            continue;
        }
        if (name_size >= 1u) {
            ss << padding_string << p.names[0];
        }
        for (std::size_t i = 1u; i < name_size; ++i) {
            ss << ", " << p.names[i];
        }
        if (!p.meta.empty()) {
            ss << " " << p.meta;
        }
        ss << std::endl;
        if (!p.help.empty()) {
            ss << padding_string << padding_string << p.help << std::endl;
        }
        if (!p.default_value.empty()) {
            ss << "[Default: " << p.default_value << "]" << std::endl;
        }
    }

    auto const positionals_max_name_size = ArgumentParser::calcMaxNameSize(positionals);
    ss << "Positionals:" << std::endl;
    for (auto const & p : positionals) {
        if (p.names.empty()) {
            continue;
        }
        ss << padding_string
           << p.names[0]
           << std::string(positionals_max_name_size - p.names[0].size(), ' ')
           << padding_string;
        if (!p.help.empty()) {
            ss << p.help;
        }
        if (!p.default_value.empty()) {
            if (!p.help.empty()) {
                ss << " ";
            }
            ss << "[Default: " << p.default_value << "]";
        }
        ss << std::endl;
    }

    if (!init.epilog.empty()) {
        ss << init.epilog << std::endl;
    }

    return ss.str();
}

// --------------
// ArgumentParser
// --------------

using Param = ArgumentParser::Param;
using Params = ArgumentParser::Params;
using ErrArgumentResult = ArgumentParser::ErrArgumentResult;
using ActionType = ArgumentParser::ActionType;

// clang-format on
ArgumentParser::ParamKey  const ArgumentParser::name          = { ArgumentParser::ARG_NAME_KEY        };
ArgumentParser::ParamKey  const ArgumentParser::dest          = { ArgumentParser::ARG_DEST_KEY        };
ArgumentParser::ParamKey  const ArgumentParser::action        = { ArgumentParser::ARG_ACTION_KEY      };
ArgumentParser::ParamKey  const ArgumentParser::default_value = { ArgumentParser::ARG_DEFAULT_KEY     };
ArgumentParser::ParamKey  const ArgumentParser::const_value   = { ArgumentParser::ARG_CONST_KEY       };
ArgumentParser::ParamKey  const ArgumentParser::help          = { ArgumentParser::ARG_HELP_KEY        };
ArgumentParser::ParamKey  const ArgumentParser::meta          = { ArgumentParser::ARG_META_KEY        };
ArgumentParser::ParamElem const ArgumentParser::store         = { ArgumentParser::ARG_STORE_KEY       };
ArgumentParser::ParamElem const ArgumentParser::store_const   = { ArgumentParser::ARG_STORE_CONST_KEY };
ArgumentParser::ParamElem const ArgumentParser::append        = { ArgumentParser::ARG_APPEND          };
// clang-format off

char const * ArgumentParser::getActionTypeName(ActionType type) TBAG_NOEXCEPT
{
    switch (type) {
    case ActionType::AT_STORE:
        return ACTION_TYPE_STORE;
    case ActionType::AT_STORE_CONST:
        return ACTION_TYPE_STORE_CONST;
    case ActionType::AT_APPEND:
        return ACTION_TYPE_APPEND;
    default:
        return "";
    }
}

ActionType ArgumentParser::getActionType(std::string const & action_name)
{
    if (action_name == ACTION_TYPE_STORE) {
        return ActionType::AT_STORE;
    } else if (action_name == ACTION_TYPE_STORE_CONST) {
        return ActionType::AT_STORE_CONST;
    } else if (action_name == ACTION_TYPE_APPEND) {
        return ActionType::AT_APPEND;
    } else {
        return ActionType::AT_NONE;
    }
}

ArgumentParser::ArgumentParser()
{
    // EMPTY.
}

ArgumentParser::ArgumentParser(InitParams const & params) : _init(params)
{
    // EMPTY.
}

ArgumentParser::ArgumentParser(InitParams && params) : _init(std::move(params))
{
    // EMPTY.
}

ArgumentParser::ArgumentParser(ArgumentParser const & obj)
        : _init(obj._init), _params(obj._params)
{
    // EMPTY.
}

ArgumentParser::ArgumentParser(ArgumentParser && obj) TBAG_NOEXCEPT
        : _init(std::move(obj._init)), _params(std::move(obj._params))
{
    // EMPTY.
}

ArgumentParser::~ArgumentParser()
{
    // EMPTY.
}

ArgumentParser & ArgumentParser::operator =(ArgumentParser const & obj)
{
    if (this != &obj) {
        _init = obj._init;
        _params = obj._params;
    }
    return *this;
}

ArgumentParser & ArgumentParser::operator =(ArgumentParser && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _init = std::move(obj._init);
        _params = std::move(obj._params);
    }
    return *this;
}

void ArgumentParser::swap(ArgumentParser & obj) TBAG_NOEXCEPT
{
    std::swap(_init, obj._init);
    _params.swap(obj._params);
}

void ArgumentParser::clear()
{
    _params.clear();
}

std::size_t ArgumentParser::calcMaxNameSize(Params const & params)
{
    std::size_t result = 0u;
    for (auto const & p : params) {
        for (auto const & n : p.names) {
            result = libtbag::algorithm::getMax(result, n.size());
        }
    }
    return result;
}

void ArgumentParser::getOptionalsAndPositionals(Params const & params, char prefix, Params & optionals, Params & positionals)
{
    for (auto const & p : params) {
        if (p.names.empty()) {
            continue;
        }
        if (p.names.size() == 1u && !p.names[0].empty() && p.names[0][0] != prefix) {
            positionals.emplace_back(p);
        } else {
            optionals.emplace_back(p);
        }
    }
}

Params::iterator ArgumentParser::findParameter(std::string const & option_name)
{
    return std::find_if(_params.begin(), _params.end(), [&option_name](Params::const_reference p) -> bool {
        return std::find(p.names.begin(), p.names.end(), option_name) != p.names.cend();
    });
}

Params::const_iterator ArgumentParser::findConstantParameter(std::string const & option_name) const
{
    return std::find_if(_params.cbegin(), _params.cend(), [&option_name](Params::const_reference p) -> bool {
        return std::find(p.names.cbegin(), p.names.cend(), option_name) != p.names.cend();
    });
}

std::string ArgumentParser::removePrefix(std::string const & text)
{
    std::size_t i = 0u;
    for (; i < text.size(); ++i) {
        if (text[i] != _init.prefix) {
            break;
        }
    }
    return text.substr(i);
}

Err ArgumentParser::updateParam(Param & arg)
{
    if (arg.names.empty()) {
        return E_ILLARGS;
    }
    if (arg.dest.empty()) {
        std::string longest_name;
        for (auto const & n : arg.names) {
            auto const updated_name = removePrefix(n);
            if (updated_name.size() > longest_name.size()) {
                longest_name = updated_name;
            }
        }
        arg.dest = longest_name;
        if (arg.dest.empty()) {
            return E_ILLARGS;
        }
    }
    return E_SUCCESS;
}

bool ArgumentParser::insertToOptional(Optionals & optional, std::string const & dest_name, std::string const & val) const
{
    auto param_itr = std::find_if(_params.cbegin(), _params.cend(), [&dest_name](Params::const_reference p) -> bool {
        return p.dest == dest_name;
    });
    if (param_itr == _params.cend()) {
        return false;
    }

    if (param_itr->action == ActionType::AT_APPEND) {
        optional.emplace(dest_name, val);
    } else {
        auto optional_itr = optional.find(dest_name);
        if (optional_itr == optional.end()) {
            optional.emplace(dest_name, val);
        } else {
            optional_itr->second.assign(val);
        }
    }
    return true;
}

Err ArgumentParser::add_param(Param const & arg)
{
    Param updated_arg = arg;
    auto const code = updateParam(updated_arg);
    if (isFailure(code)) {
        return code;
    }
    _params.emplace_back(std::move(updated_arg));
    return E_SUCCESS;
}

Err ArgumentParser::add_param(Param && arg)
{
    auto const code = updateParam(arg);
    if (isFailure(code)) {
        return code;
    }
    _params.emplace_back(arg);
    return E_SUCCESS;
}

Err ArgumentParser::add_elems(std::vector<ParamElem> const & elems)
{
    Param arg;
    for (auto const & e : elems) {
        arg << e;
    }
    return add(std::move(arg));
}

ErrArgumentResult ArgumentParser::parse(int argc, char ** argv) const
{
    if (argc <= 0 || argv == nullptr) {
        return E_ILLARGS;
    }
    std::vector<std::string> arguments(argc);
    for (int i = 0; i < argc; ++i) {
        arguments[i] = argv[i];
    }
    return parse(arguments);
}

ErrArgumentResult ArgumentParser::parse(std::string const & argv) const
{
    return parse(libtbag::string::splitTokens(argv, ARGUMENT_DELIMITER));
}

enum argument_required_type
{
    argument_required_type_none,
    argument_required_type_wait_optional_value,
};

ErrArgumentResult ArgumentParser::parse(std::vector<std::string> const & argv) const
{
    if (argv.empty()) {
        return E_ILLARGS;
    }

    std::queue<std::string> positional_names;
    COMMENT("Find positional arguments.") {
        for (auto const & arg : _params) {
            if (arg.names.size() == 1 && !arg.names[0].empty() && arg.names[0][0] != _init.prefix) {
                positional_names.push(arg.names[0]);
            }
        }
    }

    std::unordered_map<std::string, std::string> key_map;
    std::string selected_key = {};
    ArgumentResult result = {};

    COMMENT("Update key map.") {
        for (auto const & arg : _params) {
            for (auto const & n : arg.names) {
                key_map[n] = arg.dest;
            }
        }
    }

    COMMENT("Update default variables.") {
        for (auto const & arg : _params) {
            if (arg.names.empty()) {
                continue;
            }
            if (arg.names[0].empty()) {
                continue;
            }
            if (arg.default_value.empty()) {
                continue;
            }
            if (arg.names[0][0] == _init.prefix) {
                insertToOptional(result.optional, arg.dest, arg.default_value);
            } else {
                result.positional[arg.dest] = arg.default_value;
            }
        }
    }

    argument_required_type req_type = argument_required_type_none;
    bool stop_parsing = false;

    auto const size = argv.size();
    assert(size >= 1u);

    if (_init.first_argument_is_program_name) {
        result.program_name = argv[0];
    } else {
        result.program_name = _init.program_name;
    }

    for (std::size_t i = _init.first_argument_is_program_name?1u:0u; i < size; ++i) {
        auto const & arg = argv[i];
        if (_init.add_stop_parsing && stop_parsing) {
            result.remain.emplace_back(arg);
            continue;
        }

        auto const parse_result = parseSingleArgument(libtbag::string::trim(arg));
        switch (parse_result.code) {
        case ParseResultCode::PRC_ERROR:
            return E_PARSING;

        case ParseResultCode::PRC_FROM_FILE_ERROR:
            return E_RDERR;

        case ParseResultCode::PRC_SKIP:
            break;

        case ParseResultCode::PRC_SINGLE_PREFIX:
            assert(std::string(1u, _init.prefix) == parse_result.key);
            if (req_type == argument_required_type_wait_optional_value) {
                insertToOptional(result.optional, key_map[selected_key], parse_result.key);
                req_type = argument_required_type_none;
            } else {
                assert(req_type == argument_required_type_none);
                insertToOptional(result.optional, parse_result.key, VAL_TRUE_TEXT);
            }
            break;

        case ParseResultCode::PRC_DOUBLE_PREFIX:
            assert(std::string(2u, _init.prefix) == parse_result.key);
            if (req_type == argument_required_type_wait_optional_value) {
                insertToOptional(result.optional, key_map[selected_key], parse_result.key);
                req_type = argument_required_type_none;
            } else {
                assert(req_type == argument_required_type_none);
                insertToOptional(result.optional, parse_result.key, VAL_TRUE_TEXT);
                stop_parsing = true;
            }
            break;

        case ParseResultCode::PRC_KEY_VAL:
            if (req_type != argument_required_type_none) {
                return E_PARSING;
            }
            insertToOptional(result.optional, key_map[parse_result.key], parse_result.value);
            break;

        case ParseResultCode::PRC_KEY:
            if (req_type == argument_required_type_none) {
                auto const itr = findConstantParameter(parse_result.key);
                if (itr != _params.cend()) {
                    switch (itr->action) {
                    case ActionType::AT_STORE:
                        TBAG_FALLTHROUGH
                    case ActionType::AT_APPEND:
                        selected_key = parse_result.key;
                        req_type = argument_required_type_wait_optional_value;
                        break;
                    case ActionType::AT_STORE_CONST:
                        insertToOptional(result.optional, key_map[parse_result.key], itr->const_value);
                        break;
                    default:
                        break;
                    }
                }
            } else {
                return E_PARSING;
            }
            break;

        case ParseResultCode::PRC_VAL:
            switch (req_type) {
            case argument_required_type_none:
                if (positional_names.empty()) {
                    result.remain.emplace_back(parse_result.value);
                } else {
                    result.positional[key_map[positional_names.front()]] = parse_result.value;
                    positional_names.pop();
                }
                break;
            case argument_required_type_wait_optional_value:
                insertToOptional(result.optional, key_map[selected_key], parse_result.value);
                req_type = argument_required_type_none;
                break;
            default:
                return E_INACCESSIBLE_BLOCK;
            }
            break;

        default:
            return E_INACCESSIBLE_BLOCK;
        }
    }

    while (!positional_names.empty()) {
        auto const n = positional_names.front();
        auto const itr = findConstantParameter(n);
        assert(itr != _params.cend());
        if (itr->default_value.empty()) {
            return E_ILLARGS;
        }
        result.positional[key_map[n]] = itr->default_value;
        positional_names.pop();
    }

    if (req_type != argument_required_type_none) {
        return E_ILLSTATE;
    }

    return { E_SUCCESS, result };
}

std::string ArgumentParser::print() const
{
    if (!_init.formatter) {
        return {};
    }
    return _init.formatter->print(_init, _params);
}

ArgumentParser::ParseResult ArgumentParser::parseSingleArgument(std::string const & arg) const
{
    using namespace libtbag::filesystem;
    auto const arg_size = arg.size();
    if (arg_size == 0u) {
        return { ParseResultCode::PRC_SKIP };
    }

    if (arg_size == 1u) {
        if (arg[0] == _init.prefix) {
            return { ParseResultCode::PRC_SINGLE_PREFIX, std::string(1u, _init.prefix) };
        } else {
            return { ParseResultCode::PRC_VAL, {}, arg };
        }
    }

    if (arg_size == 2u) {
        if (arg[0] == _init.prefix) {
            if (arg[1] == _init.prefix) {
                return { ParseResultCode::PRC_DOUBLE_PREFIX, std::string(2u, _init.prefix) };
            } else {
                return { ParseResultCode::PRC_KEY, arg };
            }
        } else if (arg[0] == _init.from_file_prefix) {
            std::string content;
            if (isSuccess(readFile(std::string(1u, arg[1]), content))) {
                return { ParseResultCode::PRC_VAL, {}, content };
            } else {
                return { ParseResultCode::PRC_FROM_FILE_ERROR };
            }
        } else {
            return { ParseResultCode::PRC_VAL, {}, arg };
        }
    }

    assert(arg_size >= 3u);
    ParseResult result = {};
    if (arg[0] == _init.prefix) {
        auto const split_pos = arg.find(KEY_VAL_DELIMITER);
        if (split_pos != std::string::npos) {
            result.code = ParseResultCode::PRC_KEY_VAL;
            result.key = arg.substr(0, split_pos);
            auto const temp_value = arg.substr(split_pos+1);
            if (!temp_value.empty() && temp_value[0] == _init.from_file_prefix) {
                if (isSuccess(readFile(temp_value.substr(1), result.value))) {
                    result.code = ParseResultCode::PRC_VAL;
                } else {
                    return { ParseResultCode::PRC_FROM_FILE_ERROR };
                }
            } else {
                result.value = temp_value;
            }
        } else {
            result.code = ParseResultCode::PRC_KEY;
            result.key = arg;
        }
    } else if (arg[0] == _init.from_file_prefix) {
        if (isSuccess(readFile(arg.substr(1), result.value))) {
            result.code = ParseResultCode::PRC_VAL;
        } else {
            return { ParseResultCode::PRC_FROM_FILE_ERROR };
        }
    } else {
        result.code = ParseResultCode::PRC_VAL;
        result.value = arg;
    }
    return result;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

