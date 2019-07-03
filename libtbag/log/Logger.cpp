/**
 * @file   Logger.cpp
 * @brief  Logger class implementation.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Move namespace: log/mgr -> log)
 */

#include <libtbag/log/Logger.hpp>
#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

Logger::Logger(std::string const & name,
               SharedSink const & sink,
               SharedGenerator const & generator,
               int level,
               bool auto_flush)
        : NAME(name), _sink(sink), _generator(generator), _level(level), _auto_flush(auto_flush)
{
    assert(static_cast<bool>(_sink));
}

Logger::~Logger()
{
    _sink->flush();
    _sink.reset();
    _generator.reset();
}

void Logger::flush()
{
    assert(static_cast<bool>(_sink));
    _sink->flush();
}

bool Logger::write(int level, char const * level_name, char const * message, int size)
{
    if (level <= _level) {
        return true;
    }

    assert(static_cast<bool>(_sink));
    assert(level_name != nullptr);
    assert(message != nullptr);
    assert(size >= 1);

    char buffer[BUFFER_SIZE];
    int write_buffer_size;
    if (_generator) {
        write_buffer_size = _generator->make(buffer, BUFFER_SIZE, NAME.c_str(), level, level_name, message, size);
    } else {
        write_buffer_size = 0;
    }

    bool write_result;
    if (write_buffer_size >= 1) {
        write_result = _sink->write(buffer, write_buffer_size);
    } else {
        write_result = _sink->write(message, size);
    }

    if (_auto_flush) {
        _sink->flush();
    }

    return write_result;
}

bool Logger::write(int level, char const * level_name, char const * message)
{
    return write(level, level_name, message, ::strlen(message));
}

bool Logger::write(int level, char const * level_name, std::string const & message)
{
    return write(level, level_name, message.c_str(), static_cast<int>(message.size()));
}

bool Logger::write(int level, char const * message, int size)
{
    return write(level, getTextWithLevelStep(level), message, size);
}

bool Logger::write(int level, char const * message)
{
    return write(level, getTextWithLevelStep(level), message);
}

bool Logger::write(int level, std::string const & message)
{
    return write(level, getTextWithLevelStep(level), message);
}

bool Logger::write(Severity const & severity, char const * message, int size)
{
    return write(severity.getLevel(), severity.getText(), message, size);
}

bool Logger::write(Severity const & severity, char const * message)
{
    return write(severity.getLevel(), severity.getText(), message);
}

bool Logger::write(Severity const & severity, std::string const & message)
{
    return write(severity.getLevel(), severity.getText(), message);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

