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
#include <libtbag/loop/UvEventLoop.hpp>

#include <cstdlib>
#include <cassert>

#include <string>
#include <vector>
#include <array>
#include <numeric>
#include <functional>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * Process class prototype.
 *
 * @author zer0
 * @date   2016-05-17
 */
class Process : public loop::UvEventLoop
{
public:
    static constexpr int64_t const UNKNOWN_EXIT_CODE        = std::numeric_limits<int64_t>::min();
    static constexpr int     const UNKNOWN_TERMINATE_SIGNAL = std::numeric_limits<int    >::min();

private:
    std::atomic<int64_t> _exit_status;
    std::atomic<int> _terminate_signal;

protected:
    uv_process_t          _process;
    uv_process_options_t  _options;

private:
    std::string _execute_file;
    std::string _working_directory;

    std::vector<std::string>  _arguments;
    std::vector<char*>        _argument_pointers;

    std::vector<std::string>  _envs; ///< Environment variables.
    std::vector<char*>        _env_pointers;

public:
    Process() : _exit_status(UNKNOWN_EXIT_CODE), _terminate_signal(UNKNOWN_TERMINATE_SIGNAL) {
        memset((void*)&this->_options, 0x00, sizeof(this->_options));
        this->_options.exit_cb = &loop::uv_event::onExit;
        this->addHandle((uv_handle_t*)&this->_process);
    }

    virtual ~Process() {
        this->removeHandle((uv_handle_t*)&this->_process);
    }

protected:
    uv_process_options_t & atOptions() noexcept {
        return this->_options;
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

    void setFlags(unsigned int flags) {
        this->_options.flags = flags;
    }

public:
    bool spawn() {
        return (uv_spawn(this->getLoopPointer(), &this->_process, &this->_options) == 0 ? true : false);
    }

public:
    virtual bool exe() {
        if (this->spawn()) {
            return this->runDefault();
        }
        return false;
    }

public:
    void onExit(uv_process_t * process, int64_t exit_status, int term_signal) override {
        this->_exit_status.store(exit_status);
        this->_terminate_signal.store(term_signal);
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

