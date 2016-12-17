/**
 * @file   UvInStream.cpp
 * @brief  UvInStream class implementation.
 * @author zer0
 * @date   2016-12-16
 */

#include <libtbag/stream/UvInStream.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace stream {

// -----------------------
// UvInStream message map.
// -----------------------

TBAG_UV_EVENT_DEFAULT_IMPLEMENT_OPEN(UvInStream);
//{
    TBAG_UV_EVENT_WRITE(onWrite);
//}
TBAG_UV_EVENT_DEFAULT_IMPLEMENT_CLOSE(UvInStream);

// --------------------------
// UvInStream implementation.
// --------------------------

UvInStream::UvInStream(UvHandleType type) : util::UvHandle(type), _write(util::UvType::WRITE), _on_write_cb(nullptr)
{
    TBAG_UV_EVENT_DEFAULT_REGISTER(_write.getNative(), this);
}

UvInStream::~UvInStream()
{
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(_write.getNative());
}

ErrorCode UvInStream::write(char const * buffer, std::size_t length)
{
    uv_buf_t info = {0,};
    info.base = const_cast<char*>(buffer);
    info.len  = length;

    int const CODE = ::uv_write(_write.castNative<uv_write_t>(), this->castNative<uv_stream_t>(),
                                &info, 1, TBAG_UV_EVENT_DEFAULT_CALLBACK_WRITE(onWrite));
    if (CODE != 0) {
        __tbag_error("UvInStream::write() error[{}] {}", CODE, util::getUvErrorName(CODE));
        return ErrorCode::FAILURE;
    }
    return ErrorCode::SUCCESS;
}

ErrorCode UvInStream::write(std::vector<char> const & buffer)
{
    return write(&buffer[0], buffer.size());
}

void UvInStream::onWrite(void * request, int status)
{
    ErrorCode code;
    if (status == 0) {
        code = ErrorCode::SUCCESS;
    } else {
        code = ErrorCode::FAILURE;
    }

    if (_on_write_cb != nullptr) {
        _on_write_cb->onWrite(code);
    }
}

} // namespace stream

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

