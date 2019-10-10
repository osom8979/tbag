/**
 * @file   Environments.cpp
 * @brief  Environments class implementation.
 * @author zer0
 * @date   2017-04-29
 */

#include <libtbag/string/Environments.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/log/Log.hpp>

#include <climits>
#include <cstdio>
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

std::string Environments::convert(std::string const & source) const
{
    return convert(source, DEFAULT_REGEX_PREFIX, DEFAULT_REGEX_SUFFIX);
}

std::string Environments::convert(std::string const & source,
                                  std::string const & regex_prefix,
                                  std::string const & regex_suffix) const
{
    std::string temp = source;
    std::size_t const size = _flags.size();
    for (std::size_t i = 0; i < size; ++i) {
        auto & flag = _flags.at(i);
        if (flag.key.empty() == false) {
            temp = string::replaceRegex(temp, (regex_prefix + flag.key + regex_suffix), flag.value);
        }
    }
    return temp;
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
    envs.push(EXE_DIR , Path::getExeDir());
    envs.push(WORK_DIR, Path::getWorkDir());
    envs.push(HOME_DIR, Path::getHomeDir());
    return envs;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

