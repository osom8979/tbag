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

/**
 * Pointer implementation.
 *
 * @author zer0
 * @date   2018-10-22
 */
struct ModelNet::Impl : private Noncopyable
{
public:
    using Digraph  = lemon::ListDigraph;
    using LayerMap = lemon::ListDigraph::NodeMap<ModelLayer>;

    using Node = Digraph::Node;
    using Arc  = Digraph::Arc;

private:
    ModelNet * _parent = nullptr;

private:
    Digraph  _graph;
    LayerMap _layers;

private:
    std::set<int> _first_ids;
    std::set<int> _last_ids;

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

public:
    Err addFirst(ModelLayer const & layer)
    {
        auto const NODE = _graph.addNode();
        auto const ID = getId(NODE);
        _first_ids.insert(ID);
        _layers[NODE] = layer;
        _layers[NODE].setId(ID);
        return Err::E_SUCCESS;
    }

    Err addNode(ModelLayer const & layer)
    {
        auto node = _graph.addNode();
        _layers[node] = layer;
        _layers[node].setId(getId(node));
        return Err::E_SUCCESS;
    }

    Err addLast(ModelLayer const & layer)
    {
        auto const NODE = _graph.addNode();
        auto const ID = getId(NODE);
        _last_ids.insert(ID);
        _layers[NODE] = layer;
        _layers[NODE].setId(ID);
        return Err::E_SUCCESS;
    }

    Err addArc(ModelLayer const & source, ModelLayer const & target)
    {
        _graph.addArc(getNode(source.getId()), getNode(target.getId()));
        return Err::E_SUCCESS;
    }

public:
    std::vector<int> getLayerIds() const
    {
        std::vector<int> result;
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            result.push_back(getId(n));
        }
        return result;
    }

    std::set<ModelLayer> getLayers() const
    {
        std::set<ModelLayer> result;
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            result.insert(_layers[n]);
        }
        return result;
    }

    ModelLayer getLayer(int id) const
    {
        return _layers[getNode(id)];
    }

public:
    void clear()
    {
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            _layers[n] = ModelLayer(nullptr);
        }
        _graph.clear();
        _first_ids.clear();
        _last_ids.clear();
    }

    bool empty() const
    {
        return Digraph::NodeIt(_graph) == lemon::INVALID;
    }

    std::size_t size() const
    {
        std::size_t result = 0;
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            ++result;
        }
        return result;
    }

public:
    enum class ArcOrder
    {
        AO_SOURCE,
        AO_TARGET,
    };

    enum class Direction
    {
        D_FORWARD,
        D_BACKWARD,
    };

    inline static char const * getDirectionName(Direction direction) TBAG_NOEXCEPT
    {
        if (direction == Direction::D_FORWARD) {
            return "FORWARD";
        } else {
            assert(direction == Direction::D_BACKWARD);
            return "BACKWARD";
        }
    }

public:
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

    std::vector<int> getNodeIds(int node_id, ArcOrder order) const
    {
        if (order == ArcOrder::AO_SOURCE) {
            return getSourceNodeIds(node_id);
        } else {
            assert(order == ArcOrder::AO_TARGET);
            return getTargetNodeIds(node_id);
        }
    }

public:
    /** Unplug the finished flags. */
    void updateIncomplete()
    {
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            if (_layers[n]) {
                _layers[n].incomplete();
            }
        }
    }

    void updateComplete()
    {
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            if (_layers[n]) {
                _layers[n].complete();
            }
        }
    }

public:
    Err setup(std::string const & data)
    {
        // TODO: SetUp self network.
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            std::string layer_data;
            // TODO: Splits the data into layer_data.
            if (_layers[n]) {
                _layers[n].setup(layer_data);
            }
        }
        return Err::E_SUCCESS;
    }

    Err teardown()
    {
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            if (_layers[n]) {
                _layers[n].teardown();
            }
        }
        // TODO: TearDown self network.
        return Err::E_SUCCESS;
    }

public:
    TBAG_CONSTEXPR static std::size_t const MAX_RUN_DEPTH = 1024;

public:
    Layers getInputLayers(int node_id, ArcOrder order)
    {
        std::vector<ModelLayer> input_layers;
        for (auto & source_id : getNodeIds(node_id, order)) {
            input_layers.push_back(_layers[getNode(source_id)]);
        }
        return input_layers;
    }

    bool isReady(int node_id, ArcOrder order)
    {
        for (auto & source_id : getNodeIds(node_id, order)) {
            if (!_layers[getNode(source_id)].isComplete()) {
                return false;
            }
        }
        return true;
    }

    Err run(std::set<int> const & start_node_ids, Direction direction, std::size_t max_depth = MAX_RUN_DEPTH)
    {
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
                    code = _layers[getNode(current_id)].forward(getInputLayers(current_id, ArcOrder::AO_SOURCE));
                } else {
                    assert(direction == Direction::D_BACKWARD);
                    code = _layers[getNode(current_id)].backward(getInputLayers(current_id, ArcOrder::AO_TARGET));
                }

                if (isSuccess(code)) {
                    _layers[getNode(current_id)].complete();
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

        return Err::E_SUCCESS;
    }

    Err forward()
    {
        if (_first_ids.empty()) {
            tDLogE("ModelNet::Impl::forward() Not found first nodes.");
            return Err::E_NREADY;
        }
        return run(_first_ids, Direction::D_FORWARD);
    }

    Err backward()
    {
        if (_last_ids.empty()) {
            tDLogE("ModelNet::Impl::forward() Not found last nodes.");
            return Err::E_NREADY;
        }
        return run(_last_ids, Direction::D_BACKWARD);
    }

