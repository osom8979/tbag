/**
 * @file   UvHandlerManager.hpp
 * @brief  UvHandlerManager class prototype.
 * @author zer0
 * @date   2016-11-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVHANDLERMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVHANDLERMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/container/Pointer.hpp>
#include <libtbag/loop/event/UvHandler.hpp>
#include <libtbag/lock/RwLock.hpp>
#include <libtbag/Type.hpp>

#include <unordered_map>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop  {
namespace event {

/**
 * UvHandlerManager class prototype.
 *
 * @author zer0
 * @date   2016-05-18
 * @date   2016-10-16 (Restore this class)
 * @date   2016-11-25 (Rename: UvEventManager -> UvHandlerManager)
 */
class TBAG_API UvHandlerManager : public Noncopyable
{
public:
    template <typename T>
    using Pointer = libtbag::container::Pointer<T>;

    using Native  = Pointer<void>;
    using Handler = Pointer<UvHandler>;

#if defined(__OS_MACOS__) && !defined(NDEBUG)
    // Only debugging.
    using EventHandlerMap = std::map<Native, Handler, Native::Less>;
#else
    using EventHandlerMap = std::unordered_map<Native, Handler, Native::Hash, Native::EqualTo>;
#endif

    using RwLock     = libtbag::lock::RwLock;
    using WriteGuard = libtbag::lock::WriteLockGuard;
    using ReadGuard  = libtbag::lock::ReadLockGuard;

private:
    mutable RwLock  _rwlock;
    EventHandlerMap _handlers;

public:
    UvHandlerManager();
    ~UvHandlerManager();

public:
    void add(void * handle, UvHandler * event);
    void remove(Native native);

public:
    template <typename HandleType>
    inline Handler get(HandleType * handle) const
    {
        ReadGuard guard(_rwlock);
        auto find_itr = _handlers.find((void*)handle);
        if (find_itr != _handlers.end()) {
            return find_itr->second;
        }
        return Handler(nullptr);
    }
};

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVHANDLERMANAGER_HPP__

