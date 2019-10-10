/**
 * @file   Environments.cpp
 * @brief  Environments class implementation.
 * @author zer0
 * @date   2017-04-29
 */

#include <libtbag/string/Environments.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/log/Log.hpp>

#include <climits>
#include <cstdio>

#include <sstream>
#include <utility>

extern char ** environ;

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

TBAG_CONSTEXPR static char const * const _NO_PREFIX_STRING = "";

Environments::Environments()
{
    // EMPTY.
}

Environments::Environments(char ** envs)
{
    if (!pushEnvs(envs)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(char ** envs, std::string const & delimiter)
{
    if (!pushEnvs(envs, delimiter)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(std::string const & envs)
{
    if (!pushEnvs(envs)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(std::string const & envs, std::string const & delimiter)
{
    if (!pushEnvs(envs, delimiter)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(std::vector<std::string> const & envs, std::string const & delimiter)
{
    if (!pushEnvs(envs, delimiter)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(Flags const & flags) : _flags(flags)
{
    // EMPTY.
}

Environments::Environments(Flags && flags) TBAG_NOEXCEPT : _flags(std::move(flags))
{
    // EMPTY.
}

Environments::Environments(Environments const & obj) : _flags(obj._flags)
{
    // EMPTY.
}

Environments::Environments(Environments && obj) TBAG_NOEXCEPT : _flags(std::move(obj._flags))
{
    // EMPTY.
}

Environments::~Environments()
{
    // EMPTY.
}

Environments & Environments::operator =(Environments const & obj)
{
    if (this != &obj) {
        _flags = obj._flags;
    }
    return *this;
}

Environments & Environments::operator =(Environments && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _flags = std::move(obj._flags);
    }
    return *this;
}

Environments & Environments::operator =(Flags const & flags)
{
    if (&_flags != &flags) {
        _flags = flags;
    }
    return *this;
}

Environments & Environments::operator =(Flags && flags) TBAG_NOEXCEPT
{
    if (&_flags != &flags) {
        _flags = std::move(flags);
    }
    return *this;
}

bool Environments::pushSystemEnvs()
{
    return pushEnvs(environ);
}

bool Environments::pushEnvs(char ** envs)
{
    return pushEnvs(envs, DEFAULT_DELIMITER);
}

bool Environments::pushEnvs(char ** envs, std::string const & delimiter)
{
    auto const size = getEnvsSize(envs);
    if (size > static_cast<std::size_t>(INT32_MAX)) {
        tDLogW("Environments::parse() envs size({}) is too big.", size);
    }
    return _flags.parse(static_cast<int>(size), envs, _NO_PREFIX_STRING, delimiter);
}

bool Environments::pushEnvs(std::string const & envs)
{
    return pushEnvs(envs, DEFAULT_DELIMITER);
}

bool Environments::pushEnvs(std::string const & envs, std::string const & delimiter)
{
    return _flags.parse(envs, _NO_PREFIX_STRING, delimiter);
}

bool Environments::pushEnvs(std::vector<std::string> const & envs, std::string const & delimiter)
{
    return _flags.parse(envs, _NO_PREFIX_STRING, delimiter);
}

void Environments::readFromResource(Resource const & res)
{
    for (auto const & cursor : res.map) {
        push(cursor.first, cursor.second);
    }
}

void Environments::readFromResourceXmlString(std::string const & xml)
{
    readFromResource(Resource::createFromXmlString(xml));
}

void Environments::readFromResourceXmlFile(std::string const & path)
{
    readFromResource(Resource::createFromXmlFile(path));
}

void Environments::saveToResource(Resource & res)
{
    auto const size = _flags.size();
    for (std::size_t i = 0; i < size; ++i) {
        res.set(_flags.at(i).key, _flags.at(i).value);
    }
}

bool Environments::saveToResourceXmlString(std::string & xml)
{
    Resource res;
    saveToResource(res);
    return res.saveToXmlString(xml);
}

bool Environments::saveToResourceXmlFile(std::string const & path)
{
    libtbag::dom::xml::Resource res;
    std::size_t const size = _flags.size();
    for (std::size_t i = 0; i < size; ++i) {
        auto const & flag = _flags.at(i);
        res.set(flag.key, flag.value);
    }
    return res.saveToXmlFile(path);
}

enum convert_state_t
{
    convert_state_normal,
    convert_state_enable_prefix0,
    convert_state_enable_variable,
};

std::string Environments::convert(std::string const & source) const
{
    auto const size = source.size();

    convert_state_t state = convert_state_normal;
    char buffer[MAX_VARIABLE_BUFFER_LENGTH] = {0,};
    std::size_t buffer_size = 0;
    std::stringstream ss;
    char c = 0;

    for (std::size_t i = 0; i < size; ++i) {
        c = source[i];

        switch (state) {
        case convert_state_normal:
            if (c == DEFAULT_PREFIX0) {
                state = convert_state_enable_prefix0;
                buffer[0] = DEFAULT_PREFIX0;
                buffer_size = 1;
            } else {
                ss << c;
            }
            break;

        case convert_state_enable_prefix0:
            if (c == DEFAULT_PREFIX1) {
                state = convert_state_enable_variable;
                buffer[1] = DEFAULT_PREFIX1;
                buffer_size = 2;
            } else {
                state = convert_state_normal;
                ss << DEFAULT_PREFIX0 << c;
            }
            break;

        case convert_state_enable_variable:
            if (c == DEFAULT_SUFFIX) {
                state = convert_state_normal;
                if (buffer_size <= 2) {
                    // Variable name is "${}". Discard this case.
                } else {
                    ss << parameterExpansion(std::string(buffer+2, buffer+buffer_size));
                }
                buffer_size = 0;
            } else {
                buffer[buffer_size] = c;
                buffer_size++;
            }
            break;

        default:
            break;
        }
    }

    if (state != convert_state_normal && buffer_size >= 1) {
        // Flush remaining buffers.
        ss << std::string(buffer, buffer+buffer_size);
    }
    return ss.str();
}

enum expansion_state_t
{
    expansion_state_enable_variable,
    expansion_state_enable_colon,
    expansion_state_enable_colon_minus,
    expansion_state_enable_colon_equals,
    expansion_state_enable_colon_question,
    expansion_state_enable_colon_plus,
    expansion_state_enable_colon_offset,
    expansion_state_enable_colon_offset_colon,
    expansion_state_enable_sharp,
    expansion_state_enable_sharp_done,
    expansion_state_enable_sharp_sharp,
    expansion_state_enable_percent,
    expansion_state_enable_percent_done,
    expansion_state_enable_percent_percent,
    expansion_state_enable_slash,
    expansion_state_enable_slash_slash,
    expansion_state_enable_carat,
    expansion_state_enable_carat_done,
    expansion_state_enable_carat_carat,
    expansion_state_enable_comma,
    expansion_state_enable_comma_done,
    expansion_state_enable_comma_comma,
    expansion_state_enable_at,
};

enum expansion_format_t
{
    _ef_unknown,
    _ef_normal,                               //   var                // ${parameter}
    _ef_sharp_parameter,                      // # var                // ${#parameter}
    _ef_bang_prefix_asterisk,                 // ! var *              // ${!prefix*}
    _ef_bang_prefix_at,                       // ! var @              // ${!prefix@}
    _ef_bang_name_at,                         // ! var [@]            // ${!name[@]}
    _ef_bang_name_asterisk,                   // ! var [*]            // ${!name[*]}
    _ef_parameter_colon_minus_word,           //   var : - word       // ${parameter:-word}
    _ef_parameter_colon_equals_word,          //   var : = word       // ${parameter:=word}
    _ef_parameter_colon_question_word,        //   var : ? word       // ${parameter:?word}
    _ef_parameter_colon_plus_word,            //   var : + word       // ${parameter:+word}
    _ef_parameter_colon_offset,               //   var : off          // ${parameter:offset}
    _ef_parameter_colon_offset_colon_length,  //   var : off : len    // ${parameter:offset:length}
    _ef_parameter_sharp_word,                 //   var #   word       // ${parameter#word}
    _ef_parameter_sharp_sharp_word,           //   var # # word       // ${parameter##word}
    _ef_parameter_percent_word,               //   var %   word       // ${parameter%word}
    _ef_parameter_percent_percent_word,       //   var % % word       // ${parameter%%word}
    _ef_parameter_slash_pattern_slash_string, //   var /   patt / sub // ${parameter/pattern/string}
    _ef_parameter_carat_pattern,              //   var ^   patt       // ${parameter^pattern}
    _ef_parameter_carat_carat_pattern,        //   var ^ ^ patt       // ${parameter^^pattern}
    _ef_parameter_comma_pattern,              //   var ,   patt       // ${parameter,pattern}
    _ef_parameter_comma_comma_pattern,        //   var , , patt       // ${parameter,,pattern}
    _ef_parameter_at_operator_Q,              //   var @ Q            // ${parameter@Q}
    _ef_parameter_at_operator_E,              //   var @ E            // ${parameter@E}
    _ef_parameter_at_operator_P,              //   var @ P            // ${parameter@P}
    _ef_parameter_at_operator_A,              //   var @ A            // ${parameter@A}
    _ef_parameter_at_operator_a,              //   var @ a            // ${parameter@a}
};

Err Environments::parameterExpansion(std::string const & variable, Flags const & flags, std::string & result)
{
    if (variable.empty()) {
        return E_ILLARGS;
    }

    auto const size = variable.size();
    assert(size >= 1);

    if (variable[0] == '#') {
        result = libtbag::string::toString(flags.opt(variable.substr(1)).size());
        return E_SUCCESS;
    }

    if (variable[0] == '!') {
        // TODO: implement this expansion.
        return E_ENOSYS;
    }

    expansion_state_t state = expansion_state_enable_variable;
    expansion_format_t format = _ef_unknown;
    std::stringstream name_ss;
    std::stringstream word_ss;
    std::stringstream patt_ss;
    std::stringstream sub_ss; // substitution
    std::stringstream offset_ss;
    std::stringstream length_ss;
    char c = 0;

    for (std::size_t i = 0; i < size; ++i) {
        c = variable[i];
        switch (state) {
        case expansion_state_enable_variable:
            switch (c) {
            case ':': state = expansion_state_enable_colon;   break;
            case '#': state = expansion_state_enable_sharp;   break;
            case '%': state = expansion_state_enable_percent; break;
            case '/': state = expansion_state_enable_slash;   break;
            case '^': state = expansion_state_enable_carat;   break;
            case ',': state = expansion_state_enable_comma;   break;
            case '@': state = expansion_state_enable_at;      break;
            default:
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                name_ss << c;
                break;
            }
            break;

        case expansion_state_enable_colon:
            switch (c) {
            case '-': state = expansion_state_enable_colon_minus;    break;
            case '=': state = expansion_state_enable_colon_equals;   break;
            case '?': state = expansion_state_enable_colon_question; break;
            case '+': state = expansion_state_enable_colon_plus;     break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                state = expansion_state_enable_colon_offset;
                offset_ss << c;
                break;
            default:
                return E_PARSING;
            }
            break;

        case expansion_state_enable_colon_minus:
            TBAG_FALLTHROUGH
        case expansion_state_enable_colon_equals:
            TBAG_FALLTHROUGH
        case expansion_state_enable_colon_question:
            TBAG_FALLTHROUGH
        case expansion_state_enable_colon_plus:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            word_ss << c;
            break;

        case expansion_state_enable_colon_offset:
            switch (c) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                offset_ss << c;
                break;
            case ':':
                state = expansion_state_enable_colon_offset_colon;
                break;
            default:
                return E_PARSING;
            }
            break;

        case expansion_state_enable_colon_offset_colon:
            switch (c) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                length_ss << c;
                break;
            default:
                return E_PARSING;
            }
            break;

        case expansion_state_enable_sharp:
            if (c == '#') {
                state = expansion_state_enable_sharp_sharp;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                state = expansion_state_enable_sharp_done;
                word_ss << c;
            }
            break;

        case expansion_state_enable_sharp_done:
            TBAG_FALLTHROUGH
        case expansion_state_enable_sharp_sharp:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            word_ss << c;
            break;

        case expansion_state_enable_percent:
            if (c == '%') {
                state = expansion_state_enable_percent_percent;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                state = expansion_state_enable_percent_done;
                word_ss << c;
            }
            break;

        case expansion_state_enable_percent_done:
            TBAG_FALLTHROUGH
        case expansion_state_enable_percent_percent:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            word_ss << c;
            break;

        case expansion_state_enable_slash:
            if (c == '/') {
                state = expansion_state_enable_slash_slash;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                patt_ss << c;
            }
            break;

        case expansion_state_enable_slash_slash:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            sub_ss << c;
            break;

        case expansion_state_enable_carat:
            if (c == '^') {
                state = expansion_state_enable_carat_carat;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                state = expansion_state_enable_carat_done;
                patt_ss << c;
            }
            break;

        case expansion_state_enable_carat_done:
            TBAG_FALLTHROUGH
        case expansion_state_enable_carat_carat:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            patt_ss << c;
            break;

        case expansion_state_enable_comma:
            if (c == ',') {
                state = expansion_state_enable_carat_carat;
            } else {
                if (!isVariableName(c)) {
                    return E_PARSING;
                }
                state = expansion_state_enable_comma_done;
                patt_ss << c;
            }
            break;

        case expansion_state_enable_comma_done:
            TBAG_FALLTHROUGH
        case expansion_state_enable_comma_comma:
            if (!isVariableName(c)) {
                return E_PARSING;
            }
            patt_ss << c;
            break;

        case expansion_state_enable_at:
            switch (c) {
            case 'Q': format = _ef_parameter_at_operator_Q; break;
            case 'E': format = _ef_parameter_at_operator_E; break;
            case 'P': format = _ef_parameter_at_operator_P; break;
            case 'A': format = _ef_parameter_at_operator_A; break;
            case 'a': format = _ef_parameter_at_operator_a; break;
            default:
                return E_PARSING;
            }
            break;
        }

        if (format != _ef_unknown) {
            break;
        }
    }

    // Select result format.
    if (format == _ef_unknown) {
        switch (state) {
        case expansion_state_enable_variable:
            format = _ef_normal;
            break;

        case expansion_state_enable_colon:
            return E_ILLSTATE;
        case expansion_state_enable_colon_minus:
            format = _ef_parameter_colon_minus_word;
            break;
        case expansion_state_enable_colon_equals:
            format = _ef_parameter_colon_equals_word;
            break;
        case expansion_state_enable_colon_question:
            format = _ef_parameter_colon_question_word;
            break;
        case expansion_state_enable_colon_plus:
            format = _ef_parameter_colon_plus_word;
            break;

        case expansion_state_enable_colon_offset:
            format = _ef_parameter_colon_offset;
            break;
        case expansion_state_enable_colon_offset_colon:
            format = _ef_parameter_colon_offset_colon_length;
            break;

        case expansion_state_enable_sharp:
            return E_ILLSTATE;
        case expansion_state_enable_sharp_done:
            format = _ef_parameter_sharp_word;
            break;
        case expansion_state_enable_sharp_sharp:
            format = _ef_parameter_sharp_sharp_word;
            break;

        case expansion_state_enable_percent:
            return E_ILLSTATE;
        case expansion_state_enable_percent_done:
            format = _ef_parameter_percent_word;
            break;
        case expansion_state_enable_percent_percent:
            format = _ef_parameter_percent_percent_word;
            break;

        case expansion_state_enable_slash:
            return E_ILLSTATE;
        case expansion_state_enable_slash_slash:
            format = _ef_parameter_slash_pattern_slash_string;
            break;

        case expansion_state_enable_carat:
            return E_ILLSTATE;
        case expansion_state_enable_carat_done:
            format = _ef_parameter_carat_pattern;
            break;
        case expansion_state_enable_carat_carat:
            format = _ef_parameter_carat_carat_pattern;
            break;

        case expansion_state_enable_comma:
            return E_ILLSTATE;
        case expansion_state_enable_comma_done:
            format = _ef_parameter_comma_pattern;
            break;
        case expansion_state_enable_comma_comma:
            format = _ef_parameter_comma_comma_pattern;
            break;

        case expansion_state_enable_at:
            return E_ILLSTATE;
        default:
            return E_INACCESSIBLE_BLOCK;
        }
    }

    assert(format != _ef_unknown);
    assert(format != _ef_sharp_parameter);
    assert(format != _ef_bang_prefix_asterisk);
    assert(format != _ef_bang_prefix_at);
    assert(format != _ef_bang_name_at);
    assert(format != _ef_bang_name_asterisk);

    auto const name = name_ss.str();
    auto const word = word_ss.str();
    auto const patt = patt_ss.str();
    auto const sub = sub_ss.str();
    auto const offset = offset_ss.str();
    auto const length = length_ss.str();

    switch (format) {
    case _ef_normal:
        result = flags.opt(name);
        return E_SUCCESS;

    case _ef_parameter_colon_minus_word:
        // If parameter is unset or null, the expansion of word is substituted.
        // Otherwise, the value of parameter is substituted.
        if (flags.existsByKey(name)) {
            auto const value = flags.opt(name);
            if (value.empty()) {
                result = word;
            } else {
                result = value;
            }
        } else {
            result = word;
        }
        return E_SUCCESS;

    case _ef_parameter_colon_equals_word:
        return E_ENOSYS;
    case _ef_parameter_colon_question_word:
        return E_ENOSYS;
    case _ef_parameter_colon_plus_word:
        return E_ENOSYS;
    case _ef_parameter_colon_offset:
        return E_ENOSYS;
    case _ef_parameter_colon_offset_colon_length:
        return E_ENOSYS;
    case _ef_parameter_sharp_word:
        return E_ENOSYS;
    case _ef_parameter_sharp_sharp_word:
        return E_ENOSYS;
    case _ef_parameter_percent_word:
        return E_ENOSYS;
    case _ef_parameter_percent_percent_word:
        return E_ENOSYS;
    case _ef_parameter_slash_pattern_slash_string:
        return E_ENOSYS;
    case _ef_parameter_carat_pattern:
        return E_ENOSYS;
    case _ef_parameter_carat_carat_pattern:
        return E_ENOSYS;
    case _ef_parameter_comma_pattern:
        return E_ENOSYS;
    case _ef_parameter_comma_comma_pattern:
        return E_ENOSYS;
    case _ef_parameter_at_operator_Q:
        return E_ENOSYS;
    case _ef_parameter_at_operator_E:
        return E_ENOSYS;
    case _ef_parameter_at_operator_P:
        return E_ENOSYS;
    case _ef_parameter_at_operator_A:
        return E_ENOSYS;
    case _ef_parameter_at_operator_a:
        return E_ENOSYS;
    default:
        return E_INACCESSIBLE_BLOCK;
    }
}

std::string Environments::parameterExpansion(std::string const & variable) const
{
    std::string result;
    if (isFailure(parameterExpansion(variable, _flags, result))) {
        return {};
    }
    return result;
}

std::vector<std::string> Environments::toStrings(std::string const & delimiter) const
{
    std::vector<std::string> result;
    auto const size = _flags.size();
    for (std::size_t i = 0; i < size; ++i) {
        result.emplace_back(_flags.at(i).key + delimiter + _flags.at(i).value);
    }
    return result;
}

std::vector<std::string> Environments::toStrings() const
{
    return toStrings(DEFAULT_DELIMITER);
}

std::size_t Environments::getEnvsSize(char ** envs)
{
    if ((*envs) == nullptr) {
        return 0U;
    }
    std::size_t count = 0;
    do {
        ++count;
    } while (*(envs + count) != nullptr);
    return count;
}

Environments Environments::createDefaultEnvironments(bool with_system)
{
    using namespace libtbag::filesystem;
    Environments envs;
    if (with_system) {
        envs.pushSystemEnvs();
    }
    envs.push(EXE_PATH, Path::getExePath());
    envs.push(EXE_NAME, Path::getExePath().getName());
    envs.push(EXE_DIR, Path::getExeDir());
    envs.push(WORK_DIR, Path::getWorkDir());
    envs.push(HOME_DIR, Path::getHomeDir());
    return envs;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

