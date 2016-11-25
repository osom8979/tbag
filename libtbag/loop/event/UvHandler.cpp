/**
 * @file   UvHandler.cpp
 * @brief  libuv event handling class implementation.
 * @author zer0
 * @date   2016-10-16
 */

#include <libtbag/loop/event/UvHandler.hpp>
#include <libtbag/loop/event/UvHandlerManager.hpp>
#include <libtbag/pattern/Singleton2.hpp>

#include <cassert>
#include <algorithm>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop  {
namespace event {

namespace uv {

/**
 * UvEventManager class prototype.
 *
 * @author zer0
 * @date   2016-05-18
 * @date   2016-10-16 (Restore this class)
 */
class UvEventManager : public UvHandlerManager
{ SINGLETON2_PROTOTYPE(UvEventManager); };
SINGLETON2_IMPLEMENT(UvEventManager);

#ifndef TBAG_UV_EVNET_IMPLEMENT
#define TBAG_UV_EVNET_IMPLEMENT

#define TBAG_UV_EVNET_IMPLEMENT_PARAM0(name, p0)                        \
    void name(p0 a0) {                                                  \
        auto c = UvEventManager::getInstance()->get((uv_handle_t*)a0);  \
        if (static_cast<bool>(c)) { c->name(a0); }                      \
    }

#define TBAG_UV_EVNET_IMPLEMENT_PARAM1(name, p0, p1)                    \
    void name(p0 a0, p1 a1) {                                           \
        auto c = UvEventManager::getInstance()->get((uv_handle_t*)a0);  \
        if (static_cast<bool>(c)) { c->name(a0, a1); }                  \
    }

#define TBAG_UV_EVNET_IMPLEMENT_PARAM2(name, p0, p1, p2)                \
    void name(p0 a0, p1 a1, p2 a2) {                                    \
        auto c = UvEventManager::getInstance()->get((uv_handle_t*)a0);  \
        if (static_cast<bool>(c)) { c->name(a0, a1, a2); }              \
    }

#define TBAG_UV_EVNET_IMPLEMENT_PARAM3(name, p0, p1, p2, p3)            \
    void name(p0 a0, p1 a1, p2 a2, p3 a3) {                             \
        auto c = UvEventManager::getInstance()->get((uv_handle_t*)a0);  \
        if (static_cast<bool>(c)) { c->name(a0, a1, a2, a3); }          \
    }
#endif // TBAG_UV_EVNET_IMPLEMENT

/**
 * @defgroup __DOXYGEN_GROUP__UV_EVENT_FUNCTION__ libuv event function.
 * @brief libuv event function.
 * @{
 */

TBAG_UV_EVNET_IMPLEMENT_PARAM2(onAlloc      , void *, size_t, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM2(onRead       , void *, ssize_t, void const *);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onWrite      , void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onConnect    , void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onShutdown   , void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onConnection , void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onClose      , void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM2(onPoll       , void *, int, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onTimer      , void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onAsync      , void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onPrepare    , void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onCheck      , void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onIdle       , void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM2(onExit       , void *, int64_t, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onWalk       , void *, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onFs         , void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onWork       , void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onAfterWork  , void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM2(onGetaddrinfo, void *, int, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM3(onGetnameinfo, void *, int, char const *, char const *);

/**
 * @}
 */

} // namespace uv

// -------------------------
// UvHandler implementation.
// -------------------------

UvHandler::UvHandler()
{
    // EMPTY.
}

UvHandler::UvHandler(void * h)
{
    add(h);
}

UvHandler::~UvHandler()
{
    clear();
}

void UvHandler::clear()
{
    using UvEventManager = libtbag::loop::event::uv::UvEventManager;
    UvEventManager * em = UvEventManager::getInstance();
    assert(em != nullptr);

    for (auto & cursor : _handles) {
        em->remove(cursor.get());
    }
    _handles.clear();
}

bool UvHandler::add(void * h)
{
    using UvEventManager = libtbag::loop::event::uv::UvEventManager;
    UvEventManager * em = UvEventManager::getInstance();
    assert(em != nullptr);

    auto find_itr = std::find(_handles.begin(), _handles.end(), Handle(h));
    if (find_itr != _handles.end()) {
        // Exists element error.
        return false;
    }

    em->add(h, this);
    _handles.push_back(Handle(h));
    return true;
}

bool UvHandler::remove(void * h)
{
    using UvEventManager = libtbag::loop::event::uv::UvEventManager;
    UvEventManager * em = UvEventManager::getInstance();
    assert(em != nullptr);

    auto find_itr = std::find(_handles.begin(), _handles.end(), Handle(h));
    if (find_itr == _handles.end()) {
        // Not found error.
        return false;
    }

    em->remove(h);
    _handles.erase(find_itr);
    return true;
}

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

