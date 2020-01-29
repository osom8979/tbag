/**
 * @file   Arguments.cpp
 * @brief  Arguments class implementation.
 * @author zer0
 * @date   2016-10-29
 */

#include <libtbag/string/Arguments.hpp>
#include <sstream>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Arguments::Arguments()
{
    // EMPTY.
}

Arguments::Arguments(std::string const & arguments, std::string const & delimiter)
{
    if (!parse(arguments, delimiter)) {
        throw std::bad_alloc();
    }
}

Arguments::Arguments(Arguments const & obj) : _args(obj._args)
{
    // EMPTY.
}

Arguments::Arguments(Arguments && obj) TBAG_NOEXCEPT : _args(std::move(obj._args))
{
    // EMPTY.
}

Arguments::~Arguments()
{
    // EMPTY.
}

Arguments & Arguments::operator =(Arguments const & obj)
{
    if (this != &obj) {
        _args = obj._args;
    }
    return *this;
}

Arguments & Arguments::operator =(Arguments && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _args = std::move(obj._args);
    }
    return *this;
}

void Arguments::swap(Arguments & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _args.swap(obj._args);
    }
}

void Arguments::insert(std::size_t index, std::string const & argument)
{
    _args.insert(_args.begin() + index, argument);
}

bool Arguments::parse(std::string const & arguments, std::string const & delimiter)
{
    for (auto const & cursor : libtbag::string::splitTokens(arguments, delimiter)) {
        push_back(cursor);
    }
    return true;
}

std::string Arguments::toString(std::string const & delimiter) const
{
    if (_args.empty()) {
        return std::string();
    }

    auto const size = _args.size();
    if (size == 1) {
        return _args[0];
    }

    assert(size >= 1u);
    std::stringstream ss;
    ss << _args[0];
    for (auto index = 1u; index < size; ++index) {
        ss << delimiter + _args[index];
    }
    return ss.str();
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

