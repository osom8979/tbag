/**
 * @file   UvHandlerManager.cpp
 * @brief  UvHandlerManager class implementation.
 * @author zer0
 * @date   2016-11-25
 */

#include <libtbag/loop/event/UvHandlerManager.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop  {
namespace event {

UvHandlerManager::UvHandlerManager()
{
    // EMPTY.
}

UvHandlerManager::~UvHandlerManager()
{
    // EMPTY.
}

void UvHandlerManager::add(void * handle, UvHandler * event)
{
    WriteGuard guard(_rwlock);
    _handlers.insert(std::make_pair(Native(handle), Handler(event)));
}

void UvHandlerManager::remove(Native native)
{
    WriteGuard guard(_rwlock);
    _handlers.erase(native);
}

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

