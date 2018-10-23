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

public:
    using Digraph  = lemon::ListDigraph;
    using LayerMap = lemon::ListDigraph::NodeMap<ModelLayer>;

    using Node = Digraph::Node;
    using Arc  = Digraph::Arc;

private:
    Digraph  _graph;
    LayerMap _layers;

private:
    Node _first;
    Node _last;

public:
    // @formatter:off
    static int getId(Node v) { return Digraph::id(v); }
    static int getId(Arc  e) { return Digraph::id(e); }

    static Node getNode(int id) { return Digraph::nodeFromId(id); }
    static Arc   getArc(int id) { return Digraph:: arcFromId(id); }
    // @formatter:on

public:
    Impl(ModelNet * parent) : _parent(parent), _graph(), _layers(_graph)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

    Err addFirst(ModelLayer & layer)
    {
        _first = _graph.addNode();
        layer.setId(getId(_first));
        _layers[_first] = layer;
        return Err::E_SUCCESS;
    }

    Err addNode(ModelLayer & layer)
    {
        auto node = _graph.addNode();
        layer.setId(getId(node));
        _layers[node] = layer;
        return Err::E_SUCCESS;
    }

    Err addLast(ModelLayer & layer)
    {
        _last = _graph.addNode();
        layer.setId(getId(_last));
        _layers[_last] = layer;
        return Err::E_SUCCESS;
    }

    Err addArc(ModelLayer const & source, ModelLayer const & target)
    {
        _graph.addArc(getNode(source.getId()), getNode(target.getId()));
        return Err::E_SUCCESS;
    }

    std::vector<int> getSourceNodeIds(int node_id) const
    {
        std::vector<int> result;
        for (Digraph::InArcIt a(_graph, getNode(node_id)); a != lemon::INVALID; ++a) {
            result.push_back(getId(_graph.source(a)));
        }
        return result;
    }

    std::vector<int> getTargetNodeIds(int node_id) const
    {
        std::vector<int> result;
        for (Digraph::OutArcIt a(_graph, getNode(node_id)); a != lemon::INVALID; ++a) {
            result.push_back(getId(_graph.target(a)));
        }
        return result;
    }

    /** Unplug the finished flags. */
    void updateIncomplete()
    {
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            if (_layers[n]) {
                _layers[n].incomplete();
            }
        }
    }

    Err forward()
    {
        updateIncomplete();

        using namespace lemon;

        std::set<int> current = {getId(_first)};
        std::set<int> children;

        std::size_t max_depth = 100;
        for (std::size_t current_depth = 0; current_depth < max_depth; ++current_depth) {
            // Forward current list.
            for (auto & current_id : current) {
                std::vector<ModelLayer> input_layers;
                for (auto & source_id : getSourceNodeIds(current_id)) {
                    input_layers.push_back(_layers[getNode(source_id)]);
                }

                auto const CODE = _layers[getNode(current_id)].forward(input_layers);
                if (isSuccess(CODE)) {
                    _layers[getNode(current_id)].complete();
                }
            }

            // Update children list.
            children.clear();
            for (auto & current_id : current) {
                for (auto & target_id : getTargetNodeIds(current_id)) {

                    bool ready = true;
                    for (auto & source_id : getSourceNodeIds(target_id)) {
                        if (!_layers[getNode(source_id)].isComplete()) {
                            ready = false;
                            break;
                        }
                    }

                    if (ready) {
                        children.insert(target_id);
                    }
                }
            }

            if (children.empty()) {
                // No more children exist.
                break;
            }

            // Flip
            current.swap(children);
        }

        return Err::E_SUCCESS;
    }

    Err backward()
    {
        updateIncomplete();

        using namespace lemon;

        std::set<int> current = {getId(_last)};
        std::set<int> children;

        std::size_t max_depth = 100;
        for (std::size_t current_depth = 0; current_depth < max_depth; ++current_depth) {
            // Forward current list.
            for (auto & current_id : current) {
                std::vector<ModelLayer> input_layers;
                for (auto & target_id : getTargetNodeIds(current_id)) {
                    input_layers.push_back(_layers[getNode(target_id)]);
                }

                auto const CODE = _layers[getNode(current_id)].backward(input_layers);
                if (isSuccess(CODE)) {
                    _layers[getNode(current_id)].complete();
                }
            }

            // Update children list.
            children.clear();
            for (auto & current_id : current) {
                for (auto & source_id : getSourceNodeIds(current_id)) {

                    bool ready = true;
                    for (auto & target_id : getTargetNodeIds(source_id)) {
                        if (!_layers[getNode(target_id)].isComplete()) {
                            ready = false;
                            break;
                        }
                    }

                    if (ready) {
                        children.insert(source_id);
                    }
                }
            }

            if (children.empty()) {
                // No more children exist.
                break;
            }

            // Flip
            current.swap(children);
        }

        return Err::E_SUCCESS;
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
    clear();
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
    assert(exists());
    _impl.reset();
}

Err ModelNet::addFirst(ModelLayer & layer)
{
    assert(exists());
    return _impl->addFirst(layer);
}

Err ModelNet::addNode(ModelLayer & layer)
{
    assert(exists());
    return _impl->addNode(layer);
}

Err ModelNet::addLast(ModelLayer & layer)
{
    assert(exists());
    return _impl->addLast(layer);
}

Err ModelNet::addArc(ModelLayer const & source, ModelLayer const & target)
{
    assert(exists());
    return _impl->addArc(source, target);
}

Err ModelNet::forward()
{
    assert(exists());
    return _impl->forward();
}

Err ModelNet::backward()
{
    assert(exists());
    return _impl->backward();
}

std::string ModelNet::toString() const
{
    return std::string();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

