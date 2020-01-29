/**
 * @file   Arguments.hpp
 * @brief  Arguments class prototype.
 * @author zer0
 * @date   2016-09-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * Arguments class prototype.
 * 
 * Example:
 * @code{.cpp}
 * Arguments args;
 * args.parse("test,8,123.456,9999");
 * @endcode
 *
 * @author zer0
 * @date   2016-09-23
 */
class TBAG_API Arguments
{
public:
    TBAG_CONSTEXPR static char const * const DELIMITER = ",";

private:
    std::vector<std::string> _args;

public:
    Arguments();
    Arguments(std::string const & arguments, std::string const & delimiter = DELIMITER);
    Arguments(Arguments const & obj);
    Arguments(Arguments && obj) TBAG_NOEXCEPT;
    ~Arguments();

public:
    Arguments & operator =(Arguments const & obj);
    Arguments & operator =(Arguments && obj) TBAG_NOEXCEPT;

public:
    void swap(Arguments & obj) TBAG_NOEXCEPT;
    friend void swap(Arguments & lh, Arguments & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    inline std::vector<std::string>       & args()       TBAG_NOEXCEPT { return _args; }
    inline std::vector<std::string> const & args() const TBAG_NOEXCEPT { return _args; }

public:
    inline void clear() TBAG_NOEXCEPT_SP_OP(_args.clear())
    { _args.clear(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_args.size())
    { return _args.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_args.empty())
    { return _args.empty(); }

public:
    inline std::string & at(std::size_t index) TBAG_NOEXCEPT_SP_OP(_args.at(index))
    { return _args.at(index); }
    inline std::string const & at(std::size_t index) const TBAG_NOEXCEPT_SP_OP(_args.at(index))
    { return _args.at(index); }

public:
    void erase(std::size_t index)
    { _args.erase(_args.begin() + index); }
    void erase(std::string const & argument)
    { _args.erase(std::find(_args.begin(), _args.end(), argument)); }

public:
    void push_front(std::string const & argument)
    { _args.insert(_args.begin(), argument); }
    void pop_front()
    { erase(0); }

public:
    void push_back(std::string const & argument)
    { _args.push_back(argument); }
    void pop_back()
    { _args.pop_back(); }

public:
    void insert(std::size_t index, std::string const & argument);
    bool parse(std::string const & arguments, std::string const & delimiter = DELIMITER);

public:
    std::string toString(std::string const & delimiter = DELIMITER) const;

public:
    template <typename T>
    bool opt(std::size_t index, T * output) const
    {
        if (index < _args.size()) {
            return libtbag::string::toVal(_args[index], *output);
        }
        return false;
    }

    template <typename T>
    T get(std::size_t index, T def = T()) const
    {
        T output;
        if (opt(index, &output)) {
            return output;
        } else {
            return def;
        }
    }
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__

