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

public:
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
    std::vector<int> getChildrenNodeIds(int node_id, Direction direction) const;

public:
    Layers getInputLayers(int node_id, ArcOrder order) const;
    Layers getInputLayers(int node_id, Direction direction) const;

public:
    bool isReady(int node_id, ArcOrder order) const;
    bool isChildrenReady(int node_id, Direction direction) const;

public:
    /**
     * @param[in] start
     *      List of start node ids.
     * @param[in] direction
     *      Loop direction. (forward or backward)
     * @param[in] max_depth
     *      Maximum loop iteration.
     * @param[in] user
     *      User's data.
     * @param[out] sequence
     *      Returns the result in the task sequence.
     * @params[in] simulate
     *      If this value is true, the child layer is not executed.
     */
    Err run(std::set<int> const & start,
            Direction direction = Direction::D_FORWARD,
            std::size_t max_depth = MAX_RUN_DEPTH,
            void * user = nullptr,
            std::vector<int> * sequence = nullptr,
            bool simulate = false);

public:
    Err forward(std::size_t max_depth, void * user = nullptr);
    Err backward(std::size_t max_depth, void * user = nullptr);

public:
    Err forward(void * user = nullptr);
    Err backward(void * user = nullptr);

public:
    std::vector<int> getForwardSequence(std::size_t max_depth = MAX_RUN_DEPTH,
                                        void * user = nullptr,
                                        Err * run_result = nullptr,
                                        bool simulate = true);
    std::vector<int> getBackwardSequence(std::size_t max_depth = MAX_RUN_DEPTH,
                                         void * user = nullptr,
                                         Err * run_result = nullptr,
                                         bool simulate = true);

public:
    std::string toString() const;
};

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELNET_HPP__

