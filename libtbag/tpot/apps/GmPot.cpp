/**
 * @file   GmPot.cpp
 * @brief  GmPot class implementation.
 * @author zer0
 * @date   2019-01-30
 */

#include <libtbag/tpot/apps/GmPot.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/gui/SfWindow.hpp>
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
struct GmPotImpl : public libtbag::gui::SfWindow
{
    enum class SceneMode
    {
        SM_NORMAL,
        SM_TEST,
    };

    SceneMode mode = SceneMode::SM_NORMAL;

    GmPotImpl()
    {
        // EMPTY.
    }

    ~GmPotImpl()
    {
        // EMPTY.
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
        // EMPTY.
    }

    virtual void onKeyReleased(Key code, bool alt, bool control, bool shift, bool system) override
    {
        if (control && code == Key::F1) {
            if (mode == SceneMode::SM_NORMAL) {
                mode = SceneMode::SM_TEST;
            } else {
                mode = SceneMode::SM_NORMAL;
            }
        }
    }

    virtual void onIdle() override
    {
        if (mode == SceneMode::SM_NORMAL) {
            onNormalIdle();
        } else {
            assert(mode == SceneMode::SM_TEST);
            onTestIdle();
        }
    }

    void onNormalIdle()
    {
        clear();
        display();
    }

    void onTestIdle()
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

