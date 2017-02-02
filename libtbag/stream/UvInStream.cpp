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

UvInStream::UvInStream(UvHandleType type) : uvpp::Handle(type), _write(uvpp::utype::WRITE), _on_write_cb(nullptr)
{
    TBAG_UV_EVENT_DEFAULT_REGISTER(_write.get(), this);
}

UvInStream::~UvInStream()
{
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(_write.get());
}

Err UvInStream::write(char const * buffer, std::size_t length)
{
    uv_buf_t info = {0,};
    info.base = const_cast<char*>(buffer);
    info.len  = length;

    int const CODE = ::uv_write(_write.cast<uv_write_t>(), this->cast<uv_stream_t>(),
                                &info, 1, TBAG_UV_EVENT_DEFAULT_CALLBACK_WRITE(onWrite));
    if (CODE != 0) {
        __tbag_error("UvInStream::write() error[{}] {}", CODE, uvpp::getUvErrorName(CODE));
        return Err::FAILURE;
    }
    return Err::SUCCESS;
}

Err UvInStream::write(std::vector<char> const & buffer)
{
    return write(&buffer[0], buffer.size());
}

void UvInStream::onWrite(void * request, int status)
{
    Err code;
    if (status == 0) {
        code = Err::SUCCESS;
    } else {
        code = Err::FAILURE;
    }

    if (_on_write_cb != nullptr) {
        _on_write_cb->onWrite(code);
    }
}

} // namespace stream

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

