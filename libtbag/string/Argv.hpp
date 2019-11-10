/**
 * @file   Argv.hpp
 * @brief  Argv class prototype.
 * @author zer0
 * @date   2019-11-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGV_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGV_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * Argv class prototype.
 *
 * @author zer0
 * @date   2019-11-10
 */
class TBAG_API Argv
{
private:
    std::vector<std::string> _strings;
    std::vector<char*> _arguments;

public:
    Argv();
    Argv(int argc, char const ** argv);
    Argv(char const ** argv);
    Argv(std::vector<std::string> const & argv);
    Argv(Argv const & obj);
    Argv(Argv && obj) TBAG_NOEXCEPT;
    ~Argv();

public:
    Argv & operator =(Argv const & obj);
    Argv & operator =(Argv && obj) TBAG_NOEXCEPT;

public:
    void copy(Argv const & obj);
    void swap(Argv & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Argv & lh, Argv & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    std::vector<std::string> strings() const TBAG_NOEXCEPT
    { return _strings; }

public:
    inline int argc() const TBAG_NOEXCEPT_SP_OP(_strings.size())
    { return static_cast<int>(_strings.size()); }
    inline char ** argv() TBAG_NOEXCEPT_SP_OP(_arguments.data())
    { return _arguments.data(); }

public:
    void clear();

public:
    static std::size_t getArgvSize(char const ** argv);

public:
    void parse(int argc, char const ** argv);
    void parse(char const ** argv);
    void parse(std::vector<std::string> const & argv);
};

TBAG_CONSTEXPR char const * const TBAG_MAIN_ARGV_KEY = "libtbag::string::Argv";

TBAG_API std::weak_ptr<Argv> getMainArgv();

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGV_HPP__

