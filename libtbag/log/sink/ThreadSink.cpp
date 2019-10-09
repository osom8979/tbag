/**
 * @file   ThreadSink.cpp
 * @brief  ThreadSink class implementation.
 * @author zer0
 * @date   2019-10-09
 */

#include <libtbag/log/sink/ThreadSink.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

ThreadSink::ThreadSink(SharedSink const & sink) : _sink(sink), _exit(false)
{
    auto const code = run();
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

ThreadSink::~ThreadSink()
{
    _lock.lock();
    _exit = true;
    _lock.unlock();
    join(false);
}

bool ThreadSink::write(int level, char const * message, int size)
{
    std::string temp(message, message + size);
    UvGuard const G(_lock);
    if (_exit) {
        return false;
    }
    _queue.emplace_back(level, std::move(temp));
    _condition.signal();
    return true;
}

void ThreadSink::flush()
{
    UvGuard const G(_lock);
    _queue.emplace_back(true);
    _condition.signal();
}

void ThreadSink::onRunner()
{
    bool is_exit = false;
    bool find_message = false;
    Message current;

    while (!is_exit) {
        _lock.lock();
        while (!_exit && _queue.empty()) {
            _condition.wait(_lock);
        }
        is_exit = _exit;
        if (_queue.empty()) {
            find_message = false;
        } else {
            find_message = true;
            current.level = _queue.front().level;
            current.message = std::move(_queue.front().message);
            current.flush = _queue.front().flush;
        }
        _lock.unlock();

        while (find_message) {
            if (!current.message.empty()) {
                _sink->write(current.level, current.message.c_str(), current.message.size());
            }
            if (current.flush) {
                _sink->flush();
            }

            _lock.lock();
            if (_queue.empty()) {
                find_message = false;
            } else {
                find_message = true;
                current.level = _queue.front().level;
                current.message = std::move(_queue.front().message);
                current.flush = _queue.front().flush;
            }
            _lock.unlock();
        }
    }
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

