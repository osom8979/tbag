/**
 * @file   Environments.cpp
 * @brief  Environments class implementation.
 * @author zer0
 * @date   2017-04-29
 */

#include <libtbag/string/Environments.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <climits>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

TBAG_CONSTEXPR static char const * const DEFAULT_ENVIRONMENTS_FLAGS_PREFIX = "";

Environments::Environments()
{
    // EMPTY.
}

Environments::Environments(char ** envs)
{
    parse(envs);
}

Environments::Environments(char ** envs, std::string const & delimiter)
{
    parse(envs, delimiter);
}

Environments::Environments(std::string const & envs)
{
    parse(envs);
}

Environments::Environments(std::string const & envs, std::string const & delimiter)
{
    parse(envs, delimiter);
}

Environments::Environments(StringVector const & envs, std::string const & delimiter)
{
    parse(envs, delimiter);
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

Environments::Environments(Environments && obj)
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

Environments & Environments::operator =(Environments && obj)
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

Environments & Environments::operator =(Flags && flags)
{
    if (&_flags != &flags) {
        assign(std::move(flags));
    }
    return *this;
}

void Environments::assign(Flags const & flags)
{
    _flags = flags;
}

void Environments::assign(Flags && flags)
{
    _flags = std::move(flags);
}

bool Environments::parse(char ** envs)
{
    return parse(envs, DEFAULT_DELIMITER);
}

bool Environments::parse(char ** envs, std::string const & delimiter)
{
    std::size_t const SIZE = getEnvsSize(envs);
    if (SIZE > static_cast<std::size_t>(INT32_MAX)) {
        __tbag_debug("[WARNING] envs size({}) is too big.", SIZE);
    }
    return _flags.parse(static_cast<int>(SIZE), envs, DEFAULT_ENVIRONMENTS_FLAGS_PREFIX, delimiter);
}

bool Environments::parse(std::string const & envs)
{
    return parse(envs, DEFAULT_DELIMITER);
}

bool Environments::parse(std::string const & envs, std::string const & delimiter)
{
    return _flags.parse(envs, DEFAULT_ENVIRONMENTS_FLAGS_PREFIX, delimiter);
}

bool Environments::parse(StringVector const & envs, std::string const & delimiter)
{
    return _flags.parse(envs, DEFAULT_ENVIRONMENTS_FLAGS_PREFIX, delimiter);
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

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

