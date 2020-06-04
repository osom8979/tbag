/**
 * @file   ModelLayer.hpp
 * @brief  ModelLayer class prototype.
 * @author zer0
 * @date   2018-10-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELLAYER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELLAYER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <cstring>

#include <memory>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

// Forward declarations.
class ModelNet;
class ModelLayer;

/**
 * LayerBase class prototype.
 *
 * @author zer0
 * @date   2018-10-23
 */
class LayerBase
{
public:
    friend class ModelLayer;

public:
    using Err = libtbag::Err;

public:
    enum class Direction
    {
        D_FORWARD,
        D_BACKWARD,
    };

public:
    struct RunnerInfo
    {
        Direction direction;
        std::size_t depth;
        int previous_id;
        Err previous_code;
        std::vector<int> parent_ids;
        std::vector<int> children_ids;
        void * user;
    };

public:
    TBAG_CONSTEXPR static int const NO_ASSIGN_ID = -1;

private:
    /**
     * Layer instance id.
     *
     * @warning
     *  This value is assigned when added to the model. @n
     *  The user should not modify this value.
     */
    int _assign_id = NO_ASSIGN_ID;

public:
    /** User's data. */
    void * opaque = nullptr;

public:
    LayerBase() { /* EMPTY. */ }
    virtual ~LayerBase() { /* EMPTY. */ }

public:
    inline bool isAssigned() const TBAG_NOEXCEPT
    { return _assign_id != NO_ASSIGN_ID; }

    inline int id() const TBAG_NOEXCEPT
    { return _assign_id; }

public:
    virtual Err runner(RunnerInfo & info)
    { return E_SUCCESS; }
};

/**
 * ModelLayer class prototype.
 *
 * @author zer0
 * @date   2018-10-22
 */
class TBAG_API ModelLayer
{
public:
    friend class ModelNet;

public:
    using Err = libtbag::Err;
    using Layers = std::vector<ModelLayer>;
    using SharedBase = std::shared_ptr<LayerBase>;
    using Direction = LayerBase::Direction;
    using RunnerInfo = LayerBase::RunnerInfo;

public:
    TBAG_CONSTEXPR static int const NO_ASSIGN_ID = LayerBase::NO_ASSIGN_ID;

private:
    SharedBase _base;

public:
    ModelLayer();
    explicit ModelLayer(std::nullptr_t) TBAG_NOEXCEPT;
    explicit ModelLayer(SharedBase base) TBAG_NOEXCEPT;
    ModelLayer(ModelLayer const & obj) TBAG_NOEXCEPT;
    ModelLayer(ModelLayer && obj) TBAG_NOEXCEPT;
    ~ModelLayer();

public:
    ModelLayer & operator =(ModelLayer const & obj) TBAG_NOEXCEPT;
    ModelLayer & operator =(ModelLayer && obj) TBAG_NOEXCEPT;

public:
    void swap(ModelLayer & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(ModelLayer & lh, ModelLayer & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline LayerBase       * get()       TBAG_NOEXCEPT { return _base.get(); }
    inline LayerBase const * get() const TBAG_NOEXCEPT { return _base.get(); }

    inline LayerBase       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline LayerBase const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline LayerBase       & ref()       TBAG_NOEXCEPT { return *get(); }
    inline LayerBase const & ref() const TBAG_NOEXCEPT { return *get(); }

    inline LayerBase       & operator *()       TBAG_NOEXCEPT { return ref(); }
    inline LayerBase const & operator *() const TBAG_NOEXCEPT { return ref(); }

public:
    template <typename LayerType = LayerBase>
    std::shared_ptr<LayerType> base() const TBAG_NOEXCEPT
    {
        return std::static_pointer_cast<LayerType>(_base);
    }

    template <typename T>
    inline T * cast() TBAG_NOEXCEPT
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(LayerBase, T);
        return (T*)_base.get();
    }

    template <typename T>
    inline T const * cast() const TBAG_NOEXCEPT
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(LayerBase, T);
        return (T const *)_base.get();
    }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_base); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

private:
    void _assign_id(int id);

public:
    int id() const;

public:
    Err runner(RunnerInfo & info);

public:
    template <typename LayerType, typename ... Args>
    static ModelLayer create(Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(LayerBase, LayerType);
        typedef typename libtbag::remove_cr<LayerType>::type ResultLayerType;

        auto shared = std::make_shared<ResultLayerType>(std::forward<Args>(args) ...);
        if (static_cast<bool>(shared)) {
            return ModelLayer(std::static_pointer_cast<LayerBase>(shared));
        }
        return ModelLayer(nullptr);
    }
};

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#include <utility>
#include <functional>

// ------------
namespace std {
// ------------

template <>
struct hash<libtbag::graph::ModelLayer>
{
    inline std::size_t operator()(libtbag::graph::ModelLayer const & v) const
    {
        return reinterpret_cast<std::size_t>(v.get());
    }
};

template <>
struct equal_to<libtbag::graph::ModelLayer>
{
    inline bool operator()(libtbag::graph::ModelLayer const & x,
                           libtbag::graph::ModelLayer const & y) const
    {
        return x.get() == y.get();
    }
};

template <>
struct less<libtbag::graph::ModelLayer>
{
    inline bool operator()(libtbag::graph::ModelLayer const & x,
                           libtbag::graph::ModelLayer const & y) const
    {
        return x.get() < y.get();
    }
};

// ---------------
} // namespace std
// ---------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELLAYER_HPP__

