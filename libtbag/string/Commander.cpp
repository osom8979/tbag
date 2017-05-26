/**
 * @file   Commander.cpp
 * @brief  Commander class implementation.
 * @author zer0
 * @date   2016-10-29
 */

#include <libtbag/string/Commander.hpp>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Commander::Commander() : _prefix(DEFAULT_PREFIX), _delimiter(DEFAULT_DELIMITER), _default(nullptr)
{
    // EMPTY.
}

Commander::Commander(std::string const & prefix, std::string const & delimiter)
        : _prefix(prefix), _delimiter(delimiter), _default(nullptr)
{
    // EMPTY.
}

Commander::Commander(Callback const & default_callback) :
        _prefix(DEFAULT_PREFIX),
        _delimiter(DEFAULT_DELIMITER),
        _default(default_callback)
{
    // EMPTY.
}

Commander::Commander(Commander const & obj)
{
    (*this) = obj;
}

Commander::Commander(Commander && obj)
{
    (*this) = std::move(obj);
}

Commander::~Commander()
{
    // EMPTY.
}

Commander & Commander::operator =(Commander const & obj)
{
    if (this != &obj) {
        _prefix    = obj._prefix;
        _delimiter = obj._delimiter;
        _default   = obj._default;
        _commands  = obj._commands;
    }
    return *this;
}

Commander & Commander::operator =(Commander && obj)
{
    if (this != &obj) {
        _prefix.swap(obj._prefix);
        _delimiter.swap(obj._delimiter);
        _default.swap(obj._default);
        _commands.swap(obj._commands);
    }
    return *this;
}

void Commander::clear()
{
    _prefix.clear();
    _delimiter.clear();
    _default = Callback();
    _commands.clear();
}

bool Commander::insert(std::string const & command, Callback const & callback)
{
    return _commands.insert(CommandPair(command, callback)).second;
}

Commander::ArgsVector Commander::parseArguments(Flags const & flags)
{
    std::size_t const SIZE = flags.size();
    std::vector<Arguments> result;
    for (std::size_t index = 0; index < SIZE; ++index) {
        result.push_back(Arguments(flags.at(index).key, flags.at(index).value));
    }
    return result;
}

Commander::ArgsVector Commander::parseArguments(std::string const & arguments,
                                                std::string const & prefix,
                                                std::string const & delimiter)
{
    return parseArguments(Flags(arguments, prefix, delimiter));
}

std::size_t Commander::request(ArgsVector const & args_vector)
{
    std::size_t const SIZE = args_vector.size();
    std::size_t count = 0;

    for (std::size_t index = 0; index < SIZE; ++index) {
        Arguments const & arguments = args_vector[index];
        bool const IS_CONSUMED = onRequest(index, SIZE, arguments);

        if (IS_CONSUMED) {
            continue;
        }

        auto itr = _commands.find(arguments.getName());
        if (itr == _commands.end()) {
            if (static_cast<bool>(_default)) {
                _default(arguments);
            }
            continue;
        }

        if (static_cast<bool>(itr->second)) {
            itr->second(arguments);
        }
        ++count;
    }

    return count;
}

std::size_t Commander::request(Flags const & flags)
{
    return request(parseArguments(flags));
}

std::size_t Commander::request(std::string const & arguments, std::string const & prefix, std::string const & delimiter)
{
    return request(parseArguments(arguments, prefix, delimiter));
}

std::size_t Commander::request(std::string const & arguments)
{
    return request(parseArguments(arguments, _prefix, _delimiter));
}

std::size_t Commander::request(int argc, char ** argv, std::string const & prefix, std::string const & delimiter, bool ignore_first)
{
    std::stringstream ss;
    for (int i = (ignore_first ? 1 : 0); i < argc; ++i) {
        ss << argv[i] << ' ';
    }
    return request(ss.str(), prefix, delimiter);
}

std::size_t Commander::request(int argc, char ** argv, bool ignore_first)
{
    return request(argc, argv, _prefix, _delimiter, ignore_first);
}

bool Commander::onRequest(std::size_t index, std::size_t size, Arguments const & arguments)
{
    return false;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

