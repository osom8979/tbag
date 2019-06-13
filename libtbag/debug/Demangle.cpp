/**
 * @file   Demangle.cpp
 * @brief  Demangle class implementation.
 * @author zer0
 * @date   2017-09-02
 */

#include <libtbag/debug/Demangle.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

#include <demangle.h>

// __has_include is currently supported by GCC and Clang. However GCC 4.9 may have issues and
// returns 1 for 'defined(__has_include)', while '__has_include' is actually not supported:
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63662
#if defined(__has_include)
# if defined(TBAG_COMP_CLANG) || (defined(TBAG_COMP_GNUC_CXX) && (TBAG_COMP_GNUC_VERSION >= 50000))
#  if __has_include(<cxxabi.h>)
#   define _HAVE_CXXABI_H
#  endif
# endif
#endif

#if defined(_HAVE_CXXABI_H)
# include <cxxabi.h>
// For some archtectures (mips, mips64, x86, x86_64) cxxabi.h in Android NDK is implemented by gabi++ library
// (https://android.googlesource.com/platform/ndk/+/master/sources/cxx-stl/gabi++/), which does not implement
// abi::__cxa_demangle(). We detect this implementation by checking the include guard here.
# if defined(__GABIXX_CXXABI_H__)
#  undef _HAVE_CXXABI_H
# else
#  include <cstdlib>
#  include <cstddef>
# endif
#endif

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

std::string getAbiDemangle(char const * name)
{
    std::string result;

#if defined(_HAVE_CXXABI_H)
    int status = 0;
    std::size_t size = 0;
    char * demangle = nullptr;

    // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
    demangle = abi::__cxa_demangle(name, nullptr, &size, &status);

    if (status != 0) {
        if (status == -1) {
            tDLogE("getDemangle({}) A memory allocation failiure occurred", name);
        } else if (status == -2) {
            tDLogE("getDemangle({}) Name is not a valid name under the C++ ABI mangling rules", name);
        } else if (status == -3) {
            tDLogE("getDemangle({}) One of the arguments is invalid", name);
        } else {
            tDLogE("getDemangle({}) Unknown error code: {}", name, status);
        }
        return std::string();
    }

    // The demangling operation succeeded.
    assert(status == 0);
    result = demangle;
    std::free(const_cast<char*>(demangle));
#endif

    return result;
}

std::string getDemangle(char const * name)
{
    int const BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE + 1/*NULL_CHAR*/] = {0,};
    if (!::Demangle(name, buffer, BUFFER_SIZE)) {
        tDLogE("getDemangle({}) demangle failure.", name);
        return std::string();
    }
    return std::string(buffer);
}

// ------------------------
// Demangle implementation.
// ------------------------

Demangle::Demangle(char const * name) : _name(name), _demangle(libtbag::debug::getDemangle(name))
{
    // EMPTY.
}

Demangle::~Demangle()
{
    // EMPTY.
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

