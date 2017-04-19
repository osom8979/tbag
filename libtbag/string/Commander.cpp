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

Commander::Commander() : _default(nullptr)
{
    // EMPTY.
}

Commander::Commander(Callback const & default_callback) : _default(default_callback)
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
        _default = obj._default;
        _commands = obj._commands;
    }
    return *this;
}

Commander & Commander::operator =(Commander && obj)
{
    if (this != &obj) {
        _default.swap(obj._default);
        _commands.swap(obj._commands);
    }
    return *this;
}

void Commander::clear()
{
    _default = Callback();
    _commands.clear();
}

bool Commander::insert(std::string const & command, Callback const & callback)
{
    return _commands.insert(CommandPair(command, callback)).second;
}

std::vector<Arguments> Commander::parseArguments(std::string const & arguments)
{
    Flags       const FLAGS = Flags(arguments);
    std::size_t const SIZE  = FLAGS.size();

    std::vector<Arguments> result;
    for (std::size_t index = 0; index < SIZE; ++index) {
        result.push_back(Arguments(FLAGS.at(index).key, FLAGS.at(index).value));
    }
    return result;
}

void Commander::request(std::string const & arguments)
{
    for (auto & cursor : parseArguments(arguments)) {
        onRequest(cursor);
    }
}

void Commander::request(int argc, char ** argv)
{
    std::stringstream ss;
    for (int i = 0; i < argc; ++i) {
        ss << argv[i] << ' ';
    }
    request(ss.str());
}

void Commander::onRequest(Arguments const & arguments)
{
    auto find_itr = _commands.find(arguments.getName());
    if (find_itr != _commands.end() && static_cast<bool>(find_itr->second)) {
        find_itr->second(arguments);
    } else if (static_cast<bool>(_default)) {
        _default(arguments);
    }
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

