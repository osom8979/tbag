/**
 * @file   Logger.cpp
 * @brief  Logger class implementation.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Move namespace: log/mgr -> log)
 */

#include <libtbag/log/Logger.hpp>
#include <libtbag/util/BufferInfo.hpp>

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

void Logger::flush() const
{
    assert(static_cast<bool>(_sink));
    _sink->flush();
}

bool Logger::write(int level, char const * level_name, char const * message, int size, bool generate) const
{
    if (youShallNotPass(level)) {
        return true;
    }
    if (!_sink) {
        return false;
    }

    assert(level_name != nullptr);
    assert(message != nullptr);
    assert(size >= 1);

    bool write_result;
    if (generate && _generator) {
        std::string buffer;
        _generator->make(NAME.c_str(), level, level_name, message, size, buffer);
        write_result = _sink->write(level, buffer.c_str(), buffer.size());
    } else {
        write_result = _sink->write(level, message, size);
    }

    if (write_result && _auto_flush) {
        _sink->flush();
    }

    return write_result;
}

bool Logger::write(int level, char const * level_name, char const * message, int size) const
{
    return write(level, level_name, message, size, static_cast<bool>(_generator));
}

bool Logger::write(int level, char const * level_name, char const * message) const
{
    return write(level, level_name, message, ::strlen(message));
}

bool Logger::write(int level, char const * level_name, std::string const & message) const
{
    return write(level, level_name, message.c_str(), static_cast<int>(message.size()));
}

bool Logger::write(int level, char const * message, int size) const
{
    return write(level, getTextWithLevelStep(level), message, size);
}

bool Logger::write(int level, char const * message) const
{
    return write(level, getTextWithLevelStep(level), message);
}

bool Logger::write(int level, std::string const & message) const
{
    return write(level, getTextWithLevelStep(level), message);
}

bool Logger::write(Severity const & severity, char const * message, int size) const
{
    return write(severity.getLevel(), severity.getText(), message, size);
}

bool Logger::write(Severity const & severity, char const * message) const
{
    return write(severity, message, ::strlen(message));
}

bool Logger::write(Severity const & severity, std::string const & message) const
{
    return write(severity, message.c_str(), message.size());
}

bool Logger::raw(Severity const & severity, char const * message, int size) const
{
    return write(severity.getLevel(), severity.getText(), message, size, false);
}

bool Logger::raw(Severity const & severity, char const * message) const
{
    return raw(severity, message, ::strlen(message));
}

bool Logger::raw(Severity const & severity, std::string const & message) const
{
    return raw(severity, message.c_str(), message.size());
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

