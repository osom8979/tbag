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

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop  {
namespace event {

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

// ------------------------------
// UvEventHandler implementation.
// ------------------------------

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

