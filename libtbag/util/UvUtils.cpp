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

char const * getUvType(void * handle)
{
#ifndef __CASE_TYPE_NAME_RETURN
#define __CASE_TYPE_NAME_RETURN(name) case name: return #name
#endif
    switch (static_cast<uv_handle_t*>(handle)->type) {
    __CASE_TYPE_NAME_RETURN(UV_NAMED_PIPE);
    __CASE_TYPE_NAME_RETURN(UV_TTY);
    __CASE_TYPE_NAME_RETURN(UV_TCP);
    __CASE_TYPE_NAME_RETURN(UV_UDP);
    __CASE_TYPE_NAME_RETURN(UV_PREPARE);
    __CASE_TYPE_NAME_RETURN(UV_CHECK);
    __CASE_TYPE_NAME_RETURN(UV_IDLE);
    __CASE_TYPE_NAME_RETURN(UV_ASYNC);
    __CASE_TYPE_NAME_RETURN(UV_TIMER);
    __CASE_TYPE_NAME_RETURN(UV_PROCESS);
    __CASE_TYPE_NAME_RETURN(UV_FS_EVENT);
    __CASE_TYPE_NAME_RETURN(UV_POLL);
    __CASE_TYPE_NAME_RETURN(UV_FS_POLL);
    __CASE_TYPE_NAME_RETURN(UV_SIGNAL);
    default: return "Unknown handle type.";
    }
#undef __CASE_TYPE_NAME_RETURN
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

