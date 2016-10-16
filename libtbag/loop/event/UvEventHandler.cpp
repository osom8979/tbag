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

// ------------------------------
// UvEventHandler implementation.
// ------------------------------

UvEventHandler::UvEventHandler()
{
    // EMPTY.
}

UvEventHandler::~UvEventHandler()
{
    // EMPTY.
}

void UvEventHandler::add(void * handle)
{
    _handles.insert(UvHandle(handle));
}

void UvEventHandler::remove(void * handle)
{
    _handles.erase(UvHandle(handle));
}

bool UvEventHandler::exists(void * handle)
{
    return _handles.find(UvHandle(handle)) != _handles.end();
}

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

