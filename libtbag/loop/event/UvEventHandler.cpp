/**
 * @file   UvEventHandler.cpp
 * @brief  UvEventHandler class implementation.
 * @author zer0
 * @date   2016-10-16
 */

#include <libtbag/loop/event/UvEventHandler.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/pattern/Singleton.hpp>
#include <libtbag/lock/RwLock.hpp>
#include <libtbag/predef.hpp>

#include <cassert>

#include <unordered_map>
#include <map>

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
    using Handle = libtbag::container::Pointer<void>;
    using Event  = libtbag::container::Pointer<UvHandler>;

#if defined(__OS_MACOS__) && !defined(NDEBUG)
    using EventHandlerSet = std::map<Handle, Event, Handle::Less>;
#else
    using EventHandlerSet = std::unordered_map<Handle, Event, Handle::Hash, Handle::EqualTo>;
#endif

    using RwLock     = libtbag::lock::RwLock;
    using WriteGuard = libtbag::lock::WriteLockGuard;
    using ReadGuard  = libtbag::lock::ReadLockGuard;

private:
    mutable RwLock  _rwlock;
    EventHandlerSet _handlers;

public:
    void add(void * handle, UvHandler * event)
    {
        WriteGuard guard(_rwlock);
        _handlers.insert(std::make_pair(Handle(handle), Event(event)));
    }

    void remove(Handle h)
    {
        WriteGuard guard(_rwlock);
        _handlers.erase(h);
    }

    bool exists(Handle h) const
    {
        ReadGuard guard(_rwlock);
        return _handlers.find(h) != _handlers.end();
    }

    Event get(uv_handle_t * handle) const
    {
        ReadGuard guard(_rwlock);
        auto find_itr = _handlers.find(Handle(handle));
        if (find_itr != _handlers.end()) {
            return find_itr->second;
        }
        return Event();
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

UvHandler::UvHandler(void * h) : _handle(h)
{
    using UvEventManager = libtbag::loop::event::uv::UvEventManager;
    UvEventManager * em = UvEventManager::getInstance();
    assert(em != nullptr);
    em->add(_handle, this);
}

UvHandler::~UvHandler()
{
    using UvEventManager = libtbag::loop::event::uv::UvEventManager;
    UvEventManager * em = UvEventManager::getInstance();
    assert(em != nullptr);
    em->remove(_handle);
}

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

