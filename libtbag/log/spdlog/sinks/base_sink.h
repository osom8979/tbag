//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once
//
// base sink templated over a mutex (either dummy or realy)
// concrete implementation should only overrid the _sink_it method.
// all locking is taken care of here so no locking needed by the implementors..
//

#include <libtbag/log/spdlog/sinks/sink.h>
#include <libtbag/log/spdlog/formatter.h>
#include <libtbag/log/spdlog/common.h>
#include <libtbag/log/spdlog/details/log_msg.h>

#include <mutex>

namespace spdlog
{
namespace sinks
{
template<class Mutex>
class base_sink:public sink
{
public:
    base_sink():_mutex() {}
    virtual ~base_sink() = default;

    base_sink(const base_sink&) = delete;
    base_sink& operator=(const base_sink&) = delete;

    void log(const details::log_msg& msg) override
    {
        std::lock_guard<Mutex> lock(_mutex);
        _sink_it(msg);
    }

protected:
    virtual void _sink_it(const details::log_msg& msg) = 0;
    Mutex _mutex;
};
}
}
