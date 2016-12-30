/**
 * @file   HandleFactory.cpp
 * @brief  HandleFactory class implementation.
 * @author zer0
 * @date   2016-12-30
 */

#include <libtbag/uv/ex/HandleFactory.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Handle.hpp>

#include <libtbag/uv/Tcp.hpp>
#include <libtbag/uv/Idle.hpp>
#include <libtbag/uv/Async.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {
namespace ex {

Handle * createHandle(Loop & loop, UvHandleType type)
{
    // @formatter:off
    switch (type) {
    case   UvHandleType::TCP     : return new Tcp    (loop);
    case   UvHandleType::IDLE    : return new Idle   (loop);
    //case UvHandleType::PIPE    : return new Pipe   (loop);
    //case UvHandleType::TTY     : return new Tty    (loop);
    //case UvHandleType::UDP     : return new Udp    (loop);
    //case UvHandleType::POLL    : return new Poll   (loop);
    //case UvHandleType::TIMER   : return new Timer  (loop);
    //case UvHandleType::PREPARE : return new Prepare(loop);
    //case UvHandleType::CHECK   : return new Check  (loop);
    case UvHandleType::ASYNC     : return new Async  (loop);
    //case UvHandleType::PROCESS : return new Process(loop);
    //case UvHandleType::FS_EVENT: return new FsEvent(loop);
    //case UvHandleType::FS_POLL : return new FsPoll (loop);
    //case UvHandleType::SIGNAL  : return new Signal (loop);
    }
    // @formatter:on

    __tbag_error("createHandle() Unsupported handle type [{}].", static_cast<int>(type));
    return nullptr;
}

} // namespace ex
} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

