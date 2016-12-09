/**
 * @file   UvUtils.cpp
 * @brief  UvUtils class implementation.
 * @author zer0
 * @date   2016-11-03
 */

#include <libtbag/util/UvUtils.hpp>
#include <cstdlib>
#include <cstring>
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

std::string getUvErrorString(int uv_error_code)
{
    return std::string(uv_strerror(uv_error_code));
}

std::string getUvErrorName(int uv_error_code)
{
    return std::string(uv_err_name(uv_error_code));
}

// ------------------------
// UvHandle implementation.
// ------------------------

UvHandle::UvHandle(UvType type) : TYPE(type), _native(nullptr)
{
#ifndef __CASE_TYPE_ALLOC
#define __CASE_TYPE_ALLOC(type, uv_type)          \
    case UvType::type:                            \
        _native = ::malloc(sizeof(uv_type));      \
        ::memset(_native, 0x00, sizeof(uv_type)); \
        break
#endif

    switch (TYPE) {
    __CASE_TYPE_ALLOC(LOOP             , uv_loop_t);
    __CASE_TYPE_ALLOC(HANDLE           , uv_handle_t);
    __CASE_TYPE_ALLOC(STREAM           , uv_stream_t);
    __CASE_TYPE_ALLOC(TCP              , uv_tcp_t);
    __CASE_TYPE_ALLOC(UDP              , uv_udp_t);
    __CASE_TYPE_ALLOC(PIPE             , uv_pipe_t);
    __CASE_TYPE_ALLOC(TTY              , uv_tty_t);
    __CASE_TYPE_ALLOC(POLL             , uv_poll_t);
    __CASE_TYPE_ALLOC(TIMER            , uv_timer_t);
    __CASE_TYPE_ALLOC(PREPARE          , uv_prepare_t);
    __CASE_TYPE_ALLOC(CHECK            , uv_check_t);
    __CASE_TYPE_ALLOC(IDLE             , uv_idle_t);
    __CASE_TYPE_ALLOC(ASYNC            , uv_async_t);
    __CASE_TYPE_ALLOC(PROCESS          , uv_process_t);
    __CASE_TYPE_ALLOC(FS_EVENT         , uv_fs_event_t);
    __CASE_TYPE_ALLOC(FS_POLL          , uv_fs_poll_t);
    __CASE_TYPE_ALLOC(SIGNAL           , uv_signal_t);
    __CASE_TYPE_ALLOC(REQ              , uv_req_t);
    __CASE_TYPE_ALLOC(GETADDRINFO      , uv_getaddrinfo_t);
    __CASE_TYPE_ALLOC(GETNAMEINFO      , uv_getnameinfo_t);
    __CASE_TYPE_ALLOC(SHUTDOWN         , uv_shutdown_t);
    __CASE_TYPE_ALLOC(WRITE            , uv_write_t);
    __CASE_TYPE_ALLOC(CONNECT          , uv_connect_t);
    __CASE_TYPE_ALLOC(UDP_SEND         , uv_udp_send_t);
    __CASE_TYPE_ALLOC(FS               , uv_fs_t);
    __CASE_TYPE_ALLOC(WORK             , uv_work_t);
    __CASE_TYPE_ALLOC(CPU_INFO         , uv_cpu_info_t);
    __CASE_TYPE_ALLOC(INTERFACE_ADDRESS, uv_interface_address_t);
    __CASE_TYPE_ALLOC(DIRENT           , uv_dirent_t);
    __CASE_TYPE_ALLOC(PASSWD           , uv_passwd_t);
    }

#undef __CASE_TYPE_ALLOC
}

UvHandle::~UvHandle()
{
    if (_native != nullptr) {
        ::free(_native);
        _native = nullptr;
    }
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

