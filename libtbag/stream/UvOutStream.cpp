/**
 * @file   UvOutStream.cpp
 * @brief  UvOutStream class implementation.
 * @author zer0
 * @date   2016-12-16
 */

#include <libtbag/stream/UvOutStream.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace stream {

// ------------------------
// UvOutStream message map.
// ------------------------

TBAG_UV_EVENT_DEFAULT_IMPLEMENT_OPEN(UvOutStream);
//{
    TBAG_UV_EVENT_ALLOC(onAlloc);
    TBAG_UV_EVENT_READ(onRead);
//}
TBAG_UV_EVENT_DEFAULT_IMPLEMENT_CLOSE(UvOutStream);

// ---------------------------
// UvOutStream implementation.
// ---------------------------

UvOutStream::UvOutStream(UvHandleType type) : uvpp::Handle(type), _on_read_cb(nullptr)
{
    TBAG_UV_EVENT_DEFAULT_REGISTER(this->get(), this);
}

UvOutStream::~UvOutStream()
{
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(this->get());
}

Err UvOutStream::read()
{
    int const CODE = ::uv_read_start(this->cast<uv_stream_t>(),
                                     TBAG_UV_EVENT_DEFAULT_CALLBACK_ALLOC(onAlloc),
                                     TBAG_UV_EVENT_DEFAULT_CALLBACK_READ(onRead));
    if (CODE != 0) {
        __tbag_error("UvOutStream::read() error[{}] {}", CODE, uvpp::getUvErrorName(CODE));
        return Err::FAILURE;
    }
    return Err::SUCCESS;
}

void UvOutStream::onAlloc(void * handle, size_t suggested_size, void * buf)
{
    // Realloc with read buffer.
    if (_buffer.size() < suggested_size) {
        _buffer.resize(suggested_size);
    }

    uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);
    uv_buf->base = &_buffer[0];
    uv_buf->len  =  _buffer.size();
}

void UvOutStream::onRead(void * stream, ssize_t nread, void const * buf)
{
    Err code = Err::FAILURE;

    if (nread == UV_EOF) {
        code = Err::END_OF_FILE;
    } else if (nread >= 0){
        code = Err::SUCCESS;
    }

    if (_on_read_cb != nullptr) {
        _on_read_cb->onRead(code, nread, buf);
    }
}

} // namespace stream

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

