/**
 * @file   Commander.hpp
 * @brief  Commander class prototype.
 * @author zer0
 * @date   2016-10-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_COMMANDER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_COMMANDER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/Arguments.hpp>
#include <libtbag/string/Flags.hpp>

#include <string>
#include <vector>
#include <map>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * Commander class prototype.
 *
 * @author zer0
 * @date   2016-10-01
 */
class Commander : public Noncopyable
{
public:
    using Arguments  = libtbag::string::Arguments;
    using Flags      = libtbag::string::Flags;

    using Callback    = std::function<void(Arguments const &)>;
    using CommandMap  = std::map<std::string, Callback>;
    using CommandPair = typename CommandMap::value_type;

private:
    Callback   _default;
    CommandMap _commands;

public:
    Commander(Callback const & default_callback) : _default(default_callback)
    { /* EMPTY. */ }

public:
    Commander() = default;
    virtual ~Commander() = default;

    Commander(Commander const & obj) = default;
    Commander & operator =(Commander const & obj) = default;

#if defined(TBAG_HAS_DEFAULTED_FUNCTIONS) && !defined(TBAG_HAS_DEFAULTED_FUNCTIONS_BUT_NOT_MOVE_FUNCTION)
    Commander(Commander && obj) = default;
    Commander & operator =(Commander && obj) = default;
#endif

public:
    inline bool empty() const TBAG_NOEXCEPT
    { return _commands.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT
    { return _commands.size(); }

    inline void setDefaultCallback(Callback const & callback)
    { _default = callback; }

public:
    void clear()
    {
        _default = Callback();
        _commands.clear();
    }

    bool insert(std::string const & command, Callback const & callback)
    {
        return _commands.insert(CommandPair(command, callback)).second;
    }

public:
    std::vector<Arguments> parseArguments(std::string const & arguments)
    {
        Flags const FLAGS(arguments);
        std::size_t const SIZE = FLAGS.size();

        std::vector<Arguments> result;

        for (std::size_t index = 0; index < SIZE; ++index) {
            result.push_back(Arguments(FLAGS.at(index).key, FLAGS.at(index).value));
        }

        return result;
    }

public:
    void request(std::string const & arguments)
    {
        for (auto & cursor : parseArguments(arguments)) {
            onRequest(cursor);
        }
    }

public:
    virtual void onRequest(Arguments const & arguments)
    {
        auto find_itr = _commands.find(arguments.getName());
        if (find_itr != _commands.end() && static_cast<bool>(find_itr->second)) {
            find_itr->second(arguments);
        } else if (static_cast<bool>(_default)) {
            _default(arguments);
        }
    }
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_COMMANDER_HPP__

