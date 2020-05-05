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

std::vector<int> ModelNet::getLayerIds() const
{
    assert(exists());
    std::vector<int> result;
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

std::vector<int> ModelNet::getSourceNodeIds(int node_id) const
{
    assert(exists());
    std::vector<int> result;
    for (Digraph::InArcIt a(_impl->graph, __get_node(node_id)); a != lemon::INVALID; ++a) {
        result.push_back(__get_id(_impl->graph.source(a)));
    }
    return result;
}

std::vector<int> ModelNet::getTargetNodeIds(int node_id) const
{
    assert(exists());
    std::vector<int> result;
    for (Digraph::OutArcIt a(_impl->graph, __get_node(node_id)); a != lemon::INVALID; ++a) {
        result.push_back(__get_id(_impl->graph.target(a)));
    }
    return result;
}

std::vector<int> ModelNet::getNodeIds(int node_id, ArcOrder order) const
{
    if (order == ArcOrder::AO_SOURCE) {
        return getSourceNodeIds(node_id);
    } else {
        assert(order == ArcOrder::AO_TARGET);
        return getTargetNodeIds(node_id);
    }
}

std::vector<int> ModelNet::getChildrenNodeIds(int node_id, Direction direction) const
{
    return getNodeIds(node_id, direction == Direction::D_FORWARD ? ArcOrder::AO_TARGET : ArcOrder::AO_SOURCE);
}

std::vector<int> ModelNet::getParentNodeIds(int node_id, Direction direction) const
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

Err ModelNet::run(std::set<int> const & start,
                  Direction direction,
                  std::size_t max_depth,
                  void * user,
                  std::vector<int> * sequence,
                  bool simulate) const
{
    assert(exists());
    if (start.empty()) {
        return E_ILLARGS;
    }

    auto const all_ids = getLayerIds();
    assert(!all_ids.empty());

    std::unordered_map<int, bool> completes;
    for (auto const id : all_ids) {
        completes.emplace(id, false);
    }

    std::set<int> current = start;
    std::set<int> children;

    for (auto depth = 0; depth < max_depth; ++depth) {
        if (current.empty()) {
            break; // No more current node exist.
        }

        // Run current list.
        for (auto const id : current) {
            if (completes[id]) {
                continue; // This layer has already been run.
            }

            Err code;
            if (simulate) {
                code = E_SUCCESS;
            } else {
                code = _impl->layers[__get_node(id)].runner(direction, user);
                if (isFailure(code)) {
                    return code;
                }
            }

            assert(isSuccess(code));
            completes[id] = true;

            if (sequence) {
                sequence->push_back(id);
            }
        }

        // Update children list.
        children.clear();

        for (auto const id : current) {
            for (auto const next_node_id : getChildrenNodeIds(id, direction)) {
                if (!completes[next_node_id]) {
                    children.insert(next_node_id);
                }
            }
        }

        // Flip for next iteration.
        current.swap(children);
    }

    return E_SUCCESS;
}

Err ModelNet::dependencyRun(std::set<int> const & start,
                            Direction direction,
                            std::size_t max_depth,
                            void * user,
                            std::vector<int> * sequence,
                            bool simulate,
                            bool skip_first_dep_test) const
{
    assert(exists());
    if (start.empty()) {
        return E_ILLARGS;
    }

    if (!skip_first_dep_test) {
        // The parent node must not exist because it cannot be executed.
        for (auto const id : start) {
            if (!getParentNodeIds(id, direction).empty()) {
                return E_NREADY;
            }
        }
    }

    auto const all_ids = getLayerIds();
    assert(!all_ids.empty());

    std::unordered_map<int, bool> completes;
    for (auto const id : all_ids) {
        completes.emplace(id, false);
    }

    auto const is_ready = [&](int id) -> bool {
        for (auto const parent_id : getParentNodeIds(id, direction)) {
            if (!completes[parent_id]) {
                return false;
            }
        }
        return true;
    };

    std::set<int> current = start;
    std::set<int> children;

    for (auto depth = 0; depth < max_depth; ++depth) {
        if (current.empty()) {
            break; // No more current node exist.
        }

        // Run current list.
        for (auto const id : current) {
            if (completes[id]) {
                continue; // This layer has already been run.
            }

            Err code;
            if (simulate) {
                code = E_SUCCESS;
            } else {
                code = _impl->layers[__get_node(id)].runner(direction, user);
                if (isFailure(code)) {
                    return code;
                }
            }

            assert(isSuccess(code));
            completes[id] = true;

            if (sequence) {
                sequence->push_back(id);
            }
        }

        // Update children list.
        children.clear();

        for (auto const id : current) {
            for (auto const next_node_id : getChildrenNodeIds(id, direction)) {
                if (!completes[next_node_id] && is_ready(next_node_id)) {
                    children.insert(next_node_id);
                }
            }
        }

        // Flip for next iteration.
        current.swap(children);
    }

    return E_SUCCESS;
}

Err ModelNet::forward(std::set<int> const & start,
                      std::size_t max_depth,
                      void * user,
                      std::vector<int> * sequence,
                      bool simulate) const
{
    return run(start, Direction::D_FORWARD, max_depth,
               user, sequence, simulate);
}

Err ModelNet::backward(std::set<int> const & start,
                       std::size_t max_depth,
                       void * user,
                       std::vector<int> * sequence,
                       bool simulate) const
{
    return run(start, Direction::D_BACKWARD, max_depth,
               user, sequence, simulate);
}

Err ModelNet::dependencyForward(std::set<int> const & start,
                                std::size_t max_depth,
                                void * user,
                                std::vector<int> * sequence,
                                bool simulate,
                                bool skip_first_dep_test) const
{
    return dependencyRun(start, Direction::D_FORWARD, max_depth, user,
                         sequence, simulate, skip_first_dep_test);
}

Err ModelNet::dependencyBackward(std::set<int> const & start,
                                 std::size_t max_depth,
                                 void * user,
                                 std::vector<int> * sequence,
                                 bool simulate,
                                 bool skip_first_dep_test) const
{
    return dependencyRun(start, Direction::D_BACKWARD, max_depth, user,
                         sequence, simulate, skip_first_dep_test);
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

