/**
 * @file   OfstreamSink.hpp
 * @brief  OfstreamSink class prototype.
 * @author zer0
 * @date   2016-07-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_OFSTREAMSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_OFSTREAMSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <fstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * StreamSink class prototype.
 *
 * @author zer0
 * @date   2016-07-10
 */
template <typename MutexType = lock::FakeLock>
class OfstreamSink : public Sink<MutexType>, public Noncopyable
{
public:
    using Parent   = Sink<MutexType>;
    using Ofstream = std::ofstream;

public:
    Ofstream _ofstream;

public:
    OfstreamSink(std::string const & path, bool force_flush = false)
            : Parent(force_flush), _ofstream(path /*, std::ios_base::binary | std::ios_base::out */)
    { /* EMPTY. */ }

    virtual ~OfstreamSink()
    {
        if (_ofstream.is_open() == true) {
            _ofstream.close();
        }
    }

public:
    virtual void writeReal(std::string const & msg) override
    {
        _ofstream.write(msg.c_str(), msg.size());
    }

    virtual void flushReal() override
    {
        _ofstream.flush();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_OFSTREAMSINK_HPP__

