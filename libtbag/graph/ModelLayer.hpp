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
#include <libtbag/container/Box.hpp>
#include <libtbag/util/BufferInfo.hpp>

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
    using Buffer = libtbag::util::Buffer;
    using Box  = libtbag::container::Box;
    using BagExs = std::vector<Box>;
    using Layers = std::vector<ModelLayer>;

public:
    TBAG_CONSTEXPR static int const UNKNOWN_ID = -1;

public:
    /**
     * LayerBase class prototype.
     *
     * @author zer0
     * @date   2018-10-23
     */
    class LayerBase : public Noncopyable
    {
    public:
        using Buffer = ModelLayer::Buffer;
        using Box    = ModelLayer::Box;
        using BagExs = ModelLayer::BagExs;
        using Layers = ModelLayer::Layers;

    public:
        friend class ModelLayer;

    protected:
        int  _id       = UNKNOWN_ID;
        bool _complete = false;

    private:
        std::string _name;

    public:
        LayerBase() { /* EMPTY. */ }
        LayerBase(std::string const & name) : _name(name) { /* EMPTY. */ }
        virtual ~LayerBase() { /* EMPTY. */ }

    public:
        inline int getId() const TBAG_NOEXCEPT { return _id; }
        inline bool isComplete() const TBAG_NOEXCEPT { return _complete; }

    public:
        std::string getName() const { return _name; }
        void setName(std::string const & name) { _name = name; }

    public:
        virtual Err setup(std::string const & data) { return Err::E_SUCCESS; }
        virtual Err teardown() { return Err::E_SUCCESS; }

    public:
        virtual Err  forward(Layers const & input) { return Err::E_SUCCESS; }
        virtual Err backward(Layers const & input) { return Err::E_SUCCESS; }

    public:
        virtual Err toData(Buffer & output) const { return Err::E_SUCCESS; }
        virtual Err fromData(Buffer const & input) { return Err::E_SUCCESS; }

    public:
        virtual std::string get(std::string const & key) const { return std::string(); }
        virtual void set(std::string const & key, std::string const & val) { /* EMPTY. */ }
    };

    using SharedBase = std::shared_ptr<LayerBase>;

public:
    /**
     * LogLayer class prototype.
     *
     * @author zer0
     * @date   2018-10-24
     */
    class TBAG_API LogLayer : public LayerBase
    {
    private:
        bool _verbose;

    public:
        LogLayer(bool verbose = false);
        virtual ~LogLayer();

    public:
        inline bool isVerbose() const TBAG_NOEXCEPT { return _verbose; }
        inline void setVerbose(bool enable = true) TBAG_NOEXCEPT { _verbose = enable; }

    public:
        virtual Err setup(std::string const & data) override;
        virtual Err teardown() override;

    public:
        virtual Err  forward(Layers const & input) override;
        virtual Err backward(Layers const & input) override;
    };

private:
    SharedBase _base;

public:
    ModelLayer();
    explicit ModelLayer(int id);
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
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend bool operator <(ModelLayer const & x, ModelLayer const & y)
    {
        return x.getId() < y.getId();
    }

public:
    bool operator ==(ModelLayer const & obj) const TBAG_NOEXCEPT
    {
        return _base.get() == obj.get();
    }

public:
    int getId() const;
    bool isComplete() const;
    std::string getName() const;

protected:
    void setId(int id);
    void complete();
    void incomplete();

public:
    Err setup(std::string const & data);
    Err teardown();

public:
    Err  forward(Layers const & input);
    Err backward(Layers const & input);

public:
    Err toData(Buffer & output) const;
    Err fromData(Buffer const & input);

public:
    std::string get(std::string const & key) const;
    void set(std::string const & key, std::string const & val);

public:
    std::string toString() const;

public:
    template <typename LayerType, typename ... Args>
    static ModelLayer create(Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(LayerBase, LayerType);
        STATIC_ASSERT_CHECK_IS_DEFAULT_CONSTRUCTIBLE(LayerType);
        typedef typename remove_cr<LayerType>::type ResultLayerType;

        auto shared = std::make_shared<ResultLayerType>(std::forward<Args>(args) ...);
        if (static_cast<bool>(shared)) {
            return ModelLayer(std::static_pointer_cast<ModelLayer::LayerBase>(shared));
        }
        return ModelLayer(nullptr);
    }

    static ModelLayer createLog(bool verbose = true)
    {
        return create<LogLayer>(verbose);
    }
};

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELLAYER_HPP__

