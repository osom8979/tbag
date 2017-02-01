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

namespace uvpp {

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

#define TBAG_UV_EVNET_IMPLEMENT_PARAM0(name, method, mgr, handle)   \
    void name(handle h) {                                           \
        auto c = mgr::getInstance()->get(h);                        \
        if (static_cast<bool>(c)) { c->name(h); }                   \
    }

#define TBAG_UV_EVNET_IMPLEMENT_PARAM1(name, method, mgr, handle, p1)   \
    void name(handle h, p1 a1) {                                        \
        auto c = mgr::getInstance()->get(h);                            \
        if (static_cast<bool>(c)) { c->name(h, a1); }                   \
    }

#define TBAG_UV_EVNET_IMPLEMENT_PARAM2(name, method, mgr, handle, p1, p2)   \
    void name(handle h, p1 a1, p2 a2) {                                     \
        auto c = mgr::getInstance()->get(h);                                \
        if (static_cast<bool>(c)) { c->name(h, a1, a2); }                   \
    }

#define TBAG_UV_EVNET_IMPLEMENT_PARAM3(name, method, mgr, handle, p1, p2, p3)   \
    void name(handle h, p1 a1, p2 a2, p3 a3) {                                  \
        auto c = mgr::getInstance()->get(h);                                    \
        if (static_cast<bool>(c)) { c->name(h, a1, a2, a3); }                   \
    }
#endif // TBAG_UV_EVNET_IMPLEMENT

/**
 * @defgroup __DOXYGEN_GROUP__UV_EVENT_FUNCTION__ libuv event function.
 * @brief libuv event function.
 * @{
 */

TBAG_UV_EVNET_IMPLEMENT_PARAM2(onAlloc      , onAlloc      , UvEventManager, void *, size_t, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM2(onRead       , onRead       , UvEventManager, void *, ssize_t, void const *);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onWrite      , onWrite      , UvEventManager, void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onConnect    , onConnect    , UvEventManager, void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onShutdown   , onShutdown   , UvEventManager, void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onConnection , onConnection , UvEventManager, void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onClose      , onClose      , UvEventManager, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM2(onPoll       , onPoll       , UvEventManager, void *, int, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onTimer      , onTimer      , UvEventManager, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onAsync      , onAsync      , UvEventManager, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onPrepare    , onPrepare    , UvEventManager, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onCheck      , onCheck      , UvEventManager, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onIdle       , onIdle       , UvEventManager, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM2(onExit       , onExit       , UvEventManager, void *, int64_t, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onWalk       , onWalk       , UvEventManager, void *, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onFs         , onFs         , UvEventManager, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM0(onWork       , onWork       , UvEventManager, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM1(onAfterWork  , onAfterWork  , UvEventManager, void *, int);
TBAG_UV_EVNET_IMPLEMENT_PARAM2(onGetaddrinfo, onGetaddrinfo, UvEventManager, void *, int, void *);
TBAG_UV_EVNET_IMPLEMENT_PARAM3(onGetnameinfo, onGetnameinfo, UvEventManager, void *, int, char const *, char const *);

/**
 * @}
 */

} // namespace uvpp

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
    using UvEventManager = libtbag::loop::event::uvpp::UvEventManager;
    UvEventManager * em = UvEventManager::getInstance();
    assert(em != nullptr);

    for (auto & cursor : _handles) {
        em->remove(cursor.get());
    }
    _handles.clear();
}

bool UvHandler::add(void * h)
{
    using UvEventManager = libtbag::loop::event::uvpp::UvEventManager;
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
    using UvEventManager = libtbag::loop::event::uvpp::UvEventManager;
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

