/**
 * @file   WriteQueueClient.cpp
 * @brief  WriteQueueClient class implementation.
 * @author zer0
 * @date   2017-09-24
 */

#include <libtbag/network/stream/WriteQueueClient.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/details/NetCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

WriteQueueClient::WriteQueueClient(ClientInterface * parent, Loop & loop, StreamType type)
        : InternalClient(parent, loop, type), _queue(), _max_size(details::MAXIMUM_WRITE_QUEUE_SIZE)
{
    // EMPTY.
}

WriteQueueClient::WriteQueueClient(ClientInterface * parent, Loop & loop, StreamType type, WriteReady const & ready)
        : InternalClient(parent, loop, type, ready), _queue(), _max_size(details::MAXIMUM_WRITE_QUEUE_SIZE)
{
    // EMPTY.
}

WriteQueueClient::~WriteQueueClient()
{
    // EMPTY.
}

Err WriteQueueClient::_writeFromQueue()
{
    auto & winfo = _atWriteInfo();
    if (winfo.isNotReady() || winfo.isClosing() || winfo.isEnd()) {
        return Err::E_CLOSING;
    }

    if (_queue.empty() || winfo.isReady() == false) {
        return Err::E_ILLSTATE;
    }

    SharedBuffer shared_buffer;
    Err const POP_CODE = _queue.frontAndPop(shared_buffer);
    if (TBAG_ERR_FAILURE(POP_CODE)) {
        tDLogE("WriteQueueClient::_writeFromQueue() Dequeue {} error", getErrName(POP_CODE));
        return POP_CODE;
    }

    tDLogD("WriteQueueClient::_writeFromQueue() Remaining queue size: {}", _queue.size());

    if (static_cast<bool>(shared_buffer) == false) {
        tDLogE("WriteQueueClient::_writeFromQueue() Expired buffer");
        return Err::E_EXPIRED;
    }

    if (shared_buffer->empty()) {
        tDLogE("WriteQueueClient::_writeFromQueue() Empty current buffer");
        return Err::E_EBUFFER;
    }

    return _autoWrite(shared_buffer->data(), shared_buffer->size());
}

bool WriteQueueClient::empty() const
{
    lock();
    bool const IS_EMPTY = _queue.empty();
    unlock();
    return IS_EMPTY;
}

std::size_t WriteQueueClient::size() const
{
    lock();
    std::size_t const SIZE = _queue.size();
    unlock();
    return SIZE;
}

std::size_t WriteQueueClient::getMaxSize() const
{
    lock();
    std::size_t const SIZE = _max_size;
    unlock();
    return SIZE;
}

void WriteQueueClient::setMaxSize(std::size_t size)
{
    lock();
    _max_size = size;
    unlock();
}

Err WriteQueueClient::autoWrite(char const * buffer, std::size_t size)
{
    ClientGuard const LOCK(*this);
    auto & winfo = _atWriteInfo();
    if (winfo.isNotReady() || winfo.isClosing() || winfo.isEnd()) {
        return Err::E_CLOSING;
    }

    if (winfo.isReady()) {
        return _autoWrite(buffer, size);
    }

    if (_queue.size() >= _max_size) {
        tDLogE("WriteQueueClient::write() The queue is full ({})", _max_size);
        return Err::E_EBUSY;
    }

    auto & back = _queue.push();
    if (static_cast<bool>(back)) {
        back->assign(buffer, buffer + size);
    } else {
        try {
            back.reset(new Buffer(buffer, buffer + size));
        } catch (...) {
            tDLogE("WriteQueueClient::write() Queue alloc fail.");
            return Err::E_BADALLOC;
        }
    }
    return Err::E_ENQASYNC;
}

void WriteQueueClient::preShutdown(Err code)
{
    ClientGuard const LOCK(*this);
    _atWriteInfo().setReady();
    if (_queue.empty() == false) {
        _writeFromQueue();
    }
}

void WriteQueueClient::preWrite(Err code)
{
    ClientGuard const LOCK(*this);
    auto & winfo = _atWriteInfo();
    assert(winfo.isWrite() || winfo.isShutdown());
    winfo.setReady();
    _stopShutdownTimer();
    if (_queue.empty() == false) {
        _writeFromQueue();
    }
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

