/**
 * @file   MqQueue.cpp
 * @brief  MqQueue class implementation.
 * @author zer0
 * @date   2018-11-10
 */

#include <libtbag/mq/details/MqQueue.hpp>
#include <libtbag/bitwise/BitHacks.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <cmath>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

MqQueue::MqQueue(std::size_t size, std::size_t msg_size) : _active(), _ready()
{
    std::size_t const POWER_OF_2 = calcMinimumQueueSize(size);
    _active = std::make_unique<Queue>(this, POWER_OF_2);
    _ready = std::make_unique<Queue>(this, POWER_OF_2);
    assert(static_cast<bool>(_active));
    assert(static_cast<bool>(_ready));
    for (std::size_t i = 0; i < POWER_OF_2; ++i) {
        assert(_ready->enqueue(createMsg(msg_size)));
    }
    assert(_ready->potentially_inaccurate_count());
}

MqQueue::~MqQueue()
{
    _active.reset();
    _ready.reset();
}

MqMsg * MqQueue::createMsg(std::size_t size)
{
    return new MqMsg(size);
}

void MqQueue::removeMsg(MqMsg * value)
{
    delete value;
}

std::size_t MqQueue::calcMinimumQueueSize(std::size_t request_size)
{
    using namespace libtbag::bitwise;
    if (isPowerOf2(request_size)) {
        return request_size;
    }
    auto const RESULT_SIZE = static_cast<std::size_t>(pow(2, findMostSignificantBit(request_size) + 1));
    assert(isPowerOf2(RESULT_SIZE));
    assert(RESULT_SIZE >= request_size);
    return RESULT_SIZE;
}

std::size_t MqQueue::getInaccurateSizeOfActive() const
{
    return _active->potentially_inaccurate_count();
}

std::size_t MqQueue::getInaccurateSizeOfReady() const
{
    return _ready->potentially_inaccurate_count();
}

MqQueue::MiscValidity MqQueue::validateOfActive(std::size_t min, std::size_t max) const
{
    return _active->singlethreaded_validate(min, max);
}

MqQueue::MiscValidity MqQueue::validateOfReady(std::size_t min, std::size_t max) const
{
    return _ready->singlethreaded_validate(min, max);
}

// ---------------
namespace __impl {
// ---------------

struct DefaultCopyFrom
{
    char const * data;
    std::size_t  size;

    DefaultCopyFrom(char const * d, std::size_t s) : data(d), size(s)
    { /* EMPTY. */ }
    ~DefaultCopyFrom()
    { /* EMPTY. */ }

    bool operator()(MqMsg * msg)
    {
        Err code;
        msg->type = MqType::MT_BOX_ADDRESS;
        assert(size <= libtbag::type::TypeInfo<unsigned>::maximum());
        code = msg->box.resize(static_cast<unsigned>(size));
        assert(isSuccess(code));
        code = msg->box.copyFrom(data, size);
        assert(isSuccess(code));
        return true;
    }
};

struct DefaultCopyTo
{
    MqType      * type;
    char        * data;
    std::size_t    max;
    std::size_t * size;

    DefaultCopyTo(MqType * t, char * d, std::size_t m, std::size_t * s)
            : type(t), data(d), max(m), size(s)
    { /* EMPTY. */ }
    ~DefaultCopyTo()
    { /* EMPTY. */ }

    bool operator()(MqMsg * msg)
    {
        if (type != nullptr) {
            *type = msg->type;
        }
        auto const BOX_SIZE = msg->box.size();
        if (size != nullptr) {
            *size = BOX_SIZE;
        }
        if (data != nullptr) {
            auto const * BEGIN = msg->box.cast<int8_t>();
            auto const * END   = BEGIN + (BOX_SIZE <= max ? BOX_SIZE : max);
            std::copy(BEGIN, END, data);
        }
        return true;
    }
};

// ------------------
} // namespace __impl
// ------------------

Err MqQueue::enqueue(char const * data, std::size_t size)
{
    return enqueue(__impl::DefaultCopyFrom(data, size));
}

Err MqQueue::dequeue(char * data, std::size_t max_buffer, MqType * type, std::size_t * size)
{
    return dequeue(__impl::DefaultCopyTo(type, data, max_buffer, size));
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

