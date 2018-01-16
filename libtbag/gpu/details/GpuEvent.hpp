/**
 * @file   GpuEvent.hpp
 * @brief  GpuEvent class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUEVENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUEVENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class GpuContext;
class GpuStream;

/**
 * GpuEvent class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuEvent
{
private:
    GpuContext const * _context;
    GpuId _start;
    GpuId _stop;

public:
    GpuEvent(GpuContext const * c = nullptr, GpuId start = UNKNOWN_ID, GpuId stop = UNKNOWN_ID);
    GpuEvent(GpuEvent const & obj);
    GpuEvent(GpuEvent && obj);
    virtual ~GpuEvent();

public:
    GpuEvent & operator =(GpuEvent const & obj);
    GpuEvent & operator =(GpuEvent && obj);

public:
    void swap(GpuEvent & obj);
    inline friend void swap(GpuEvent & lh, GpuEvent & rh) { lh.swap(rh); }

public:
    inline bool isSameContext(GpuContext const * c) const TBAG_NOEXCEPT { return _context == c; }
    inline GpuContext const * getContextPtr() const TBAG_NOEXCEPT { return _context; }

    inline bool      existsId() const TBAG_NOEXCEPT { return _start != UNKNOWN_ID; }
    inline GpuId        getId() const TBAG_NOEXCEPT { return _start; }
    inline GpuId       & atId()       TBAG_NOEXCEPT { return _start; }
    inline GpuId const & atId() const TBAG_NOEXCEPT { return _start; }

    template <typename T> inline void    setId(T i) TBAG_NOEXCEPT { _start = (GpuId)i; }
    template <typename T> inline void setStart(T i) TBAG_NOEXCEPT { _start = (GpuId)i; }
    template <typename T> inline void  setStop(T i) TBAG_NOEXCEPT {  _stop = (GpuId)i; }

    inline GpuId getStart() const TBAG_NOEXCEPT { return _start; }
    inline GpuId  getStop() const TBAG_NOEXCEPT { return  _stop; }

    template <typename T> inline T    castId() const TBAG_NOEXCEPT { return (T)_start; }
    template <typename T> inline T castStart() const TBAG_NOEXCEPT { return (T)_start; }
    template <typename T> inline T  castStop() const TBAG_NOEXCEPT { return (T) _stop; }

    inline void clearIds() TBAG_NOEXCEPT { _start = UNKNOWN_ID; _stop = UNKNOWN_ID; }

    inline bool validate() const TBAG_NOEXCEPT
    { return (_context != nullptr) && existsId(); }
    inline bool validate(GpuContext const * c) const TBAG_NOEXCEPT
    { return (_context != nullptr) && isSameContext(c) && existsId(); }

public:
    Err create(GpuStream const & stream);
    Err release();

public:
    Err sync();
    Err elapsed(float * millisec);
    float elapsed();

public:
    static GpuEvent instance(GpuStream const & stream);
};

/**
 * CpuEventGuard class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API CpuEventGuard : private Noncopyable
{
private:
    GpuEvent * _event;

public:
    CpuEventGuard(GpuEvent * e = nullptr);
    ~CpuEventGuard();

public:
    static GpuId nowNano();
};

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUEVENT_HPP__

