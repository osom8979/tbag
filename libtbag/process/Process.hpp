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
#include <array>
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
    uv_loop_t             _loop;
    uv_process_t          _process;
    uv_process_options_t  _options;

public:
    static constexpr int const STANDARD_INPUT_FD  = 0; ///< @c stdin
    static constexpr int const STANDARD_OUTPUT_FD = 1; ///< @c stdout
    static constexpr int const STANDARD_ERROR_FD  = 2; ///< @c stderr
    uv_pipe_t _pipe_stdin;
    uv_pipe_t _pipe_stdout;
    uv_pipe_t _pipe_stderr;

    static constexpr int const STDIO_SIZE = 3;
    std::array<uv_stdio_container_t, STDIO_SIZE> _stdios;


private:
    std::string _execute_file;
    std::string _working_directory;

    std::vector<std::string>  _arguments;
    std::vector<char*>        _argument_pointers;

    std::vector<std::string>  _envs; ///< Environment variables.
    std::vector<char*>        _env_pointers;

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

    void updateEnvironmentVariablePointer() {
        std::size_t const ENVS_SIZE = this->_arguments.size();

        this->_env_pointers.clear();
        this->_env_pointers.resize(ENVS_SIZE + 1);

        // Arguments.
        for (std::size_t index = 0; index < ENVS_SIZE; ++index) {
            this->_env_pointers[index] = &this->_envs[index][0];
        }

        // Last: NULL pointer.
        this->_argument_pointers[ENVS_SIZE] = nullptr;

        // Update options.
        this->_options.env = &this->_env_pointers[0];
    }

public:
    void setStdios() {
        // The ipc argument is a boolean to indicate
        // if this pipe will be used for handle passing between processes.
        bool const ENABLE_IPC = false;

        //uv_pipe_init(&this->_loop, &this->_pipe_stdin,  (ENABLE_IPC == true ? 1 : 0));
        //uv_pipe_init(&this->_loop, &this->_pipe_stdout, (ENABLE_IPC == true ? 1 : 0));
        //uv_pipe_init(&this->_loop, &this->_pipe_stderr, (ENABLE_IPC == true ? 1 : 0));

        //this->_options.stdio_count = STDIO_SIZE;
        //this->_options.stdio = &this->_stdios[0];

        //_stdios[STANDARD_INPUT_FD].flags  = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
        //_stdios[STANDARD_OUTPUT_FD].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
        //_stdios[STANDARD_ERROR_FD].flags  = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
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

    void setEnvironmentVariables(std::vector<std::string> const & envs) {
        this->_envs = envs;
        this->updateEnvironmentVariablePointer();
    }

    void setWorkingDirectory(std::string const & path) {
        this->_working_directory = path;
        this->_options.cwd = this->_working_directory.c_str();
    }

    /**
     * @remarks
     *  Setting @c uv_process_options_t.flags to a bitwise OR of the following flags, @n
     *  modifies the child process behaviour:
     *  - @c UV_PROCESS_SETUID @n
     *    sets the child’s execution user ID to @c uv_process_options_t.uid.
     *  - @c UV_PROCESS_SETGID @n
     *    sets the child’s execution group ID to @c uv_process_options_t.gid.
     *
     *  Changing the UID/GID is only supported on Unix, @n
     *  @c uv_spawn will fail on Windows with @c UV_ENOTSUP.
     *  - @c UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS @n
     *    No quoting or escaping of @c uv_process_options_t.args is done on Windows. @n
     *    Ignored on Unix.
     *  - @c UV_PROCESS_DETACHED @n
     *    Starts the child process in a new session, @n
     *    which will keep running after the parent process exits.
     *
     *  Passing the flag @c UV_PROCESS_DETACHED can be used to launch daemons, @n
     *  or child processes which are independent of the parent so that @n
     *  the parent exiting does not affect it.
     *  - Just remember that the handle is still monitoring the child, @n
     *    so your program won’t exit. Use @c uv_unref() @n
     *    if you want to be more fire-and-forget.
     */
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

