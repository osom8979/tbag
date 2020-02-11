/**
 * @file   TmxAnimation.hpp
 * @brief  TmxAnimation class prototype.
 * @author zer0
 * @date   2019-07-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXANIMATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXANIMATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/tiled/details/TmxTag.hpp>
#include <libtbag/tiled/details/TmxFrame.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxAnimation class prototype.
 *
 * @author zer0
 * @date   2019-07-23
 */
struct TBAG_API TmxAnimation : public TmxTag
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "animation";

    using Frames = std::vector<TmxFrame>;

    Frames frames;

    TmxAnimation();
    TmxAnimation(Frames const & f);
    ~TmxAnimation();

    using       iterator = typename Frames::iterator;
    using const_iterator = typename Frames::const_iterator;
    using       reverse_iterator = typename Frames::reverse_iterator;
    using const_reverse_iterator = typename Frames::const_reverse_iterator;

    iterator begin() TBAG_NOEXCEPT_SP_OP(frames.begin())
    { return frames.begin(); }
    iterator end() TBAG_NOEXCEPT_SP_OP(frames.end())
    { return frames.end(); }

    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(frames.begin())
    { return frames.begin(); }
    const_iterator end() const TBAG_NOEXCEPT_SP_OP(frames.end())
    { return frames.end(); }

    const_iterator cbegin() const TBAG_NOEXCEPT_SP_OP(frames.cbegin())
    { return frames.cbegin(); }
    const_iterator cend() const TBAG_NOEXCEPT_SP_OP(frames.cend())
    { return frames.cend(); }

    reverse_iterator rbegin() TBAG_NOEXCEPT_SP_OP(frames.rbegin())
    { return frames.rbegin(); }
    reverse_iterator rend() TBAG_NOEXCEPT_SP_OP(frames.rend())
    { return frames.rend(); }

    const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(frames.rbegin())
    { return frames.rbegin(); }
    const_reverse_iterator rend() const TBAG_NOEXCEPT_SP_OP(frames.rend())
    { return frames.rend(); }

    const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(frames.crbegin())
    { return frames.crbegin(); }
    const_reverse_iterator crend() const TBAG_NOEXCEPT_SP_OP(frames.crend())
    { return frames.crend(); }

    bool empty() const;
    std::size_t size() const;
    void clear();

    virtual Err onRead(Element const & elem) override;
    virtual Err onWrite(Element & elem) const override;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXANIMATION_HPP__

