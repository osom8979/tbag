/**
 * @file   SignalRunner.cpp
 * @brief  SignalRunner class implementation.
 * @author zer0
 * @date   2019-09-11
 */

#include <libtbag/signal/SignalRunner.hpp>
#include <iostream>
#include <thread>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal {

SignalRunner::SignalRunner(Callback * cb, bool enable_stdout)
        : _signal(), _callback(cb), _enable_stdout(enable_stdout)
{
    // EMPTY.
}

SignalRunner::~SignalRunner()
{
    // EMPTY.
}

bool SignalRunner::run(int tick_ms)
{
    if (_callback) {
        auto setup_result = _callback->onSetUp();
        if (_enable_stdout) {
            std::cerr << "SetUp is failure." << std::endl;
        }
        if (!setup_result) {
            return false;
        }
    }

    std::atomic_bool exit_flag;
    auto const code1 = _signal.startInterrupt([&](){
        if (_enable_stdout) {
            std::cout << "Interrupt signal catch!" << std::endl;
        }
        exit_flag = true;
    });
    if (isSuccess(code1)) {
        if (_enable_stdout) {
            std::cout << "Interrupt signal detection started." << std::endl;
        }
    } else {
        if (_enable_stdout) {
            std::cerr << "Interrupt signal detection error: " << code1 << std::endl;
        }
        return false;
    }

    char const progress_char[] = "*|/-\\|/-\\";
    auto const progress_char_size = sizeof(progress_char) / sizeof(progress_char[0]) - 1;
    auto const tick = std::chrono::milliseconds(tick_ms);
    std::size_t loop_count = 0;

    if (_enable_stdout) {
        std::cout << "Now running ...  " << progress_char[0];
    }

    if (_callback) {
        _callback->onAsync();
    }

    while (!exit_flag) {
        if (_enable_stdout) {
            std::cout << "\b" << progress_char[loop_count%progress_char_size];
            std::cout.flush();
            ++loop_count;
        }
        std::this_thread::sleep_for(tick);
    }

    _signal.done();

    if (_callback) {
        _callback->onTearDown();
    }

    return true;
}

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

