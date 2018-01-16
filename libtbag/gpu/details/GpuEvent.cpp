/**
 * @file   GpuEvent.cpp
 * @brief  GpuEvent class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuEvent.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// ------------------------
// GpuEvent implementation.
// ------------------------

GpuEvent::GpuEvent(GpuContext const * c, GpuStream const * s, GpuId start, GpuId stop)
        : _context(c), _stream(s), _start(start), _stop(stop)
{
    // EMPTY.
}

GpuEvent::GpuEvent(GpuEvent const & obj) : GpuEvent()
{
    (*this) = obj;
}

GpuEvent::GpuEvent(GpuEvent && obj) : GpuEvent()
{
    (*this) = std::move(obj);
}

GpuEvent::~GpuEvent()
{
    // EMPTY.
}

GpuEvent & GpuEvent::operator =(GpuEvent const & obj)
{
    if (this != &obj) {
        _context = obj._context;
        _stream  = obj._stream;
        _start   = obj._start;
        _stop    = obj._stop;
    }
    return *this;
}

GpuEvent & GpuEvent::operator =(GpuEvent && obj)
{
    swap(obj);
    return *this;
}

void GpuEvent::swap(GpuEvent & obj)
{
    if (this != &obj) {
        std::swap(_context, obj._context);
        std::swap(_stream , obj._stream);
        std::swap(_start  , obj._start);
        std::swap(_stop   , obj._stop);
    }
}

bool GpuEvent::create()
{
    return (_context != nullptr && _stream != nullptr ? _context->createEvent(*_stream, *this) : false);
}

bool GpuEvent::release()
{
    return (_context != nullptr ? _context->releaseEvent(*this) : false);
}

bool GpuEvent::sync()
{
    return (_context != nullptr ? _context->syncEvent(*this) : false);
}

bool GpuEvent::elapsed(float * millisec)
{
    return (_context != nullptr ? _context->elapsedEvent(*this, millisec) : false);
}

float GpuEvent::elapsed()
{
    float millisec = 0;
    if (elapsed(&millisec)) {
        return millisec;
    }
    return 0;
}

GpuEvent GpuEvent::create(GpuStream const * s)
{
    if (s == nullptr) {
        return GpuEvent();
    }

    GpuEvent event(s->getContextPtr(), s);
    if (event.create()) {
        return event;
    }
    return GpuEvent();
}

// -----------------------------
// CpuEventGuard implementation.
// -----------------------------

CpuEventGuard::CpuEventGuard(GpuEvent * e) : _event(e)
{
    if (_event != nullptr) {
        _event->setStart(nowNano());
    }
}

CpuEventGuard::~CpuEventGuard()
{
    if (_event != nullptr) {
        _event->setStop(nowNano());
    }
}

GpuId CpuEventGuard::nowNano()
{
    using namespace std::chrono;
    return (GpuId)duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

