/**
 * @file   Node.hpp
 * @brief  Node class prototype.
 * @author zer0
 * @date   2018-10-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_NODE_NODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_NODE_NODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace node    {

/**
 * Node class prototype.
 *
 * @author zer0
 * @date   2018-10-26
 */
class TBAG_API Node
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    Node();
    Node(std::nullptr_t) TBAG_NOEXCEPT;
    Node(Node const & obj) TBAG_NOEXCEPT;
    Node(Node && obj) TBAG_NOEXCEPT;
    ~Node();

public:
    Node & operator =(Node const & obj) TBAG_NOEXCEPT;
    Node & operator =(Node && obj) TBAG_NOEXCEPT;

public:
    void copy(Node const & obj) TBAG_NOEXCEPT;
    void swap(Node & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Node & lh, Node & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(Node const & x, Node const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(Node const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }
};

} // namespace node
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_NODE_NODE_HPP__

