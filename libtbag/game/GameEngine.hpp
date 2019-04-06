/**
 * @file   GameEngine.hpp
 * @brief  GameEngine class prototype.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 * @date   2019-03-23 (Rename: RenderWindow -> Window)
 * @date   2019-04-06 (Rename: Window -> GameEngine)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMEENGINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMEENGINE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/game/GameCommon.hpp>
#include <libtbag/geometry/GeometryTypes.hpp>
#include <libtbag/graphic/Color.hpp>
#include <libtbag/res/Storage.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

TBAG_CONSTEXPR int const GAME_EXIT_CODE_EXIT_SUCCESS = 1;
TBAG_CONSTEXPR int const GAME_EXIT_CODE_EXIT_FAILURE = 2;
TBAG_CONSTEXPR int const GAME_EXIT_CODE_RESTART = 3;

/**
 * GameEngine class prototype.
 *
 * @author zer0
 * @date   2019-04-06
 */
class TBAG_API GameEngine : private Noncopyable,
                            public GameInterface,
                            public libtbag::geometry::GeometryTypes
{
public:
    using Storage = libtbag::res::Storage;
    using Channel = libtbag::graphic::Channel;
    using Rgb24   = libtbag::graphic::Rgb24;
    using Rgb32   = libtbag::graphic::Rgb32;

public:
    TBAG_CONSTEXPR static Channel const CHANNEL_MAX  = libtbag::graphic::channel_max();
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN  = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_HALF = libtbag::graphic::channel_half();

public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl  _impl;
    Storage     _storage;
    GameParams  _params;

public:
    GameEngine(Storage & storage, GameParams const & params);
    virtual ~GameEngine();

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

public:
    inline Impl * get() TBAG_NOEXCEPT_SP_OP(_impl.get())
    { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT_SP_OP(_impl.get())
    { return _impl.get(); }

public:
    inline Storage & storage() TBAG_NOEXCEPT
    { return _storage; }
    inline Storage const & storage() const TBAG_NOEXCEPT
    { return _storage; }

public:
    virtual void closeGame() override;

public:
    int run();
};

/**
 * GameScene class prototype.
 *
 * @author zer0
 * @date   2019-04-06
 */
class TBAG_API GameScene : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    GameScene(GameEngine * engine);
    virtual ~GameScene();

public:
    // clang-format off
    virtual void onStart () { /* EMPTY. */ }
    virtual void onUpdate(double elapsed) { /* EMPTY. */ }
    virtual void onLeave () { /* EMPTY. */ }
    virtual void onExit  () { /* EMPTY. */ }
    virtual void onEnter () { /* EMPTY. */ }
    virtual void onResume() { /* EMPTY. */ }
    virtual void onDraw  () { /* EMPTY. */ }
    virtual void onEnd   () { /* EMPTY. */ }
    // clang-format on
};

TBAG_API int runGame(libtbag::res::Storage & storage);
TBAG_API int runGame();

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMEENGINE_HPP__

