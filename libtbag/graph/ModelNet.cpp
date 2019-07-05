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
    assert(exists());
    _impl->clear();
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
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;

    for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
        if (layers[n]) {
            layers[n]._incomplete();
        }
    }
}

void ModelNet::updateComplete()
{
    assert(exists());
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;

    for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
        if (layers[n]) {
            layers[n]._complete();
        }
    }
}

void ModelNet::addFirst(ModelLayer const & layer)
{
    assert(exists());
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;
    auto & first_ids = _impl->first_ids;

    auto const NODE = graph.addNode();
    auto const ID = __get_id(NODE);
    first_ids.insert(ID);
    layers[NODE] = layer;
    layers[NODE]._assign_id(ID);
}

void ModelNet::addNode(ModelLayer const & layer)
{
    assert(exists());
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;

    auto node = graph.addNode();
    layers[node] = layer;
    layers[node]._assign_id(__get_id(node));
}

void ModelNet::addLast(ModelLayer const & layer)
{
    assert(exists());
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;
    auto & last_ids = _impl->last_ids;

    auto const NODE = graph.addNode();
    auto const ID = __get_id(NODE);
    last_ids.insert(ID);
    layers[NODE] = layer;
    layers[NODE]._assign_id(ID);
}

void ModelNet::addArc(ModelLayer const & source, ModelLayer const & target)
{
    assert(exists());
    _impl->graph.addArc(__get_node(source.id()), __get_node(target.id()));
}

std::vector<int> ModelNet::getLayerIds() const
{
    assert(exists());
    auto & graph = _impl->graph;

    std::vector<int> result;
    for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
        result.push_back(__get_id(n));
    }
    return result;
}

std::set<ModelLayer> ModelNet::getLayers() const
{
    assert(exists());
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;

    std::set<ModelLayer> result;
    for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
        result.insert(layers[n]);
    }
    return result;
}

ModelLayer ModelNet::getLayer(int id) const
{
    assert(exists());
    auto & layers = _impl->layers;

    if (id == LayerBase::NO_ASSIGN_ID) {
        return ModelLayer(nullptr);
    }
    return layers[__get_node(id)];
}

std::size_t ModelNet::setup(std::string const & data)
{
    assert(exists());
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;

    // TODO: SetUp self network.

    std::size_t success_count = 0;
    for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
        std::string layer_data;
        // TODO: Splits the data into layer_data.
        if (layers[n]) {
            if (isSuccess(layers[n].setup(layer_data))) {
                ++success_count;
            }
        }
    }
    return success_count;
}

std::size_t ModelNet::teardown()
{
    assert(exists());
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;

    std::size_t success_count = 0;
    for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
        if (layers[n]) {
            if (isSuccess(layers[n].teardown())) {
                ++success_count;
            }
        }
    }

    // TODO: TearDown self network.

    return success_count;
}

std::vector<int> ModelNet::getSourceNodeIds(int node_id) const
{
    assert(exists());
    auto & graph = _impl->graph;

    std::vector<int> result;
    for (Digraph::InArcIt a(graph, __get_node(node_id)); a != lemon::INVALID; ++a) {
        result.push_back(__get_id(graph.source(a)));
    }
    return result;
}

std::vector<int> ModelNet::getTargetNodeIds(int node_id) const
{
    assert(exists());
    auto & graph = _impl->graph;

    std::vector<int> result;
    for (Digraph::OutArcIt a(graph, __get_node(node_id)); a != lemon::INVALID; ++a) {
        result.push_back(__get_id(graph.target(a)));
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

ModelNet::Layers ModelNet::getInputLayers(int node_id, ArcOrder order) const
{
    assert(exists());
    auto & layers = _impl->layers;

    std::vector<ModelLayer> input_layers;
    for (auto & source_id : getNodeIds(node_id, order)) {
        input_layers.push_back(layers[__get_node(source_id)]);
    }
    return input_layers;
}

bool ModelNet::isReady(int node_id, ArcOrder order) const
{
    assert(exists());
    auto & layers = _impl->layers;

    for (auto & source_id : getNodeIds(node_id, order)) {
        if (!layers[__get_node(source_id)].isComplete()) {
            return false;
        }
    }
    return true;
}

Err ModelNet::run(std::set<int> const & start_node_ids, Direction direction, std::size_t max_depth)
{
    assert(exists());
    auto & layers = _impl->layers;

    updateIncomplete();

    std::set<int> current = start_node_ids;
    std::set<int> children;

    for (std::size_t current_depth = 0; current_depth < max_depth; ++current_depth) {
        if (current.empty()) {
            break; // No more current node exist.
        }

        // Run current list.
        for (int current_id : current) {
            Err code;

            if (direction == Direction::D_FORWARD) {
                code = layers[__get_node(current_id)].forward(getInputLayers(current_id, ArcOrder::AO_SOURCE));
            } else {
                assert(direction == Direction::D_BACKWARD);
                code = layers[__get_node(current_id)].backward(getInputLayers(current_id, ArcOrder::AO_TARGET));
            }

            if (isSuccess(code)) {
                layers[__get_node(current_id)]._complete();
            } else {
                tDLogE("ModelNet::Impl::run({}) Layer({}) error: {}",
                       getDirectionName(direction), current_id, code);
                return code;
            }
        }

        // Update children list.
        children.clear();
        for (int current_id : current) {
            if (direction == Direction::D_FORWARD) {
                for (auto & next_node_id : getTargetNodeIds(current_id)) {
                    if (isReady(next_node_id, ArcOrder::AO_SOURCE)) {
                        children.insert(next_node_id);
                    }
                }
            } else {
                assert(direction == Direction::D_BACKWARD);
                for (auto & next_node_id : getSourceNodeIds(current_id)) {
                    if (isReady(next_node_id, ArcOrder::AO_TARGET)) {
                        children.insert(next_node_id);
                    }
                }
            }
        }

        // Flip for next iteration.
        current.swap(children);
    }

    return E_SUCCESS;
}

Err ModelNet::forward()
{
    assert(exists());
    auto & first_ids = _impl->first_ids;

    if (first_ids.empty()) {
        return E_NREADY;
    }
    return run(first_ids, Direction::D_FORWARD);
}

Err ModelNet::backward()
{
    assert(exists());
    auto & last_ids = _impl->last_ids;

    if (last_ids.empty()) {
        return E_NREADY;
    }
    return run(last_ids, Direction::D_BACKWARD);
}

std::string ModelNet::toString() const
{
    assert(exists());
    auto & graph = _impl->graph;
    auto & layers = _impl->layers;
    auto & first_ids = _impl->first_ids;
    auto & last_ids = _impl->last_ids;

    bool first_node = true;
    int layer_count = 0;
    std::stringstream ss;

    for (Digraph::NodeIt n(graph); n != lemon::INVALID; ++n, ++layer_count) {
        if (first_node) {
            first_node = false;
        } else {
            ss << std::endl;
        }

        ss << "Layer #" << layer_count << ": " << layers[n].toString();
        auto const CURRENT_NODE_ID = __get_id(n);

        if (first_ids.find(CURRENT_NODE_ID) != first_ids.end()) {
            ss << " [FIRST]";
        } else if (last_ids.find(CURRENT_NODE_ID) != last_ids.end()) {
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

