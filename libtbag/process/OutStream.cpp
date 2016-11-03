/**
 * @file   OutStream.cpp
 * @brief  OutStream class implementation.
 * @author zer0
 * @date   2016-11-03
 */

#include <libtbag/process/OutStream.hpp>
#include <libtbag/loop/event/UvHandler.hpp>

#include <cstring>
#include <vector>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * Pointer to implementation of @c uv_pipe_t.
 *
 * @author zer0
 * @date   2016-11-03
 *
 * @remarks
 *  Use the libuv.
 */
struct OutStream::PipePimpl : public libtbag::loop::event::UvHandler
{
private:
    OutStream & _parent;
    uv_pipe_t _pipe;

private:
    std::vector<char> _buffer;

public:
    PipePimpl(OutStream & parent) : UvHandler(&_pipe), _parent(parent)
    {
        ::memset(&_pipe, 0x00, sizeof(uv_pipe_t));
    }

    ~PipePimpl()
    {
        // EMPTY.
    }

public:
    bool startRead()
    {
        return ::uv_read_start((uv_stream_t*)&_pipe, TBAG_UV_EVENT_CALLBACK_ALLOC, TBAG_UV_EVENT_CALLBACK_READ) == 0;
    }

    void reallocWithStreamBuffer(std::size_t size)
    {
        if (_buffer.size() < size) {
            _buffer.resize(size);
        }
    }

public:
    virtual void onAlloc(void * handle, size_t suggested_size, void * buf) override
    {
        reallocWithStreamBuffer(suggested_size);

        uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);
        uv_buf->base = &this->_buffer[0];
        uv_buf->len  =  this->_buffer.size();
    }

    virtual void onRead(/*uv_stream_t*/void * stream, ssize_t nread, /*uv_buf_t*/void const * buf) override
    {
        OutStream::ErrorCode code;
        if (nread == UV_EOF) {
            code = OutStream::ErrorCode::END_OF_FILE;
        } else if (nread < 0){
            code = OutStream::ErrorCode::UNKNOWN_ERROR;
        } else {
            code = OutStream::ErrorCode::SUCCESS;
        }

        if (static_cast<bool>(_parent._callback)) {
            _parent._callback(code, &_buffer[0], static_cast<std::size_t>(nread));
        }
    }
};

// -------------------------
// OutStream implementation.
// -------------------------

OutStream::OutStream(OnReadCallback const & callback) : _pipe(new PipePimpl(*this)), _callback(callback)
{
    // EMPTY.
}

OutStream::OutStream() : OutStream(nullptr)
{
    // EMPTY.
}

OutStream::~OutStream()
{
    // EMPTY.
}

void * OutStream::getNative()
{
    return _pipe->getNative();
}

void const * OutStream::getNative() const
{
    return _pipe->getNative();
}

bool OutStream::startRead()
{
    return _pipe->startRead();
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

