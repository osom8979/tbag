/**
 * @file   UvEventHandler.cpp
 * @brief  UvEventHandler class implementation.
 * @author zer0
 * @date   2016-10-16
 */

#include <libtbag/loop/event/UvEventHandler.hpp>
#include <libtbag/pattern/Singleton.hpp>
#include <libtbag/lock/RwLock.hpp>
#include <libtbag/predef.hpp>
#include <cassert>

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
class UvEventManager : SINGLETON_INHERITANCE(UvEventManager)
{
public:
    SINGLETON_RESTRICT(UvEventManager);

public:
    using UvEventHandler = libtbag::container::Pointer<libtbag::loop::event::UvEventHandler>;

#if defined(__OS_MACOS__) && !defined(NDEBUG)
    using EventHandlerSet = std::set<UvEventHandler>;
#else
    using EventHandlerSet = std::unordered_set<UvEventHandler, UvEventHandler::Hash>;
#endif

    using RwLock     = libtbag::lock::RwLock;
    using WriteGuard = libtbag::lock::WriteLockGuard;
    using ReadGuard  = libtbag::lock::ReadLockGuard;

private:
    mutable RwLock  _rwlock;
    EventHandlerSet _handlers;

public:
    void add(UvEventHandler handler)
    {
        WriteGuard guard(_rwlock);
        _handlers.insert(handler);
    }

    void remove(UvEventHandler handler)
    {
        WriteGuard guard(_rwlock);
        _handlers.erase(handler);
    }

    bool exists(UvEventHandler handler) const
    {
        ReadGuard guard(_rwlock);
        return _handlers.find(handler) != _handlers.end();
    }

    UvEventHandler get(uv_handle_t * handle) const
    {
        ReadGuard guard(_rwlock);
        for (auto & cursor : _handlers) {
            if (static_cast<bool>(cursor) && cursor->exists(handle)) {
                return cursor;
            }
        }
        return UvEventHandler();
    }
};

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

// ------------------------------
// UvEventHandler implementation.
// ------------------------------

UvEventHandler::UvEventHandler()
{
    using UvEventManager = libtbag::loop::event::uv::UvEventManager;
    UvEventManager * em = UvEventManager::getInstance();
    assert(em != nullptr);
    em->add(this);
}

UvEventHandler::~UvEventHandler()
{
    using UvEventManager = libtbag::loop::event::uv::UvEventManager;
    UvEventManager * em = UvEventManager::getInstance();
    assert(em != nullptr);
    em->remove(this);
}

void UvEventHandler::add(void * handle)
{
    _handles.insert(UvHandle(handle));
}

void UvEventHandler::remove(void * handle)
{
    _handles.erase(UvHandle(handle));
}

bool UvEventHandler::exists(void * handle) const
{
    return _handles.find(UvHandle(handle)) != _handles.end();
}

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

