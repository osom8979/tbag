/**
 * @file   ModelNet.cpp
 * @brief  ModelNet class implementation.
 * @author zer0
 * @date   2018-10-22
 */

#include <libtbag/graph/ModelNet.hpp>
#include <libtbag/graph/ModelLayer.hpp>

#include <cassert>
#include <algorithm>
#include <utility>
#include <unordered_map>

#include <lemon/list_graph.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

using Digraph  = lemon::ListDigraph;
using LayerMap = lemon::ListDigraph::NodeMap<ModelLayer>;

using Node = Digraph::Node;
using Arc  = Digraph::Arc;

using LayerIds = ModelNet::LayerIds;
using IdSet = ModelNet::IdSet;

inline static int __get_id(Node v) TBAG_NOEXCEPT
{
    return Digraph::id(v);
}

inline static int __get_id(Arc e)  TBAG_NOEXCEPT
{
    return Digraph::id(e);
}

inline static Node __get_node(int id) TBAG_NOEXCEPT
{
    return Digraph::nodeFromId(id);
}

inline static Arc __get_arc(int id) TBAG_NOEXCEPT
{
    return Digraph::arcFromId(id);
}

/**
 * ModelNet Impl structures.
 *
 * @author zer0
 * @date   2018-10-22
 * @date   2019-07-05 (Refactoring: remove all public methods)
 */
struct ModelNet::Impl TBAG_FINAL : private Noncopyable
{
    Digraph graph;
    LayerMap layers;

