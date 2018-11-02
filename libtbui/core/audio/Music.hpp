/**
 * @file   Music.hpp
 * @brief  Music class prototype.
 * @author zer0
 * @date   2018-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBUI_CORE_AUDIO_MUSIC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBUI_CORE_AUDIO_MUSIC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbui/export.h>

#include <memory>

namespace libtbui {
namespace core    {
namespace audio   {

/**
 * Music class prototype.
 *
 * @author zer0
 * @date   2018-11-03
 */
class TBUI_API Music
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    Music();
    Music(std::nullptr_t) TBAG_NOEXCEPT;
    Music(Music const & obj) TBAG_NOEXCEPT;
    Music(Music && obj) TBAG_NOEXCEPT;
    ~Music();

public:
    Music & operator =(Music const & obj) TBAG_NOEXCEPT;
    Music & operator =(Music && obj) TBAG_NOEXCEPT;

public:
    void copy(Music const & obj) TBAG_NOEXCEPT;
    void swap(Music & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Music & lh, Music & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(Music const & x, Music const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(Music const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(Music const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();
};

} // namespace audio
} // namespace core
} // namespace libtbui

#endif // __INCLUDE_LIBTBAG__LIBTBUI_CORE_AUDIO_MUSIC_HPP__

