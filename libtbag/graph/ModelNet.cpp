/**
 * @file   ModelNet.cpp
 * @brief  ModelNet class implementation.
 * @author zer0
 * @date   2018-10-22
 */

#include <libtbag/graph/ModelNet.hpp>
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
struct ModelNet::Impl : private Noncopyable
{
private:
    ModelNet * _parent = nullptr;

public:
    Impl(ModelNet * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ------------------------
// ModelNet implementation.
// ------------------------

ModelNet::ModelNet() TBAG_NOEXCEPT : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

ModelNet::ModelNet(ModelNet const & obj) TBAG_NOEXCEPT : ModelNet()
{
    (*this) = obj;
}

ModelNet::ModelNet(ModelNet && obj) TBAG_NOEXCEPT : ModelNet()
{
    (*this) = std::move(obj);
}

ModelNet::~ModelNet()
{
    // EMPTY.
}

ModelNet & ModelNet::operator =(ModelNet const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

ModelNet & ModelNet::operator =(ModelNet && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void ModelNet::copy(ModelNet const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void ModelNet::swap(ModelNet & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

std::string ModelNet::toString() const
{
    return std::string();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

