/**
 * @file   PipeProcess.hpp
 * @brief  PipeProcess class prototype.
 * @author zer0
 * @date   2016-05-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/process/Process.hpp>

#include <array>
#include <vector>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * PipeProcess class prototype.
 *
 * @author zer0
 * @date   2016-05-19
 */
class PipeProcess : public Process
{
public:
    static constexpr int const STANDARD_INPUT_FD  = 0; ///< @c stdin
    static constexpr int const STANDARD_OUTPUT_FD = 1; ///< @c stdout
    static constexpr int const STANDARD_ERROR_FD  = 2; ///< @c stderr

    static constexpr int const STDIO_SIZE = 3;

private:
    uv_pipe_t _pipe_stdin;
    uv_pipe_t _pipe_stdout;
    uv_pipe_t _pipe_stderr;

    std::array<uv_stdio_container_t, STDIO_SIZE> _stdios;

private:
    std::vector<char> _buffer_stdout;
    std::vector<char> _buffer_stderr;

    std::stringstream _stream_stdout;
    std::stringstream _stream_stderr;

public:
    PipeProcess() {
        this->initPipe();

        this->addHandle((uv_handle_t*)&this->_pipe_stdin);
        this->addHandle((uv_handle_t*)&this->_pipe_stdout);
        this->addHandle((uv_handle_t*)&this->_pipe_stderr);
    }

    virtual ~PipeProcess() {
        this->removeHandle((uv_handle_t*)&this->_pipe_stdin);
        this->removeHandle((uv_handle_t*)&this->_pipe_stdout);
        this->removeHandle((uv_handle_t*)&this->_pipe_stderr);
    }

private:
    void initPipe() {
        // The ipc argument is a boolean to indicate
        // if this pipe will be used for handle passing between processes.
        int const ENABLE_IPC = 0; // enable == 1, disable == 0.

        // uv_pipe_init(this->getLoopPointer(), &_pipe_stdin , ENABLE_IPC);
        uv_pipe_init(this->getLoopPointer(), &_pipe_stdout, ENABLE_IPC);
        uv_pipe_init(this->getLoopPointer(), &_pipe_stderr, ENABLE_IPC);

        uv_stdio_flags const PIPE_FLAGS =
                static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);

        //this->_stdios[STANDARD_INPUT_FD ].flags = PIPE_FLAGS;
        //this->_stdios[STANDARD_INPUT_FD ].data.stream = (uv_stream_t*)&this->_pipe_stdin;
        this->_stdios[STANDARD_INPUT_FD ].flags = UV_IGNORE;
        this->_stdios[STANDARD_INPUT_FD ].data.stream = nullptr;

        this->_stdios[STANDARD_OUTPUT_FD].flags = PIPE_FLAGS;
        this->_stdios[STANDARD_OUTPUT_FD].data.stream = (uv_stream_t*)&this->_pipe_stdout;

        this->_stdios[STANDARD_ERROR_FD ].flags = PIPE_FLAGS;
        this->_stdios[STANDARD_ERROR_FD ].data.stream = (uv_stream_t*)&this->_pipe_stderr;

        uv_process_options_t & options = this->atOptions();
        options.stdio = &this->_stdios[0];
        options.stdio_count = STDIO_SIZE;
    }

private:
    void reallocWithStreamBuffer(std::vector<char> & buffer, std::size_t size) {
        if (buffer.size() < size) {
            buffer.resize(size);
        }
    }

public:
    virtual void onAlloc(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf) override {
        if (handle == (uv_handle_t*)&this->_pipe_stdout) {
            reallocWithStreamBuffer(this->_buffer_stdout, suggested_size);
            buf->base = &this->_buffer_stdout[0];
            buf->len  =  this->_buffer_stdout.size();
        } else if (handle == (uv_handle_t*)&this->_pipe_stderr) {
            reallocWithStreamBuffer(this->_buffer_stderr, suggested_size);
            buf->base = &this->_buffer_stderr[0];
            buf->len  =  this->_buffer_stderr.size();
        } else {
            // ERROR.
        }
    }

public:
    virtual void onRead(uv_stream_t * stream, ssize_t nread, uv_buf_t const * buf) override {
        if (nread == UV_EOF) {
            // END OF FILE.
        } else if (nread < 0){
            // ERROR.
        } else {
            if (stream == (uv_stream_t*)&this->_pipe_stdout) {
                this->_stream_stdout.write(&this->_buffer_stdout[0], nread);
            } else if (stream == (uv_stream_t*)&this->_pipe_stderr) {
                this->_stream_stderr.write(&this->_buffer_stderr[0], nread);
            } else {
                // ERROR.
            }
        }
    }

public:
    virtual void onWrite(uv_write_t * req, int status) override {
    }

public:
    bool read() {
        int error_code = 0;
        REMOVE_UNUSED_VARIABLE(error_code);

        error_code |= uv_read_start((uv_stream_t*) &this->_pipe_stdout, &loop::onAlloc, &loop::onRead);
        assert(error_code == 0);

        error_code |= uv_read_start((uv_stream_t*) &this->_pipe_stderr, &loop::onAlloc, &loop::onRead);
        assert(error_code == 0);

        return (error_code == 0 ? true : false);
    }

public:
    std::string getStandardOutput() const {
        return this->_stream_stdout.str();
    }

    std::string getStandardError() const {
        return this->_stream_stderr.str();
    }
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__

