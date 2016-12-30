/**
 * @file   RequestQueue.cpp
 * @brief  RequestQueue class implementation.
 * @author zer0
 * @date   2016-12-28
 */

#include <libtbag/uv/ex/RequestQueue.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/Request.hpp>
#include <libtbag/uv/Handle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {
namespace ex {

RequestQueue::RequestQueue(UvRequestType type) : _type(type)
{
    // EMPTY.
}

RequestQueue::~RequestQueue()
{
    Guard guard(_mutex);
    while (_prepare.empty() == false) {
        _prepare.pop();
    }
    _active.clear();
}

RequestQueue::WeakRequest RequestQueue::find(Request * request) const
{
    Guard guard(_mutex);

    auto find_itr = _active.find(RequestKey(request));
    if (find_itr == _active.end()) {
        return WeakRequest();
    }
    return WeakRequest(find_itr->second);
}

RequestQueue::WeakRequest RequestQueue::create(Handle * owner)
{
    Guard guard(_mutex);

    if (_prepare.empty()) {
        Request * request = new Request(_type, owner);
        auto insert_itr = _active.insert(ActiveMap::value_type(RequestKey(request), SharedRequest(request)));
        if (insert_itr.second == false) {
            __tbag_error("RequestQueue::create() insert request error.");
            return WeakRequest();
        }
        return WeakRequest(insert_itr.first->second);
    }

    // Find prepare queue.
    auto prepare = _prepare.front();
    _prepare.pop();
    prepare->setOwner(owner);
    _active.insert(ActiveMap::value_type(RequestKey(prepare.get()), prepare));
    return WeakRequest(prepare);
}

void RequestQueue::release(Request * request)
{
    Guard guard(_mutex);

    auto find_itr = _active.find(RequestKey(request));
    if (find_itr == _active.end()) {
        return;
    }

    _prepare.push(find_itr->second);
    _active.erase(find_itr);
}

} // namespace ex
} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

