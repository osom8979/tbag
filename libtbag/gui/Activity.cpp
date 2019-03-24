/**
 * @file   Activity.cpp
 * @brief  Activity class implementation.
 * @author zer0
 * @date   2019-03-24
 */

#include <libtbag/gui/Activity.hpp>
#include <libtbag/log/Log.hpp>

#include <Swoosh/ActivityController.h>
#include <Swoosh/Game.h>
#include <Swoosh/ActionList.h>
#include <Swoosh/Activity.h>
#include <Swoosh/Ease.h>
#include <Swoosh/EmbedGLSL.h>
#include <Swoosh/Segue.h>
#include <Swoosh/Shaders.h>
#include <Swoosh/Timer.h>

#include <SFML/Graphics.hpp>

#include <Segues/BlackWashFade.h>
#include <Segues/BlendFadeIn.h>
#include <Segues/BlurFadeIn.h>
#include <Segues/Checkerboard.h>
#include <Segues/CircleClose.h>
#include <Segues/CircleOpen.h>
#include <Segues/CrossZoom.h>
#include <Segues/Cube3D.h>
#include <Segues/DiamondTileCircle.h>
#include <Segues/DiamondTileSwipe.h>
#include <Segues/HorizontalOpen.h>
#include <Segues/HorizontalSlice.h>
#include <Segues/Morph.h>
#include <Segues/PageTurn.h>
#include <Segues/PixelateBlackWashFade.h>
#include <Segues/PushIn.h>
#include <Segues/RadialCCW.h>
#include <Segues/RetroBlit.h>
#include <Segues/SlideIn.h>
#include <Segues/SwipeIn.h>
#include <Segues/VerticalOpen.h>
#include <Segues/VerticalSlice.h>
#include <Segues/WhiteWashFade.h>
#include <Segues/ZoomFadeIn.h>
#include <Segues/ZoomFadeInBounce.h>
#include <Segues/ZoomIn.h>
#include <Segues/ZoomOut.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * Activity implementation.
 *
 * @author zer0
 * @date   2019-03-24
 */
struct Activity::Impl : public swoosh::Activity
{
    Impl(void * controller) : swoosh::Activity((swoosh::ActivityController*)controller)
    {
        // EMPTY.
    }

    virtual ~Impl()
    {
        // EMPTY.
    }

    virtual void onStart() override
    {
    }

    virtual void onUpdate(double elapsed) override
    {
    }

    virtual void onLeave() override
    {
    }

    virtual void onExit() override
    {
    }

    virtual void onEnter() override
    {
    }

    virtual void onResume() override
    {
    }

    virtual void onDraw(sf::RenderTexture & surface) override
    {
    }

    virtual void onEnd() override
    {
    }
};

// --------
// Activity
// --------

Activity::Activity()
{
    // EMPTY.
}

Activity::~Activity()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

