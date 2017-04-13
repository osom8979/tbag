/**
 * @file   Sink.hpp
 * @brief  Sink class prototype.
 * @author zer0
 * @date   2016-07-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/lock/FakeLock.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

struct SinkInterface
{
    using String = std::string;

    virtual void safeWrite(String const & message) = 0;
    virtual void safeFlush() = 0;

    virtual void write(String const & message) = 0;
    virtual void flush() = 0;
};

/**
 * Base Sink interface.
 *
 * @author zer0
 * @date   2016-07-08
 */
template <typename MutexType = lock::FakeLock>
class Sink : public SinkInterface, public Noncopyable
{
public:
    using String = SinkInterface::String;
    using Mutex  = MutexType;

public:
    TBAG_CONSTEXPR static char const * const WINDOWS_NEW_LINE = "\r\n";
    TBAG_CONSTEXPR static char const * const    UNIX_NEW_LINE = "\n";

private:
    Mutex  _mutex;
    bool   _force_flush;
    String _endl;

public:
    Sink() : _mutex(), _force_flush(false), _endl(UNIX_NEW_LINE)
    { /* EMPTY. */ }
    Sink(bool force_flush) : _mutex(), _force_flush(force_flush), _endl(UNIX_NEW_LINE)
    { /* EMPTY. */ }
    virtual ~Sink()
    { /* EMPTY. */ }

public:
    // @formatter:off
    inline void setNewLineForUnixStyle()
    { _endl = UNIX_NEW_LINE; }
    inline void setNewLineForWindowsStyle()
    { _endl = WINDOWS_NEW_LINE; }
    // @formatter:on

public:
    virtual void safeWrite(String const & message) override
    {
        _mutex.lock();
        this->write(message + _endl);
        if (_force_flush) {
            this->flush();
        }
        _mutex.unlock();
    }

    virtual void safeFlush() override
    {
        _mutex.lock();
        this->flush();
        _mutex.unlock();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__

