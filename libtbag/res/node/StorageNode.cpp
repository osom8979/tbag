/**
 * @file   StorageNode.cpp
 * @brief  StorageNode class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbag/res/node/StorageNode.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res  {
namespace node {

StorageNode::StorageNode()
{
    // EMPTY.
}

StorageNode::StorageNode(StorageNode const & obj) : StorageNode()
{
    (*this) = obj;
}

StorageNode::StorageNode(StorageNode && obj) TBAG_NOEXCEPT : StorageNode()
{
    (*this) = std::move(obj);
}

StorageNode::~StorageNode()
{
    // EMPTY.
}

StorageNode & StorageNode::operator =(StorageNode const & obj)
{
    copy(obj);
    return *this;
}

StorageNode & StorageNode::operator =(StorageNode && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void StorageNode::copy(StorageNode const & obj)
{
    if (this != &obj) {
        // TODO: Copy object.
    }
}

void StorageNode::swap(StorageNode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // TODO: Swap object.
    }
}

std::string StorageNode::name() const
{
    return std::string(TAG_STORAGE);
}

void StorageNode::setup()
{
}

void StorageNode::teardown()
{
}

void StorageNode::load(Element const & element)
{
//    std::string default_root;
//    if (isSuccess(optAttr(element, ATT_DEFAULT_ROOT, default_root))) {
//    } else {
//        default_root =
//    }
//
//    ATT_DEFAULT_ROOT
//    'directory_path' archive='dir' absolute='true'>
}

void StorageNode::save(Element & element) const
{
}

} // namespace node
} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

