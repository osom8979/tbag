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
#include <sstream>

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

    std::set<int> first_ids;
    std::set<int> last_ids;

    Impl() : graph(), layers(graph), first_ids(), last_ids()
    {
        // EMPTY.
    }

    ~Impl()
    {
        clear();
    }

    void clear()
    {
        for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
            layers[n] = ModelLayer(nullptr);
        }
        graph.clear();
        first_ids.clear();
        last_ids.clear();
    }
};

// --------
// ModelNet
// --------

ModelNet::ModelNet() TBAG_NOEXCEPT : _impl(std::make_shared<Impl>())
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
    if (exists()) {
        _impl->clear();
    }
}

bool ModelNet::empty() const
{
    if (!exists()) {
        return true;
    }
    return Digraph::NodeIt(_impl->graph) == lemon::INVALID;
}

std::size_t ModelNet::size() const
{
    if (!exists()) {
        return 0;
    }
    std::size_t result = 0;
    for (Digraph::NodeIt n(_impl->graph); n != lemon::INVALID; ++n) {
        ++result;
    }
    return result;
}

void ModelNet::updateIncomplete()
{
    assert(exists());
    for (Digraph::NodeIt n(_impl->graph); n != lemon::INVALID; ++n) {
        if (_impl->layers[n]) {
            _impl->layers[n]._incomplete();
        }
    }
}

void ModelNet::updateComplete()
{
    assert(exists());
    for (Digraph::NodeIt n(_impl->graph); n != lemon::INVALID; ++n) {
        if (_impl->layers[n]) {
            _impl->layers[n]._complete();
        }
    }
}

void ModelNet::addFirst(ModelLayer const & layer)
{
    assert(exists());
    auto const NODE = _impl->graph.addNode();
    auto const ID = __get_id(NODE);
    _impl->first_ids.insert(ID);
    _impl->layers[NODE] = layer;
    _impl->layers[NODE]._assign_id(ID);
}

void ModelNet::addNode(ModelLayer const & layer)
{
    assert(exists());
    auto node = _impl->graph.addNode();
    _impl->layers[node] = layer;
    _impl->layers[node]._assign_id(__get_id(node));
}

void ModelNet::addLast(ModelLayer const & layer)
{
    assert(exists());
    auto const NODE = _impl->graph.addNode();
    auto const ID = __get_id(NODE);
    _impl->last_ids.insert(ID);
    _impl->layers[NODE] = layer;
    _impl->layers[NODE]._assign_id(ID);
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

std::set<int> ModelNet::getFirstIds() const
{
    assert(exists());
    return _impl->first_ids;
}

std::set<int> ModelNet::getLastIds() const
{
    assert(exists());
    return _impl->last_ids;
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

bool ModelNet::isReady(int node_id, ArcOrder order) const
{
    assert(exists());
    for (auto & source_id : getNodeIds(node_id, order)) {
        if (!_impl->layers[__get_node(source_id)].isComplete()) {
            return false;
        }
    }
    return true;
}

bool ModelNet::isChildrenReady(int node_id, Direction direction) const
{
    return isReady(node_id, direction == Direction::D_FORWARD ? ArcOrder::AO_SOURCE : ArcOrder::AO_TARGET);
}

Err ModelNet::run(std::set<int> const & start,
                  Direction direction,
                  std::size_t max_depth,
                  void * user,
                  std::vector<int> * sequence,
                  bool simulate)
{
    assert(exists());

    if (start.empty()) {
        return E_NREADY;
    }

    std::set<int> current = start;
    std::set<int> children;

    updateIncomplete();

    for (auto i = 0; i < max_depth; ++i) {
        if (current.empty()) {
            break; // No more current node exist.
        }

        // Run current list.
        for (int id : current) {
            auto & layer = _impl->layers[__get_node(id)];

            Err code;
            if (simulate) {
                code = E_SUCCESS;
            } else {
                code = layer.runner(direction, getInputLayers(id, direction), user);
                if (isFailure(code)) {
                    return code;
                }
            }

            assert(isSuccess(code));
            if (sequence != nullptr) {
                sequence->push_back(id);
            }
            layer._complete();
        }

        // Update children list.
        children.clear();

        for (int id : current) {
            for (auto & next_node_id : getChildrenNodeIds(id, direction)) {
                if (isChildrenReady(next_node_id, direction)) {
                    children.insert(next_node_id);
                }
            }
        }

        // Flip for next iteration.
        current.swap(children);
    }

    return E_SUCCESS;
}

Err ModelNet::forward(std::size_t max_depth, void * user)
{
    return run(_impl->first_ids, Direction::D_FORWARD, max_depth, user);
}

Err ModelNet::backward(std::size_t max_depth, void * user)
{
    return run(_impl->last_ids, Direction::D_BACKWARD, max_depth, user);
}

Err ModelNet::forward(void * user)
{
    return forward(MAX_RUN_DEPTH, user);
}

Err ModelNet::backward(void * user)
{
    return backward(MAX_RUN_DEPTH, user);
}

std::vector<int> ModelNet::getForwardSequence(std::size_t max_depth, void * user, Err * run_result, bool simulate)
{
    std::vector<int> result;
    auto const code = run(_impl->first_ids, Direction::D_FORWARD, max_depth, user, &result, simulate);
    if (run_result != nullptr) {
        *run_result = code;
    }
    if (isFailure(code)) {
        return {};
    }
    return result;
}

std::vector<int> ModelNet::getBackwardSequence(std::size_t max_depth, void * user, Err * run_result, bool simulate)
{
    std::vector<int> result;
    auto const code = run(_impl->last_ids, Direction::D_BACKWARD, max_depth, user, &result, simulate);
    if (run_result != nullptr) {
        *run_result = code;
    }
    if (isFailure(code)) {
        return {};
    }
    return result;
}

std::string ModelNet::toString() const
{
    assert(exists());
    bool first_node = true;
    int layer_count = 0;
    std::stringstream ss;

    auto & graph = _impl->graph;

    for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n, ++layer_count) {
        if (first_node) {
            first_node = false;
        } else {
            ss << std::endl;
        }

        ss << "Layer #" << layer_count << ": " << _impl->layers[n].toString();
        auto const CURRENT_NODE_ID = __get_id(n);

        if (_impl->first_ids.find(CURRENT_NODE_ID) != _impl->first_ids.end()) {
            ss << " [FIRST]";
        } else if (_impl->last_ids.find(CURRENT_NODE_ID) != _impl->last_ids.end()) {
            ss << " [LAST]";
        }

        bool first_arc = true;
        for (Digraph::InArcIt in(graph, n); in != lemon::INVALID; ++in) {
            if (first_arc) {
                ss << std::endl << " - in: " << __get_id(graph.source(in));
                first_arc = false;
            } else {
                ss << ", " << __get_id(graph.source(in));
            }
        }

        first_arc = true;
        for (Digraph::OutArcIt out(graph, n); out != lemon::INVALID; ++out) {
            if (first_arc) {
                ss << std::endl << " - out: " << __get_id(graph.target(out));
                first_arc = false;
            } else {
                ss << ", " << __get_id(graph.target(out));
            }
        }
    }

    return ss.str();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

