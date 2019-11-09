/**
 * @file   Commander.hpp
 * @brief  Commander class prototype.
 * @author zer0
 * @date   2019-11-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_COMMANDER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_COMMANDER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ErrPair.hpp>

#include <string>
#include <unordered_map>
#include <functional>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace functional {

/**
 * Commander class prototype.
 *
 * @author zer0
 * @date   2019-11-09
 */
template <typename KeyT, typename FuncT>
class TBAG_API Commander
{
public:
    using Callback = std::function<FuncT>;
    using ResultType = typename Callback::result_type;
    using Commands = std::unordered_map<KeyT, Callback>;
    using KeyType = typename Commands::key_type;

private:
    Commands _commands;

public:
    Commander() TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<Commands>::value)
    {
        // EMPTY.
    }

    Commander(Commander const & obj)
            TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_copy_constructible<Commands>::value)
            : _commands(obj._commands)
    {
        // EMPTY. */
    }

    Commander(Commander && obj)
            TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_move_constructible<Commands>::value)
            : _commands(std::move(obj._commands))
    {
        // EMPTY.
    }

    ~Commander()
    {
        // EMPTY.
    }

public:
    Commander & operator =(Commander const & obj)
    {
        if (this != &obj) {
            _commands = obj._commands;
        }
        return *this;
    }

    Commander & operator =(Commander && obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _commands = std::move(obj._commands);
        }
        return *this;
    }

    void swap(Commander & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _commands.swap(obj._commands);
        }
    }

    inline friend void swap(Commander & lh, Commander & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    inline Commands       * get()       TBAG_NOEXCEPT { return &_commands; }
    inline Commands const * get() const TBAG_NOEXCEPT { return &_commands; }

    inline Commands       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline Commands const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline Commands       & commands()       TBAG_NOEXCEPT { return _commands; }
    inline Commands const & commands() const TBAG_NOEXCEPT { return _commands; }

    inline Commands       & operator *()       TBAG_NOEXCEPT { return commands(); }
    inline Commands const & operator *() const TBAG_NOEXCEPT { return commands(); }

public:
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_commands.empty())
    { return _commands.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_commands.size())
    { return _commands.size(); }

    void clear()
    {
        _commands.clear();
    }

    bool exists(KeyType const & key) const
    {
        return _commands.find(key) != _commands.end();
    }

public:
    template <typename ... ArgTypes>
    ErrPair<ResultType> operator ()(KeyType const & key, ArgTypes ... arg) const
    {
        auto itr = _commands.find(key);
        if (itr == _commands.end()) {
            return E_NFOUND;
        }
        return { E_SUCCESS, itr->second(std::forward<ArgTypes>(arg) ...) };
    }
};

} // namespace functional

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_COMMANDER_HPP__

