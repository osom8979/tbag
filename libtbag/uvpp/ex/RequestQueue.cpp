/**
 * @file   RequestQueue.cpp
 * @brief  RequestQueue class implementation.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/ex/RequestQueue.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/uvpp/Handle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

RequestQueue::RequestQueue(ureq type) : _type(type)
{
    // EMPTY.
}

RequestQueue::~RequestQueue()
{
    clear();
}

void RequestQueue::clear()
{
    Guard guard(_mutex);
    while (_prepare.empty() == false) {
        _prepare.pop();
    }
    _active.clear();
}

RequestQueue::WeakRequest RequestQueue::find(RequestKey request) const
{
    Guard guard(_mutex);
    auto itr = _active.find(request);
    if (itr == _active.end()) {
        return WeakRequest();
    }
    return WeakRequest(itr->second);
}

RequestQueue::WeakRequest RequestQueue::create(Handle * owner)
{
    Guard guard(_mutex);

    if (_prepare.empty()) {
        Request * request = new Request(_type, owner);
        auto insert_itr = _active.insert(ActiveMap::value_type(RequestKey(request), SharedRequest(request)));
        if (insert_itr.second == false) {
            tDLogE("RequestQueue::create() insert request error.");
            return WeakRequest();
        }
        return WeakRequest(insert_itr.first->second);
    }

    // Find prepare queue.
    SharedRequest prepare = _prepare.front();
    _prepare.pop();
    prepare->setOwner(owner);

    _active.insert(ActiveMap::value_type(RequestKey(prepare.get()), prepare));
    return WeakRequest(prepare);
}

void RequestQueue::release(RequestKey request)
{
    Guard guard(_mutex);
    auto itr = _active.find(request);
    if (itr != _active.end()) {
        _prepare.push(itr->second);
        _active.erase(itr);
    }
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

