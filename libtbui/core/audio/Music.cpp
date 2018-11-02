/**
 * @file   Music.cpp
 * @brief  Music class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbui/core/audio/Music.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

namespace libtbui {
namespace core    {
namespace audio   {

/**
 * Music::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-03
 */
struct Music::Impl : private libtbag::Noncopyable
{
private:
    Music * _parent;

public:
    Impl(Music * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ---------------------
// Music implementation.
// ---------------------

Music::Music() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

Music::Music(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Music::Music(Music const & obj) TBAG_NOEXCEPT : Music(nullptr)
{
    (*this) = obj;
}

Music::Music(Music && obj) TBAG_NOEXCEPT : Music(nullptr)
{
    (*this) = std::move(obj);
}

Music::~Music()
{
    // EMPTY.
}

Music & Music::operator =(Music const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Music & Music::operator =(Music && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Music::copy(Music const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void Music::swap(Music & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void Music::reset()
{
    _impl.reset();
}

} // namespace audio
} // namespace core
} // namespace libtbui

