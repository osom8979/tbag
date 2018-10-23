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
#include <libtbag/container/BagEx.hpp>

#include <memory>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

// Forward declarations.
class ModelNet;

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
    using BagEx  = libtbag::container::BagEx;
    using BagExs = std::vector<BagEx>;

public:
    TBAG_CONSTEXPR static int const UNKNOWN_ID = -1;

public:
    class LayerBase : public Noncopyable
    {
    public:
        using BagEx  = ModelLayer::BagEx;
        using BagExs = ModelLayer::BagExs;

    public:
        friend class ModelLayer;

    private:
        int  _id       = UNKNOWN_ID;
        bool _complete = false;

    public:
        std::string name;
        BagExs input;
        BagExs weight;
        BagExs output;

    public:
        LayerBase() { /* EMPTY. */ }
        virtual ~LayerBase() { /* EMPTY. */ }

    public:
        virtual bool    setup() { return true; }
        virtual bool teardown() { return true; }

    public:
        virtual bool  forward() { return true; }
        virtual bool backward() { return true; }
    };

    using SharedBase = std::shared_ptr<LayerBase>;

private:
    SharedBase _base;

public:
    ModelLayer();
    explicit ModelLayer(std::nullptr_t) TBAG_NOEXCEPT;
    explicit ModelLayer(LayerBase * base) TBAG_NOEXCEPT;
    explicit ModelLayer(SharedBase const & base) TBAG_NOEXCEPT;
    ModelLayer(ModelLayer const & obj) TBAG_NOEXCEPT;
    ModelLayer(ModelLayer && obj) TBAG_NOEXCEPT;
    ~ModelLayer();

public:
    ModelLayer & operator =(ModelLayer const & obj) TBAG_NOEXCEPT;
    ModelLayer & operator =(ModelLayer && obj) TBAG_NOEXCEPT;

public:
    void copy(ModelLayer const & obj) TBAG_NOEXCEPT;
    void swap(ModelLayer & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(ModelLayer & lh, ModelLayer & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline LayerBase       * get()       TBAG_NOEXCEPT { return _base.get(); }
    inline LayerBase const * get() const TBAG_NOEXCEPT { return _base.get(); }

    inline LayerBase       & operator *()       TBAG_NOEXCEPT { return *_base; }
    inline LayerBase const & operator *() const TBAG_NOEXCEPT { return *_base; }

    inline LayerBase       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline LayerBase const * operator ->() const TBAG_NOEXCEPT { return get(); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_base); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    int getId() const;

private:
    void setId(int id);

public:
    bool isComplete() const;

private:
    void complete();
    void incomplete();

public:
    Err forward(std::vector<ModelLayer> const & input);
    Err backward(std::vector<ModelLayer> const & input);

public:
    std::string toString() const;

public:
    template <typename LayerType, typename ... Args>
    ModelLayer create(Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(LayerBase, LayerType);
        typedef typename remove_cr<LayerType>::type ResultLayerType;
        auto shared = std::make_shared<ResultLayerType>(std::forward<Args>(args) ...);
        if (static_cast<bool>(shared)) {
            return ModelLayer(std::static_pointer_cast<ModelLayer::LayerBase>(shared));
        }
        return ModelLayer(nullptr);
    }
};

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELLAYER_HPP__

