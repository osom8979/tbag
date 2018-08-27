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
#include <libtbag/string/StringUtils.hpp>

#include <string>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

struct SinkInterface
{
    using String = std::string;

    SinkInterface() { /* EMPTY. */ }
    virtual ~SinkInterface() { /* EMPTY. */ }

    virtual char const * const name() const = 0;

    virtual void safeWrite(String const & message) = 0;
    virtual void safeFlush() = 0;

    virtual void write(String const & message) = 0;
    virtual void flush() = 0;

    virtual void set(std::string const & key, std::string const & val) = 0;
    virtual std::string get(std::string const & key) const = 0;
};

/**
 * Base Sink interface.
 *
 * @author zer0
 * @date   2016-07-08
 */
template <typename MutexType = lock::FakeLock>
class Sink : public SinkInterface
{
public:
    using String = SinkInterface::String;
    using Mutex  = MutexType;

private:
    Mutex  _mutex;
    bool   _force_flush;

public:
    TBAG_CONSTEXPR static char const * const TYPE_NAME = "SINK";

public:
    TBAG_CONSTEXPR inline bool isLocking() const TBAG_NOEXCEPT
    { return std::is_same<MutexType, lock::FakeLock>::value; }

public:
    Sink() : _mutex(), _force_flush(false)
    { /* EMPTY. */ }
    Sink(bool force_flush) : _mutex(), _force_flush(force_flush)
    { /* EMPTY. */ }
    virtual ~Sink()
    { /* EMPTY. */ }

public:
    inline bool isForceFlush() const TBAG_NOEXCEPT { return _force_flush; }

public:
    virtual char const * const name() const override
    {
        return TYPE_NAME;
    }

    virtual void safeWrite(String const & message) override
    {
        _mutex.lock();
        this->write(message);
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

    virtual void set(std::string const & key, std::string const & val) override
    {
        // EMPTY.
    }

    virtual std::string get(std::string const & key) const override
    {
        return std::string();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__

