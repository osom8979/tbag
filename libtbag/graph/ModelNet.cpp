/**
 * @file   ModelNet.cpp
 * @brief  ModelNet class implementation.
 * @author zer0
 * @date   2018-10-22
 */

#include <libtbag/graph/ModelNet.hpp>
#include <libtbag/graph/ModelLayer.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

#include <lemon/list_graph.h>

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

private:
    lemon::ListGraph graph;
    lemon::ListGraph::NodeMap<ModelLayer> layers;

private:
    lemon::ListGraph::Node first;
    lemon::ListGraph::Node last;

public:
    Impl(ModelNet * parent) : _parent(parent), graph(), layers(graph)
    {
        assert(_parent != nullptr);

        first = graph.addNode();
        last  = graph.addNode();

        layers[first] = ModelLayer();
        layers[last]  = ModelLayer();
    }

    ~Impl()
    {
        // EMPTY.
    }

    /** Unplug the finished flags. */
    void updateIncomplete()
    {
        for (lemon::ListGraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
            layers[n].complete();
        }
    }

    Err forward()
    {
        updateIncomplete();
        return Err::E_UNSUPOP;
    }

    Err backward()
    {
        updateIncomplete();
        return Err::E_UNSUPOP;
    }
};

// ------------------------
// ModelNet implementation.
// ------------------------

ModelNet::ModelNet() TBAG_NOEXCEPT : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

ModelNet::ModelNet(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

ModelNet::ModelNet(ModelNet const & obj) TBAG_NOEXCEPT : ModelNet(nullptr)
{
    (*this) = obj;
}

ModelNet::ModelNet(ModelNet && obj) TBAG_NOEXCEPT : ModelNet(nullptr)
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

void ModelNet::clear()
{
}

Err ModelNet::forward()
{
    return Err::E_UNSUPOP;
}

Err ModelNet::backward()
{
    return Err::E_UNSUPOP;
}

std::string ModelNet::toString() const
{
    return std::string();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

