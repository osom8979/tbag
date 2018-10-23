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

public:
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

public:
    void clear()
    {
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            _layers[n] = ModelLayer(nullptr);
        }
        _graph.clear();
        _first = Node();
        _last = Node();
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

public:
    TBAG_CONSTEXPR static std::size_t const MAX_RUN_DEPTH = 1024;

public:
    std::vector<ModelLayer> getInputLayers(int node_id, ArcOrder order)
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

    Err run(int start_node_id, Direction direction, std::size_t max_depth = MAX_RUN_DEPTH)
    {
        updateIncomplete();

        std::set<int> current = {start_node_id};
        std::set<int> children;

        for (std::size_t current_depth = 0; current_depth < max_depth; ++current_depth) {
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

            if (children.empty()) {
                break; // No more children exist.
            }

            // Flip for next iteration.
            current.swap(children);
        }

        return Err::E_SUCCESS;
    }

    Err forward()
    {
        if (_first == Node()) {
            return Err::E_NREADY;
        }
        return run(getId(_first), Direction::D_FORWARD);
    }

    Err backward()
    {
        if (_last == Node()) {
            return Err::E_NREADY;
        }
        return run(getId(_last), Direction::D_BACKWARD);
    }

    std::string toString() const
    {
        std::stringstream ss;
        for (Digraph::NodeIt n(_graph); n != lemon::INVALID; ++n) {
            ss << "Layer(" << getId(n) << ") IN(";
            for (Digraph::InArcIt in(_graph, n); in != lemon::INVALID; ++in) {
                ss << "," << getId(_graph.source(in));
            }
            ss << ") OUT(";
            for (Digraph::OutArcIt out(_graph, n); out != lemon::INVALID; ++out) {
                ss << "," << getId(_graph.target(out));
            }
            ss << ")" << std::endl;
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
    assert(exists());
    _impl->clear();
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
    assert(exists());
    return _impl->toString();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

