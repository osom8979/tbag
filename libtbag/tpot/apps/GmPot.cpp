/**
 * @file   GmPot.cpp
 * @brief  GmPot class implementation.
 * @author zer0
 * @date   2019-01-30
 */

#include <libtbag/tpot/apps/GmPot.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/graphic/Color.hpp>

#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

/**
 * GUI Window implementation.
 *
 * @author zer0
 * @date   2019-02-10
 */
struct GmPotImpl : public libtbag::gui::Window
{
    GmPotImpl()
    {
    }

    ~GmPotImpl()
    {
    }

    virtual void onBegin() override
    {
        setClearColor(libtbag::graphic::MIDNIGHT_BLUE_COLOR);
    }

    virtual void onEnd() override
    {
        // EMPTY.
    }

    virtual void onKeyPressed(Key code, bool alt, bool control, bool shift, bool system) override
    {
        tDLogD("onKeyPressed() Key: {}", (int)code);
    }

    virtual void onKeyReleased(Key code, bool alt, bool control, bool shift, bool system) override
    {
        tDLogD("onKeyReleased() Key: {}", (int)code);
    }

    virtual void onIdle() override
    {
        clear();
        display();
    }
};

// --------------------
// GmPot implementation
// --------------------

GmPot::GmPot()
{
    // EMPTY.
}

GmPot::~GmPot()
{
    // EMPTY.
}

char const * const GmPot::getName() const TBAG_NOEXCEPT
{
    return "gm";
}

char const * const GmPot::getRemarks() const TBAG_NOEXCEPT
{
    return "Game Maker";
}

int GmPot::onMain()
{
    return GmPotImpl().run();
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

