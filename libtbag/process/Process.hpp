/**
 * @file   Process.hpp
 * @brief  Process class prototype.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <cstdlib>
#include <cassert>

#include <string>
#include <vector>
#include <functional>
#include <atomic>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

// Forward declaration.
class ProcessManager;

/**
 * Process result structure.
 *
 * @author zer0
 * @date   2016-05-17
 */
struct ProcessResult
{
    int64_t  exit_status;
    int      terminate_signal;
};

/**
 * Process class prototype.
 *
 * @author zer0
 * @date   2016-05-17
 */
class Process : public Noncopyable
{
public:
    friend ProcessManager;

private:
    std::atomic<int64_t> _exit_status;
    std::atomic<int> _terminate_signal;

private:
    uv_loop_t _loop;
    uv_process_t _process;
    uv_process_options_t _options;

private:
    std::string _execute_file;
    std::string _working_directory;

    std::vector<std::string> _arguments;
    std::vector<char*> _argument_pointers;

public:
    Process() : _exit_status(0), _terminate_signal(0) {
        memset((void*)&this->_options, 0x00, sizeof(this->_options));
    }

    ~Process() {
        __EMPTY_BLOCK__
    }

private:
    /**
     * Fully close a loop
     */
    static void onWalkClose(uv_handle_t * handle, void * arg) {
        if (uv_is_closing(handle) == false) {
            uv_close(handle, nullptr);
        }
    }

public:
    void init() {
        uv_loop_init(&this->_loop);
    }

    bool spawn(uv_exit_cb callback) {
        this->_options.exit_cb = callback;
        return (uv_spawn(&this->_loop, &this->_process, &this->_options) == 0 ? true : false);
    }

    void run() {
        uv_run(&this->_loop, UV_RUN_DEFAULT);
    }

    bool close() {
        uv_walk(&this->_loop, &Process::onWalkClose, nullptr);
        uv_run(&this->_loop, UV_RUN_DEFAULT);
        return (uv_loop_close(&this->_loop) == 0 ? true : false);
    }

public:
    inline bool isSameHandle(uv_handle_t const * handle) const noexcept {
        return (handle == (uv_handle_t*)&this->_process);
    }

    inline bool isSameHandle(uv_process_t const * handle) const noexcept {
        return (handle == &this->_process);
    }

    inline bool operator == (uv_handle_t const * handle) const noexcept {
        return isSameHandle(handle);
    }

    inline bool operator == (uv_process_t const * handle) const noexcept {
        return isSameHandle(handle);
    }

private:
    void updateArgumentPointer() {
        std::size_t const ARGS_SIZE = this->_arguments.size();

        this->_argument_pointers.clear();
        this->_argument_pointers.resize(ARGS_SIZE + 2);

        // First: exe file path.
        this->_argument_pointers[0] = &this->_execute_file[0];

        // Arguments.
        for (std::size_t index = 0; index < ARGS_SIZE; ++index) {
            this->_argument_pointers[index + 1] = &this->_arguments[index][0];
        }

        // Last: NULL pointer.
        this->_argument_pointers[ARGS_SIZE + 1] = nullptr;

        // Update option.
        this->_options.args = &this->_argument_pointers[0];
    }

public:
    void setExecuteFile(std::string const & path) {
        this->_execute_file = path;
        this->_options.file = this->_execute_file.c_str();
        this->updateArgumentPointer();
    }

    void setArguments(std::vector<std::string> const & args) {
        this->_arguments = args;
        this->updateArgumentPointer();
    }

    void setWorkingDirectory(std::string const & path) {
        this->_working_directory = path;
        this->_options.cwd = this->_working_directory.c_str();
    }

    void setFlags(unsigned int flags) {
        this->_options.flags = flags;
    }

public:
    uv_process_options_t const & getOptions() const noexcept {
        return this->_options;
    }

    ProcessResult getResult() const noexcept {
        ProcessResult result;
        memset((void*)&result, 0x00, sizeof(result));

        result.exit_status      = _exit_status.load();
        result.terminate_signal = _terminate_signal.load();
        return result;
    }

public:
    void onExit(int64_t exit_status, int signal) {
        this->_exit_status.store(exit_status);
        this->_terminate_signal.store(signal);
    }

    void onClose() {
        __EMPTY_BLOCK__
    }

public:
    inline int64_t getExitStatus() const noexcept {
        return this->_exit_status.load();
    }

    inline int getTerminateSignal() const noexcept {
        return this->_terminate_signal;
    }
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PROCESS_HPP__

