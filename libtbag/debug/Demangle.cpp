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

bool enableAbiDemangle() TBAG_NOEXCEPT
{
#if defined(_HAVE_CXXABI_H)
    return true;
#else
    return false;
#endif
}

std::string getAbiDemangle(char const * name, int * output_status)
{
    std::string result;

#if defined(_HAVE_CXXABI_H)
    int status = 0;
    std::size_t size = 0;
    char * demangle = nullptr;

    // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
    demangle = abi::__cxa_demangle(name, nullptr, &size, &status);

    if (output_status) {
        *output_status = status;
    }

    if (status != 0) {
        if (status == -1) {
            // A memory allocation failiure occurred
        } else if (status == -2) {
            // Name is not a valid name under the C++ ABI mangling rules
        } else if (status == -3) {
            // One of the arguments is invalid
        } else {
            // Unknown error code
        }
        return {};
    }

    // The demangling operation succeeded.
    assert(status == 0);
    result = demangle;
    std::free(const_cast<char*>(demangle));
#endif

    return result;
}

std::string getGtestDemangle(char const * name, int * output_status)
{
    char buffer[GTEST_DEMANGLE_BUFFER_SIZE+1/*NULL_CHAR*/] = {0,};
    auto const status = ::Demangle(name, buffer, GTEST_DEMANGLE_BUFFER_SIZE) ? 0 : 1;
    if (output_status) {
        *output_status = status;
    }
    if (status) {
        return buffer;
    }
    return {};
}

std::string getDemangle(char const * name)
{
    if (enableAbiDemangle()) {
        int abi_status = 0;
        auto const result = getAbiDemangle(name, &abi_status);
        if (!abi_status) {
            return result;
        }
    }

    int gtest_status = 0;
    auto const result = getGtestDemangle(name, &gtest_status);
    if (!gtest_status) {
        return result;
    }

    return name;
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

