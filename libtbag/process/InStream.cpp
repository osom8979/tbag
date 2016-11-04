/**
 * @file   InStream.cpp
 * @brief  InStream class implementation.
 * @author zer0
 * @date   2016-11-03
 */

#include <libtbag/process/InStream.hpp>

#include <cstring>
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
struct InStream::PipePimpl : public libtbag::loop::event::UvHandler
{
private:
    InStream & _parent;
    uv_pipe_t _pipe;

private:
    std::vector<char> _buffer;
    uv_buf_t          _buffer_info;
    uv_write_t        _write;

public:
    PipePimpl(InStream & parent) : _parent(parent)
    {
        ::memset(&_pipe, 0x00, sizeof(_pipe));
        ::memset(&_write, 0x00, sizeof(_write));
        add(&_pipe);
        add(&_write);
    }

    ~PipePimpl()
    {
        // EMPTY.
    }

public:
    inline uv_pipe_t * getPipe()
    { return &_pipe; }
    inline uv_pipe_t const * getPipe() const
    { return &_pipe; }

public:
    bool write(char const * buffer, std::size_t length)
    {
        _buffer.clear();
        _buffer.assign(buffer, buffer + length);

        _buffer_info.base = &_buffer[0];
        _buffer_info.len  =  _buffer.size();

        int error_code = uv_write(
                  &_write
                , (uv_stream_t*)&_pipe
                , &_buffer_info
                , 1
                , TBAG_UV_EVENT_CALLBACK_WRITE);

        // std::cout << uv_err_name(error_code) << ": "
        //           << uv_strerror(error_code) << std::endl;

        return (error_code == 0 ? true : false);
    }

public:
    virtual void onWrite(/*uv_write_t*/void * req, int status) override
    {
        InStream::ErrorCode code;
        if (status == 0) {
            code = InStream::ErrorCode::SUCCESS;
        } else {
            code = InStream::ErrorCode::UNKNOWN_ERROR;
        }

        if (static_cast<bool>(_parent._callback)) {
            _parent._callback(code);
        }
    }
};

// -------------------------
// InStream implementation.
// -------------------------

InStream::InStream(OnWriteCallback const & callback) : _pipe(new PipePimpl(*this)), _callback(callback)
{
    // EMPTY.
}

InStream::InStream() : InStream(nullptr)
{
    // EMPTY.
}

InStream::~InStream()
{
    // EMPTY.
}

void * InStream::getNative()
{
    return _pipe->getPipe();
}

void const * InStream::getNative() const
{
    return _pipe->getPipe();
}

bool InStream::write(char const * buffer, std::size_t length)
{
    return _pipe->write(buffer, length);
}

bool InStream::write(std::vector<char> const & buffer)
{
    return write(&buffer[0], buffer.size());
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

