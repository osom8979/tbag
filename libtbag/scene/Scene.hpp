/**
 * @file   Scene.hpp
 * @brief  Scene class prototype.
 * @author zer0
 * @date   2019-05-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCENE_SCENE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCENE_SCENE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace scene {

// Forward declaration.

/**
 * Scene class prototype.
 *
 * @author zer0
 * @date   2019-05-29
 */
class TBAG_API Scene
{
private:
    // Insert member variables.

public:
    Scene();
    Scene(Scene const & obj);
    Scene(Scene && obj) TBAG_NOEXCEPT;
    ~Scene();

public:
    Scene & operator =(Scene const & obj);
    Scene & operator =(Scene && obj) TBAG_NOEXCEPT;

public:
    void copy(Scene const & obj);
    void swap(Scene & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Scene & lh, Scene & rh) TBAG_NOEXCEPT { lh.swap(rh); }
};

} // namespace scene

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCENE_SCENE_HPP__

