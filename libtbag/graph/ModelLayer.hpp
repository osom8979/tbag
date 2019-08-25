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
    using Layers = std::vector<ModelLayer>;

public:
    enum class Direction
    {
        D_FORWARD,
        D_BACKWARD,
    };

public:
    TBAG_CONSTEXPR static int const NO_ASSIGN_ID = -1;
    TBAG_CONSTEXPR static int const DEFAULT_PROPERTY_BUFFER_SIZE = 1024;

private:
    /**
     * Layer class type.
     *
     * @warning
     *  Initialization using static strings is required.
     */
    char const * const TYPE;

private:
    /**
     * Layer instance id.
     *
     * @warning
     *  This value is assigned when added to the model. @n
     *  The user should not modify this value.
     */
    int _assign_id = NO_ASSIGN_ID;

    /**
     * Whether the operation(forward or backward) is complete.
     *
     * @warning
     *  This value is controlled by the model. @n
     *  The user should not modify this value.
     */
    bool _complete = false;

public:
    /** User's data. */
    void * opaque = nullptr;

public:
    LayerBase() : TYPE("UNKNOWN") { /* EMPTY. */ }
    LayerBase(char const * type) : TYPE(type) { /* EMPTY. */ }
    virtual ~LayerBase() { /* EMPTY. */ }

public:
    inline char const * type() const TBAG_NOEXCEPT
    { return TYPE; }

public:
    inline int id() const TBAG_NOEXCEPT
    { return _assign_id; }

    inline bool complete() const TBAG_NOEXCEPT
    { return _complete; }

public:
    virtual Err runner(Direction direction, Layers const & input, void * user)
    { return E_SUCCESS; }

public:
    virtual Err get(char const * key, char * buffer, int * size) const
    { return E_SUCCESS; }
    virtual Err set(char const * key, char const * data)
    { return E_SUCCESS; }

public:
    static Err getProperty(char * buffer, int * buffer_size, char const * value, int value_size)
    {
        *buffer_size = value_size;
        if (*buffer_size >= value_size) {
            ::strncpy(buffer, value, value_size);
            return E_SUCCESS;
        } else {
            return E_ENOBUFS;
        }
    }

    static Err getProperty(char * buffer, int * buffer_size, char const * value)
    {
        return getProperty(buffer, buffer_size, value, ::strlen(value));
    }

    static Err getProperty(char * buffer, int * buffer_size, std::string const & str)
    {
        return getProperty(buffer, buffer_size, str.c_str(), static_cast<int>(str.size()));
    }

    template <typename T>
    static Err getProperty(char * buffer, int * buffer_size, T const & value)
    {
        return getProperty(buffer, buffer_size, std::to_string(value));
    }
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
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_base); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    friend inline bool operator <(ModelLayer const & x, ModelLayer const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(ModelLayer const & x, ModelLayer const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(ModelLayer const & x, ModelLayer const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(ModelLayer const & x, ModelLayer const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(ModelLayer const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(ModelLayer const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

private:
    void _complete();
    void _incomplete();
    void _assign_id(int id);

public:
    char const * type() const;

public:
    int id() const;
    bool isComplete() const;

public:
    Err runner(Direction direction, Layers const & input, void * user);

public:
    Err get(std::string const & key, std::string & data) const;
    Err set(std::string const & key, std::string const & data);

public:
    std::string toString() const;

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

public:
    template <typename LayerType = LayerBase>
    inline std::shared_ptr<LayerType> base() const TBAG_NOEXCEPT
    {
        return std::static_pointer_cast<LayerType>(_base);
    }

public:
    /**
     * The class that sorts the IDs in the std::set container.
     *
     * @author zer0
     * @date   2019-07-11
     */
    struct IdLess
    {
        inline bool operator()(ModelLayer const & x, ModelLayer const & y) const
        {
            return x.id() < y.id();
        }
    };
};

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELLAYER_HPP__

