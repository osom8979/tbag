/**
 * @file   Demangle.hpp
 * @brief  Demangle class prototype.
 * @author zer0
 * @date   2017-09-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_DEMANGLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_DEMANGLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

TBAG_API std::string getAbiDemangle(char const * name);
TBAG_API std::string getDemangle(char const * name);

/**
 * Demangle class prototype.
 *
 * @author zer0
 * @date   2017-09-02
 */
class TBAG_API Demangle : private Noncopyable
{
private:
    std::string _name;
    std::string _demangle;

public:
    Demangle(char const * name);
    ~Demangle();

public:
    inline bool isDemangled() const TBAG_NOEXCEPT_SP_OP(_demangle.empty())
    { return (_demangle.empty() == false); }

public:
    inline std::string getName() const { return _name; }
    inline std::string getDemangle() const { return _demangle; }
};

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_DEMANGLE_HPP__

