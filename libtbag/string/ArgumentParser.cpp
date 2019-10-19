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

#include <cassert>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <queue>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

std::string ArgumentParser::DefaultFormatter::print(InitParams const & init, Params const & params)
{
    return {};
}

// --------------
// ArgumentParser
// --------------

using Param = ArgumentParser::Param;
using Params = ArgumentParser::Params;
using ErrArgumentResult = ArgumentParser::ErrArgumentResult;
using ParseResult = ArgumentParser::ParseResult;
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
// clang-format off

char const * ArgumentParser::getActionTypeName(ActionType type) TBAG_NOEXCEPT
{
    switch (type) {
    case ActionType::AT_STORE:
        return ACTION_TYPE_STORE;
    case ActionType::AT_STORE_CONST:
        return ACTION_TYPE_STORE_CONST;
    default:
        return "";
    }
}

ActionType ArgumentParser::getActionType(std::string const & name)
{
    if (name == ACTION_TYPE_STORE) {
        return ActionType::AT_STORE;
    } else if (name == ACTION_TYPE_STORE_CONST) {
        return ActionType::AT_STORE_CONST;
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

Params::iterator ArgumentParser::findParameter(std::string const & name)
{
    return std::find_if(_params.begin(), _params.end(), [&name](Params::const_reference p) -> bool {
        return std::find(p.names.begin(), p.names.end(), name) != p.names.cend();
    });
}

Params::const_iterator ArgumentParser::findConstantParameter(std::string const & name) const
{
    return std::find_if(_params.cbegin(), _params.cend(), [&name](Params::const_reference p) -> bool {
        return std::find(p.names.cbegin(), p.names.cend(), name) != p.names.cend();
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
        for (auto const & name : arg.names) {
            auto const updated_name = removePrefix(name);
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
            for (auto const & name : arg.names) {
                key_map[name] = arg.dest;
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
                result.optional[arg.dest] = arg.default_value;
            } else {
                result.positional[arg.dest] = arg.default_value;
            }
        }
    }

    argument_required_type req_type = argument_required_type_none;
    bool stop_parsing = false;

    auto const size = argv.size();
    assert(size >= 1u);

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
        case ParseResultCode::PRC_STDIN:
            result.optional[std::string(1u, _init.prefix)] = "true";
            break;
        case ParseResultCode::PRC_STOP_PARSING:
            stop_parsing = true;
            break;

        case ParseResultCode::PRC_KEY_VAL:
            if (req_type != argument_required_type_none) {
                return E_PARSING;
            }
            result.optional[key_map[parse_result.key]] = parse_result.value;
            break;

        case ParseResultCode::PRC_KEY:
            if (req_type == argument_required_type_none) {
                auto const itr = findConstantParameter(parse_result.key);
                if (itr != _params.cend()) {
                    switch (itr->action) {
                    case ActionType::AT_STORE:
                        selected_key = parse_result.key;
                        req_type = argument_required_type_wait_optional_value;
                        break;
                    case ActionType::AT_STORE_CONST:
                        result.optional[key_map[parse_result.key]] = itr->const_value;
                        break;
                    default:
                        return E_INACCESSIBLE_BLOCK;
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
                result.optional[key_map[selected_key]] = parse_result.value;
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
        auto const name = positional_names.front();
        auto const itr = findConstantParameter(name);
        assert(itr != _params.cend());
        if (itr->default_value.empty()) {
            return E_ILLARGS;
        }
        result.positional[key_map[name]] = itr->default_value;
        positional_names.pop();
    }

    if (req_type != argument_required_type_none) {
        return E_ILLSTATE;
    }

    return { E_SUCCESS, result };
}

ParseResult ArgumentParser::parseSingleArgument(std::string const & arg) const
{
    using namespace libtbag::filesystem;
    auto const arg_size = arg.size();
    if (arg_size == 0u) {
        return { ParseResultCode::PRC_SKIP };
    }

    if (arg_size == 1u) {
        if (arg[0] == _init.prefix) {
            return { ParseResultCode::PRC_STDIN };
        } else {
            return { ParseResultCode::PRC_VAL, {}, arg };
        }
    }

    if (arg_size == 2u) {
        if (arg[0] == _init.prefix) {
            if (arg[1] == _init.prefix) {
                return { ParseResultCode::PRC_STOP_PARSING };
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

