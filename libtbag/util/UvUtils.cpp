/**
 * @file   UvUtils.cpp
 * @brief  UvUtils class implementation.
 * @author zer0
 * @date   2016-11-03
 */

#include <libtbag/util/UvUtils.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

void initUv()
{
    uv_loop_t loop;
    ::uv_loop_init(&loop);
    ::uv_loop_close(&loop);
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

