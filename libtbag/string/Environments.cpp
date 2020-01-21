/**
 * @file   Environments.cpp
 * @brief  Environments class implementation.
 * @author zer0
 * @date   2017-04-29
 */

#include <libtbag/string/Environments.hpp>
#include <libtbag/string/ParameterExpansion.hpp>
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

std::string Environments::parameterExpansion(std::string const & variable) const
{
    std::string result;
    if (isFailure(libtbag::string::parameterExpansion(variable, _flags, result))) {
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

std::string convert(std::string const & text,
                    std::map<std::string, std::string> const & dict,
                    bool with_system)
{
    auto envs = Environments::createDefaultEnvironments(with_system);
    for (auto const & itr : dict) {
        envs.push(itr.first, itr.second);
    }
    return envs.convert(text);
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

