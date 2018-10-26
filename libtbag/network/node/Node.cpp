/**
 * @file   Node.cpp
 * @brief  Node class implementation.
 * @author zer0
 * @date   2018-10-26
 */

#include <libtbag/network/node/Node.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace node    {

/**
 * Node::Impl class implementation.
 *
 * @author zer0
 * @date   2018-10-26
 */
struct Node::Impl : private Noncopyable
{
private:
    Node * _parent;

public:
    Impl(Node * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// Node implementation.
// ----------------------------

Node::Node() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

Node::Node(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Node::Node(Node const & obj) TBAG_NOEXCEPT : Node(nullptr)
{
    (*this) = obj;
}

Node::Node(Node && obj) TBAG_NOEXCEPT : Node(nullptr)
{
    (*this) = std::move(obj);
}

Node::~Node()
{
    // EMPTY.
}

Node & Node::operator =(Node const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Node & Node::operator =(Node && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Node::copy(Node const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void Node::swap(Node & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

} // namespace node
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

