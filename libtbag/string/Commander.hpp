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
#include <libtbag/Noncopyable.hpp>
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
class TBAG_API Commander : public Noncopyable
{
public:
    using Callback    = std::function<void(Arguments const &)>;
    using CommandMap  = std::map<std::string, Callback>;
    using CommandPair = CommandMap::value_type;

private:
    Callback   _default;
    CommandMap _commands;

public:
    Commander();
    Commander(Callback const & default_callback);
    Commander(Commander const & obj);
    Commander(Commander && obj);
    virtual ~Commander();

public:
    Commander & operator =(Commander const & obj);
    Commander & operator =(Commander && obj);

public:
    inline bool empty() const TBAG_NOEXCEPT
    { return _commands.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT
    { return _commands.size(); }

    inline void setDefaultCallback(Callback const & callback)
    { _default = callback; }

public:
    void clear();
    bool insert(std::string const & command, Callback const & callback);

public:
    std::vector<Arguments> parseArguments(std::string const & arguments);

public:
    void request(std::string const & arguments);
    void request(int argc, char ** argv);

public:
    virtual void onRequest(Arguments const & arguments);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_COMMANDER_HPP__

