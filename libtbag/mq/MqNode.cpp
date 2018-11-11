/**
 * @file   MqNode.cpp
 * @brief  MqNode class implementation.
 * @author zer0
 * @date   2018-11-11
 */

#include <libtbag/mq/MqNode.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

/**
 * MqNode::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-11
 */
struct MqNode::Impl : private Noncopyable
{
public:
    using Pool = libtbag::thread::ThreadPool;
    using Loop = libtbag::uvpp::Loop;

public:
    struct bind_t    { /* EMPTY. */ };
    struct connect_t { /* EMPTY. */ };

public:
    /**
     * @warning
     *  Don't change this variable.
     */
    TBAG_CONSTEXPR static std::size_t const THREAD_SIZE = 1U;

private:
    MqNode * _parent;

private:
    Pool _pool;
    Loop _loop;
    Err  _last;

public:
    Impl(MqNode * parent) : _pool(THREAD_SIZE), _parent(parent), _last(Err::E_EBUSY)
    {
        assert(_parent != nullptr);
    }

    Impl(MqNode * parent, std::string const & uri, bind_t) : Impl(parent)
    {
        if (!startTask()) {
            throw std::bad_alloc();
        }
    }

    Impl(MqNode * parent, std::string const & uri, connect_t) : Impl(parent)
    {
        if (!startTask()) {
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
        // EMPTY.
    }

private:
    bool startTask()
    {
        return _pool.push([&](){
            this->runner();
        });
    }

public:
    void runner()
    {
        _last = _loop.run();
        if (isFailure(_last)) {
            tDLogE("MqNode::Impl::runner() Loop error: {}", _last);
        }
        _pool.exit();
    }
};

// ----------------------
// MqNode implementation.
// ----------------------

MqNode::MqNode() : _impl(nullptr)
{
    // EMPTY.
}

MqNode::MqNode(MqNode const & obj) TBAG_NOEXCEPT : MqNode()
{
    (*this) = obj;
}

MqNode::MqNode(MqNode && obj) TBAG_NOEXCEPT : MqNode()
{
    (*this) = std::move(obj);
}

MqNode::~MqNode()
{
    _impl.reset();
}

MqNode & MqNode::operator =(MqNode const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

MqNode & MqNode::operator =(MqNode && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void MqNode::copy(MqNode const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void MqNode::swap(MqNode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

Err MqNode::bind(std::string const & uri)
{
    try {
        _impl = std::make_shared<Impl>(this, uri, Impl::bind_t{});
    } catch (...) {
        return Err::E_UNKEXCP;
    }
    return Err::E_SUCCESS;
}

Err MqNode::connect(std::string const & uri)
{
    try {
        _impl = std::make_shared<Impl>(this, uri, Impl::connect_t{});
    } catch (...) {
        return Err::E_UNKEXCP;
    }
    return Err::E_SUCCESS;
}

Err MqNode::close()
{
    _impl.reset();
    return Err::E_SUCCESS;
}

Err MqNode::send(char const * buffer, std::size_t size)
{
    return Err::E_UNSUPOP;
}

Err MqNode::recv(char * buffer, std::size_t buffer_size, std::size_t * read_size)
{
    return Err::E_UNSUPOP;
}

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