    Impl() : graph(), layers(graph)
    {
        // EMPTY.
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// --------
// ModelNet
// --------

ModelNet::ModelNet() TBAG_NOEXCEPT
        : _impl(std::make_shared<Impl>())
{
    assert(static_cast<bool>(_impl));
}

ModelNet::ModelNet(std::nullptr_t) TBAG_NOEXCEPT
        : _impl(nullptr)
{
    assert(!static_cast<bool>(_impl));
}

ModelNet::ModelNet(ModelNet const & obj) TBAG_NOEXCEPT
        : _impl(obj._impl)
{
    // EMPTY.
}

ModelNet::ModelNet(ModelNet && obj) TBAG_NOEXCEPT
        : _impl(std::move(obj._impl))
{
    // EMPTY.
}

ModelNet::~ModelNet()
{
    // EMPTY.
}

ModelNet & ModelNet::operator =(ModelNet const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
    return *this;
}

ModelNet & ModelNet::operator =(ModelNet && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = std::move(obj._impl);
    }
    return *this;
}

void ModelNet::swap(ModelNet & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void ModelNet::clear()
{
    _impl = std::make_shared<Impl>();
}

bool ModelNet::empty() const
{
    assert(exists());
    return Digraph::NodeIt(_impl->graph) == lemon::INVALID;
}

std::size_t ModelNet::size() const
{
    assert(exists());
    std::size_t result = 0;
    for (Digraph::NodeIt n(_impl->graph); n != lemon::INVALID; ++n) {
        ++result;
    }
    return result;
}

void ModelNet::addNode(ModelLayer const & layer)
{
    assert(exists());
    auto node = _impl->graph.addNode();
    _impl->layers[node] = layer;
    _impl->layers[node]._assign_id(__get_id(node));
}

void ModelNet::addArc(ModelLayer const & source, ModelLayer const & target)
{
    assert(exists());
    _impl->graph.addArc(__get_node(source.id()), __get_node(target.id()));
}

LayerIds ModelNet::getLayerIds() const
{
    assert(exists());
    LayerIds result;
    for (Digraph::NodeIt n(_impl->graph); n != lemon::INVALID; ++n) {
        result.push_back(__get_id(n));
    }
    return result;
}

ModelNet::Layers ModelNet::getLayers() const
{
    assert(exists());
    Layers result;
    for (Digraph::NodeIt n(_impl->graph); n != lemon::INVALID; ++n) {
        result.push_back(_impl->layers[n]);
    }
    return result;
}

ModelLayer ModelNet::getLayer(int id) const
{
    assert(exists());
    if (id == LayerBase::NO_ASSIGN_ID) {
        return ModelLayer(nullptr);
    }
    return _impl->layers[__get_node(id)];
}

IdSet ModelNet::getSourceNodeIds(int node_id) const
{
    assert(exists());
    IdSet result;
    for (Digraph::InArcIt a(_impl->graph, __get_node(node_id)); a != lemon::INVALID; ++a) {
        result.emplace(__get_id(_impl->graph.source(a)));
    }
    return result;
}

IdSet ModelNet::getTargetNodeIds(int node_id) const
{
    assert(exists());
    IdSet result;
    for (Digraph::OutArcIt a(_impl->graph, __get_node(node_id)); a != lemon::INVALID; ++a) {
        result.emplace(__get_id(_impl->graph.target(a)));
    }
    return result;
}

IdSet ModelNet::getNodeIds(int node_id, ArcOrder order) const
{
    if (order == ArcOrder::AO_SOURCE) {
        return getSourceNodeIds(node_id);
    } else {
        assert(order == ArcOrder::AO_TARGET);
        return getTargetNodeIds(node_id);
    }
}

IdSet ModelNet::getChildrenNodeIds(int node_id, Direction direction) const
{
    return getNodeIds(node_id, direction == Direction::D_FORWARD ? ArcOrder::AO_TARGET : ArcOrder::AO_SOURCE);
}

IdSet ModelNet::getParentNodeIds(int node_id, Direction direction) const
{
    return getNodeIds(node_id, direction == Direction::D_FORWARD ? ArcOrder::AO_SOURCE : ArcOrder::AO_TARGET);
}

ModelNet::Layers ModelNet::getInputLayers(int node_id, ArcOrder order) const
{
    assert(exists());
    std::vector<ModelLayer> input_layers;
    for (auto & source_id : getNodeIds(node_id, order)) {
        input_layers.push_back(_impl->layers[__get_node(source_id)]);
    }
    return input_layers;
}

ModelNet::Layers ModelNet::getInputLayers(int node_id, Direction direction) const
{
    return getInputLayers(node_id, direction == Direction::D_FORWARD ? ArcOrder::AO_SOURCE : ArcOrder::AO_TARGET);
}

struct _NodeInfo
{
    /** Layer ID. */
    int id;

    /** Previous Layer ID. */
    int prev;

    /** Result code of the previous runner. */
    Err prev_code;
};

std::size_t ModelNet::run(IdSet const & start,
                          Direction direction,
                          std::size_t max_depth,
                          void * user,
                          std::vector<int> * sequence,
                          bool simulate) const
{
    assert(exists());

    if (start.empty()) {
        return 0;
    }

    std::vector<_NodeInfo> current;
    std::vector<_NodeInfo> children;
    std::size_t depth = 0;
    RunnerInfo info;

    for (auto const & start_id : start) {
        current.emplace_back(_NodeInfo{start_id, NO_ASSIGN_ID, E_SUCCESS});
    }

    for (; depth < max_depth; ++depth) {
        for (auto const & node : current) {
            auto const id = node.id;

            info.direction = direction;
            info.depth = depth;
            info.previous_id = node.prev;
            info.previous_code = node.prev_code;
            info.parent_ids = getParentNodeIds(id, direction);
            info.children_ids = getChildrenNodeIds(id, direction);
            info.user = user;

            Err code;
            if (simulate) {
                code = E_SIMULATE;
            } else {
                code = _impl->layers[__get_node(id)].runner(info);
            }

            for (auto const next_id : info.children_ids) {
                children.emplace_back(_NodeInfo{next_id, node.prev, code});
            }

            if (sequence) {
                sequence->push_back(id);
            }
        }

        if (children.empty()) {
            ++depth;
            break;
        }

        // Flip for next iteration.
        current.swap(children);
        children.clear();
    }

    return depth;
}

std::size_t ModelNet::forward(IdSet const & start,
                              std::size_t max_depth,
                              void * user,
                              std::vector<int> * sequence,
                              bool simulate) const
{
    return run(start, Direction::D_FORWARD, max_depth, user, sequence, simulate);
}

std::size_t ModelNet::backward(IdSet const & start,
                               std::size_t max_depth,
                               void * user,
                               std::vector<int> * sequence,
                               bool simulate) const
{
    return run(start, Direction::D_BACKWARD, max_depth, user, sequence, simulate);
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

