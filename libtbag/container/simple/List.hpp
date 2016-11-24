/**
 * @file   List.hpp
 * @brief  List class prototype.
 * @author zer0
 * @date   2016-05-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SIMPLE_LIST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SIMPLE_LIST_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/log/Log.hpp>

#include <cstddef>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {
namespace simple    {
namespace list      {

// Forward declaration.
template <typename T>
struct Node;

/**
 * Node of the LIST Data structure.
 *
 * @author zer0
 * @date   2015-11-16
 */
template <typename T>
struct Node
{
    T data;
    struct Node<T> * next;
};

template <typename T>
TBAG_CONSTEXPR Node<typename remove_cr<T>::type> *
createNode(T && data, Node<typename remove_cr<T>::type> * node = nullptr)
{
    typedef typename remove_cr<T>::type __remove_cr;
    typedef Node<__remove_cr> __node_type;
    return new (std::nothrow) __node_type{std::forward<T>(data), node};
}

template <typename T>
inline bool releaseNode(Node<T> ** node)
{
    if (node == nullptr || (*node) == nullptr) {
        __tbag_error("releaseNode() NullPointerException.");
        return false;
    }

    delete (*node);
    (*node) = nullptr;
    return true;
}

template <typename T>
Node<T> * atNode(Node<T> * node, std::size_t index = 0) TBAG_NOEXCEPT
{
    while (index > 0) {
        if (node == nullptr) {
            return nullptr;
        }
        node = node->next;
        --index;
    }

    return node;
}

template <typename T>
bool insertNext(Node<T> * node, T data) TBAG_NOEXCEPT
{
    if (node == nullptr) {
        return false;
    }

    Node<T> * new_node = nullptr;
    try {
        new_node = createNode<T>(data, nullptr);
    } catch (...) {
        return false;
    }

    if (node->next != nullptr) {
        new_node->next = node->next;
    }
    node->next = new_node;

    return true;
}

template <typename T>
bool insertNext(Node<T> * node, std::size_t index, T data) TBAG_NOEXCEPT
{
    return insertNext(atNode(node, index), data);
}

template <typename T>
bool eraseNext(Node<T> * node) TBAG_NOEXCEPT
{
    if (node == nullptr) {
        return false;
    }

    assert(node != nullptr);
    if (node->next == nullptr) {
        return true;
    }

    assert(node->next != nullptr);
    Node<T> * next_next_node = node->next->next;

    if (releaseNode(&node->next) == false) {
        return false;
    }

    assert(node->next == nullptr);
    node->next = next_next_node;

    return true;
}

template <typename T>
bool eraseNext(Node<T> * node, std::size_t index) TBAG_NOEXCEPT
{
    return eraseNext(atNode(node, index));
}

template <typename T>
bool isLeaf(Node<T> * node)
{
    assert(node != nullptr);
    return (node->next == nullptr ? true : false);
}

template <typename T>
std::size_t size(Node<T> * node)
{
    Node<T> * cursor = node;
    std::size_t index = 0;

    while (cursor != nullptr) {
        cursor = cursor->next;
        ++index;
    }

    return index;
}

} // namespace list
} // namespace simple
} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SIMPLE_LIST_HPP__

