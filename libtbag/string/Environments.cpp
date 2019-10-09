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
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Environments::Environments()
{
    // EMPTY.
}

Environments::Environments(char ** envs)
{
    if (!parse(envs)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(char ** envs, std::string const & delimiter)
{
    if (!parse(envs, delimiter)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(std::string const & envs)
{
    if (!parse(envs)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(std::string const & envs, std::string const & delimiter)
{
    if (!parse(envs, delimiter)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(std::vector<std::string> const & envs, std::string const & delimiter)
{
    if (!parse(envs, delimiter)) {
        throw std::bad_alloc();
    }
}

Environments::Environments(Flags const & flags)
{
    _flags = flags;
}

Environments::Environments(Flags && flags)
{
    _flags = std::move(flags);
}

Environments::Environments(Environments const & obj)
{
    (*this) = obj;
}

Environments::Environments(Environments && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

Environments::~Environments()
{
    _flags.clear();
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
        assign(flags);
    }
    return *this;
}

Environments & Environments::operator =(Flags && flags) TBAG_NOEXCEPT
{
    if (&_flags != &flags) {
        assign(std::move(flags));
    }
    return *this;
}

std::vector<std::string> Environments::keys() const
{
    return _flags.keys();
}

bool Environments::get(std::string const & key, std::string & val) const
{
    return _flags.get(key, val);
}

void Environments::set(std::string const & key, std::string const & val)
{
    _flags.set(key, val);
}

std::string Environments::opt(std::string const & key) const
{
    return opt(key, std::string());
}

std::string Environments::opt(std::string const & key, std::string const & default_val) const
{
    std::string result;
    if (get(key, result)) {
        return result;
    }
    return default_val;
}

void Environments::assign(Flags const & flags)
{
    _flags = flags;
}

void Environments::assign(Flags && flags) TBAG_NOEXCEPT
{
    _flags = std::move(flags);
}

void Environments::swap(Environments & obj)
{
    _flags.swap(obj._flags);
}

bool Environments::parse(char ** envs)
{
    return parse(envs, DEFAULT_DELIMITER);
}

bool Environments::parse(char ** envs, std::string const & delimiter)
{
    std::size_t const SIZE = getEnvsSize(envs);
    if (SIZE > static_cast<std::size_t>(INT32_MAX)) {
        tDLogW("Environments::parse() envs size({}) is too big.", SIZE);
    }
    return _flags.parse(static_cast<int>(SIZE), envs, ENVIRONMENTS_FLAGS_PREFIX, delimiter);
}

bool Environments::parse(std::string const & envs)
{
    return parse(envs, DEFAULT_DELIMITER);
}

bool Environments::parse(std::string const & envs, std::string const & delimiter)
{
    return _flags.parse(envs, ENVIRONMENTS_FLAGS_PREFIX, delimiter);
}

bool Environments::parse(std::vector<std::string> const & envs, std::string const & delimiter)
{
    return _flags.parse(envs, ENVIRONMENTS_FLAGS_PREFIX, delimiter);
}

void Environments::readResourceXmlString(std::string const & xml)
{
    using namespace libtbag::dom::xml;
    auto const XML_RESOURCE = Resource::createFromXmlString(xml);
    for (auto & cursor : XML_RESOURCE.map()) {
        push(cursor.first, cursor.second);
    }
}

void Environments::readResourceXmlFile(std::string const & path)
{
    using namespace libtbag::dom::xml;
    auto const XML_RESOURCE = Resource::createFromXmlFile(path);
    for (auto & cursor : XML_RESOURCE.map()) {
        push(cursor.first, cursor.second);
    }
}

bool Environments::saveResourceXmlFile(std::string const & path)
{
    libtbag::dom::xml::Resource res;
    std::size_t const SIZE = _flags.size();
    for (std::size_t i = 0; i < SIZE; ++i) {
        auto const & flag = _flags.at(i);
        res.set(flag.key, flag.value);
    }
    return res.saveFile(path);
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
    std::size_t const SIZE = _flags.size();
    for (std::size_t i = 0; i < SIZE; ++i) {
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
    std::size_t const SIZE = _flags.size();
    for (std::size_t i = 0; i < SIZE; ++i) {
        auto & flag = _flags.at(i);
        auto const key_exists = !flag.key.empty();
        auto const val_exists = !flag.value.empty();
        if (key_exists && val_exists) {
            result.push_back(flag.key + delimiter + flag.value);
        } else if (!key_exists && val_exists) {
            result.push_back(flag.value);
        } else if (key_exists && !val_exists) {
            result.push_back(flag.key);
        }
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

Environments Environments::createDefaultEnvironments()
{
    using namespace libtbag::filesystem;
    Environments envs;
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

