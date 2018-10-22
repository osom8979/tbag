/**
 * @file   ModelLayer.cpp
 * @brief  ModelLayer class implementation.
 * @author zer0
 * @date   2018-10-22
 */

#include <libtbag/graph/ModelLayer.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

/**
 * Pointer implementation.
 *
 * @author zer0
 * @date   2018-10-22
 */
struct ModelLayer::Impl : private Noncopyable
{
private:
    ModelLayer * _parent = nullptr;

public:
    Impl(ModelLayer * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// --------------------------
// ModelLayer implementation.
// --------------------------

ModelLayer::ModelLayer() TBAG_NOEXCEPT : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

ModelLayer::ModelLayer(ModelLayer const & obj) TBAG_NOEXCEPT : ModelLayer()
{
    (*this) = obj;
}

ModelLayer::ModelLayer(ModelLayer && obj) TBAG_NOEXCEPT : ModelLayer()
{
    (*this) = std::move(obj);
}

ModelLayer::~ModelLayer()
{
    // EMPTY.
}

ModelLayer & ModelLayer::operator =(ModelLayer const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

ModelLayer & ModelLayer::operator =(ModelLayer && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void ModelLayer::copy(ModelLayer const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void ModelLayer::swap(ModelLayer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

std::string ModelLayer::toString() const
{
    return std::string();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