public:
    Err toData(Buffer & output) const
    {
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            if (_layers[n]) {
                Buffer current_data;
                auto const CODE = _layers[n].toData(current_data);
                if (isSuccess(CODE)) {
                    // TODO: Output append
                }
            }
        }
        return Err::E_SUCCESS;
    }

    Err fromData(Buffer const & input)
    {
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            if (_layers[n]) {
                _layers[n].fromData(input);
            }
        }
        return Err::E_SUCCESS;
    }

public:
    std::string toString() const
    {
        bool first_node  = true;
        int  layer_count = 0;
        std::stringstream ss;

        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n, ++layer_count) {
            if (first_node) {
                first_node = false;
            } else {
                ss << std::endl;
            }

            ss << "Layer #" << layer_count << ": " << _layers[n].toString();
            auto const CURRENT_NODE_ID = getId(n);

            if (_first_ids.find(CURRENT_NODE_ID) != _first_ids.end()) {
                ss << " [FIRST]";
            } else if (_last_ids.find(CURRENT_NODE_ID) != _last_ids.end()) {
                ss << " [LAST]";
            }

            bool first_arc = true;
            for (Digraph::InArcIt in(_graph, n); in != lemon::INVALID; ++in) {
                if (first_arc) {
                    ss << std::endl << " - in: " << getId(_graph.source(in));
                    first_arc = false;
                } else {
                    ss << ", " << getId(_graph.source(in));
                }
            }

            first_arc = true;
            for (Digraph::OutArcIt out(_graph, n); out != lemon::INVALID; ++out) {
                if (first_arc) {
                    ss << std::endl << " - out: " << getId(_graph.target(out));
                    first_arc = false;
                } else {
                    ss << ", " << getId(_graph.target(out));
                }
            }
        }

        return ss.str();
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
    if (exists()) {
        _impl->clear();
    }
}

bool ModelNet::empty() const
{
    if (exists()) {
        return _impl->empty();
    }
    return true;
}

std::size_t ModelNet::size() const
{
    if (exists()) {
        return _impl->size();
    }
    return 0;
}

void ModelNet::updateIncomplete()
{
    if (exists()) {
        _impl->updateIncomplete();
    }
}

void ModelNet::updateComplete()
{
    if (exists()) {
        _impl->updateComplete();
    }
}

Err ModelNet::addFirst(ModelLayer const & layer)
{
    if (exists()) {
        return _impl->addFirst(layer);
    }
    return Err::E_NREADY;
}

Err ModelNet::addNode(ModelLayer const & layer)
{
    if (exists()) {
        return _impl->addNode(layer);
    }
    return Err::E_NREADY;
}

Err ModelNet::addLast(ModelLayer const & layer)
{
    if (exists()) {
        return _impl->addLast(layer);
    }
    return Err::E_NREADY;
}

Err ModelNet::addArc(ModelLayer const & source, ModelLayer const & target)
{
    if (exists()) {
        return _impl->addArc(source, target);
    }
    return Err::E_NREADY;
}

std::vector<int> ModelNet::getLayerIds() const
{
    if (exists()) {
        return _impl->getLayerIds();
    }
    return std::vector<int>();
}

std::set<ModelLayer> ModelNet::getLayers() const
{
    if (exists()) {
        return _impl->getLayers();
    }
    return std::set<ModelLayer>();
}

ModelLayer ModelNet::getLayer(int id) const
{
    if (0 <= id && exists()) {
        return _impl->getLayer(id);
    }
    return ModelLayer(nullptr);
}

Err ModelNet::setup(std::string const & data)
{
    if (exists()) {
        return _impl->setup(data);
    }
    return Err::E_NREADY;
}

Err ModelNet::teardown()
{
    if (exists()) {
        return _impl->teardown();
    }
    return Err::E_NREADY;
}

Err ModelNet::forward()
{
    if (exists()) {
        return _impl->forward();
    }
    return Err::E_NREADY;
}

Err ModelNet::backward()
{
    if (exists()) {
        return _impl->backward();
    }
    return Err::E_NREADY;
}

Err ModelNet::toData(Buffer & output) const
{
    if (exists()) {
        return _impl->toData(output);
    }
    return Err::E_NREADY;
}

Err ModelNet::fromData(Buffer const & input)
{
    if (exists()) {
        return _impl->fromData(input);
    }
    return Err::E_NREADY;
}

std::string ModelNet::toString() const
{
    if (exists()) {
        return _impl->toString();
    }
    return std::string();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

