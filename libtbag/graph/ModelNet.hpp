/**
 * @file   ModelNet.hpp
 * @brief  ModelNet class prototype.
 * @author zer0
 * @date   2018-10-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELNET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELNET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/graph/ModelLayer.hpp>

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <set>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

/**
 * ModelNet class prototype.
 *
 * @author zer0
 * @date   2018-10-22
 */
class TBAG_API ModelNet
{
public:
    using Layers = std::vector<ModelLayer>;
    using Direction = LayerBase::Direction;

public:
    enum class ArcOrder
    {
        AO_SOURCE,
        AO_TARGET,
    };

public:
    TBAG_CONSTEXPR static std::size_t const MAX_RUN_DEPTH = 1024;

public:
    struct Impl;
    friend struct Impl;
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    ModelNet() TBAG_NOEXCEPT;
    explicit ModelNet(std::nullptr_t) TBAG_NOEXCEPT;
    ModelNet(ModelNet const & obj) TBAG_NOEXCEPT;
    ModelNet(ModelNet && obj) TBAG_NOEXCEPT;
    ~ModelNet();

public:
    ModelNet & operator =(ModelNet const & obj) TBAG_NOEXCEPT;
    ModelNet & operator =(ModelNet && obj) TBAG_NOEXCEPT;

public:
    void copy(ModelNet const & obj) TBAG_NOEXCEPT;
    void swap(ModelNet & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(ModelNet & lh, ModelNet & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    void clear();

public:
    bool empty() const;
    std::size_t size() const;

private:
    /** Unplug the finished flags. */
    void updateIncomplete();
    void updateComplete();

public:
    void addFirst(ModelLayer const & layer);
    void addNode(ModelLayer const & layer);
    void addLast(ModelLayer const & layer);
    void addArc(ModelLayer const & source, ModelLayer const & target);

public:
    std::vector<int> getLayerIds() const;
    std::set<ModelLayer> getLayers() const;
    ModelLayer getLayer(int id) const;

public:
    std::vector<int> getSourceNodeIds(int node_id) const;
    std::vector<int> getTargetNodeIds(int node_id) const;
    std::vector<int> getNodeIds(int node_id, ArcOrder order) const;

public:
    Layers getInputLayers(int node_id, ArcOrder order) const;
    bool isReady(int node_id, ArcOrder order) const;

private:
    Err run(std::set<int> const & start_node_ids,
            Direction direction = Direction::D_FORWARD,
            std::size_t max_depth = MAX_RUN_DEPTH,
            void * user = nullptr);

public:
    Err forward(std::size_t max_depth, void * user = nullptr);
    Err backward(std::size_t max_depth, void * user = nullptr);

public:
    Err forward(void * user = nullptr);
    Err backward(void * user = nullptr);

public:
    std::string toString() const;
};

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELNET_HPP__

