/**
 * @file   AccelRaw.cpp
 * @brief  AccelRaw raw C++ implementation.
 * @author zer0
 * @date   2018-01-19
 */

#include <libtbag/gpu/accel/AccelRaw.hpp>
#include <libtbag/gpu/accel/AccelRaw.h>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu   {
namespace accel {

bool runAccelAdd(type::TypeTable type, void const * v1, void const * v2, void * r, int count)
{
    switch (type) {
    case type::TypeTable::TT_INT:    return tbAccelAdd1i((int      const *)v1, (int      const *)v2, (int      *)r, count) == TB_TRUE;
    case type::TypeTable::TT_UINT:   return tbAccelAdd1u((unsigned const *)v1, (unsigned const *)v2, (unsigned *)r, count) == TB_TRUE;
    case type::TypeTable::TT_FLOAT:  return tbAccelAdd1f((float    const *)v1, (float    const *)v2, (float    *)r, count) == TB_TRUE;
    case type::TypeTable::TT_DOUBLE: return tbAccelAdd1d((double   const *)v1, (double   const *)v2, (double   *)r, count) == TB_TRUE;
    default: break;
    }
    return false;
}

} // namespace accel
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

