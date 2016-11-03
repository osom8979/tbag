/**
 * @file   InStream.cpp
 * @brief  InStream class implementation.
 * @author zer0
 * @date   2016-11-03
 */

#include <libtbag/process/InStream.hpp>

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
    PipePimpl(InStream & parent) : UvHandler(&_pipe), _parent(parent)
    {
        ::memset(&_pipe, 0x00, sizeof(uv_pipe_t));
    }

    ~PipePimpl()
    {
        // EMPTY.
    }

public:
    bool write(char * buffer, std::size_t length)
    {
        _buffer.clear();
        _buffer.assign(buffer, buffer + length);

        _buffer_info.base = &_buffer[0];
        _buffer_info.len  =  _buffer.size();

        int error_code = uv_write(
                  &this->_write
                , (uv_stream_t*)&this->_pipe
                , &this->_buffer_info
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

        if (_parent._callback != nullptr) {
            _parent._callback->onWrite(code);
        }
    }
};

// -------------------------
// InStream implementation.
// -------------------------

InStream::InStream(OnWriteCallback * callback) : _pipe(new PipePimpl(*this)), _callback(callback)
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
    return _pipe->getNative();
}

void const * InStream::getNative() const
{
    return _pipe->getNative();
}

bool InStream::write(char * buffer, std::size_t length)
{
    return _pipe->write(buffer, length);
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

