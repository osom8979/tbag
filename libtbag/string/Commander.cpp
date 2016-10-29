/**
 * @file   Commander.cpp
 * @brief  Commander class implementation.
 * @author zer0
 * @date   2016-10-29
 */

#include <libtbag/string/Commander.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

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

