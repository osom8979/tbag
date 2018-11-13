/**
 * @file   AsyncSingleRequestQueue.hpp
 * @brief  AsyncSingleRequestQueue class prototype.
 * @author zer0
 * @date   2018-11-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_ASYNCSINGLEREQUESTQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_ASYNCSINGLEREQUESTQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <cassert>
#include <queue>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

/**
 * AsyncSingleRequestQueue class prototype.
 *
 * @author zer0
 * @date   2018-11-13
 *
 * @warning
 *  This class is not thread safe.
 */
template <typename T, typename RequestType>
class AsyncSingleRequestQueue : public Async
{
public:
    friend class libtbag::uvpp::Loop;

public:
    using Value   = T;
    using Request = RequestType;

public:
    static_assert(!std::is_same<Value, void>::value, "void type is not supported.");
    static_assert(is_request_type<Request>::value, "It must be the Request type.");

public:
    using Queue = std::queue<Value>;

public:
    enum class RequestState
    {
        RS_WAITING,
        RS_ASYNC,
        RS_REQUESTING,
    };

private:
    RequestState _state = RequestState::RS_WAITING;
    Queue        _queue;
    Request      _request;

protected:
    AsyncSingleRequestQueue(Loop & loop) : Async(loop), _state(RequestState::RS_WAITING)
    { /* EMPTY. */ }

public:
    virtual ~AsyncSingleRequestQueue()
    { /* EMPTY. */ }

public:
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_queue.empty())
    { return _queue.empty(); }

    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_queue.size())
    { return _queue.size(); }

    inline bool isWaiting() const TBAG_NOEXCEPT_SP_OP(_queue.empty())
    { return _state == RequestState::RS_WAITING; }

public:
    void enqueue(Value const & value)
    {
        _queue.push(value);
        if (_state == RequestState::RS_WAITING) {
            auto const CODE = send();
            assert(isSuccess(CODE));
            _state = RequestState::RS_ASYNC;
        }
    }

    virtual void onAsync() override
    {
        assert(!_queue.empty());
        assert(_state == RequestState::RS_ASYNC);
        _state = RequestState::RS_REQUESTING;

        doRequest(_request, _queue.front());
    }

    void doneRequest()
    {
        assert(!_queue.empty());
        assert(_state == RequestState::RS_REQUESTING);

        auto value = _queue.front();
        _queue.pop();

        onDequeue(value);

        if (_queue.empty()) {
            _state = RequestState::RS_WAITING;
        } else {
            auto const CODE = send();
            assert(isSuccess(CODE));
            _state = RequestState::RS_ASYNC;
        }
    }

protected:
    virtual void doRequest(Request & request, Value const & value) = 0;
    virtual void onDequeue(Value const & value) = 0;
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_ASYNCSINGLEREQUESTQUEUE_HPP__

