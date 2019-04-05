/**
 * @file   FlowChart.hpp
 * @brief  FlowChart class prototype.
 * @author zer0
 * @date   2017-12-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FLOW_FLOWCHART_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FLOW_FLOWCHART_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/flow/FlowLayer.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace flow {

/**
 * FlowChart class prototype.
 *
 * @author zer0
 * @date   2017-12-21
 */
template <typename DataType, typename KeyType = std::string>
class FlowChart : private Noncopyable
{
public:
    using Layer       = FlowLayer<DataType, KeyType>;
    using SharedLayer = std::shared_ptr<Layer>;

    using Data     = typename Layer::Data;
    using Key      = typename Layer::Key;
    using Keys     = typename Layer::Keys;
    using DataPtr  = typename Layer::DataPtr;
    using DataMap  = typename Layer::DataMap;
    using DataPair = typename Layer::DataPair;

public:
    struct LayerInfo
    {
        bool         completed;
        SharedLayer  layer;
        DataMap      prev_data;
        DataMap      next_data;

        LayerInfo(SharedLayer const & l) : completed(false), layer(l) { /* EMPTY. */ }
        ~LayerInfo() { /* EMPTY. */ }
    };

    using Layers     = std::unordered_map<Key, LayerInfo>;
    using LayersPair = typename Layers::value_type;

private:
    Layers _layers;

public:
    FlowChart() { /* EMPTY. */ }
    ~FlowChart() { /* EMPTY. */ }

public:
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_layers.size())  { return _layers.size();  }
    inline bool       empty() const TBAG_NOEXCEPT_SP_OP(_layers.empty()) { return _layers.empty(); }

public:
    bool insert(SharedLayer const & layer)
    {
        if (static_cast<bool>(layer) == false) {
            return false;
        }
        return _layers.insert(LayersPair(layer->key, LayerInfo(layer))).second;
    }

    /** Create(new) & insert layer. */
    template <typename LayerType, typename ... Args>
    inline bool createAndInsert(Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(Layer, LayerType);
        SharedLayer layer(new (std::nothrow) LayerType(std::forward<Args>(args) ...));
        if (static_cast<bool>(layer) == false) {
            return false;
        }
        return insert(layer);
    }

    bool erase(Key const & key)
    {
        return _layers.erase(key) == 1U;
    }

    inline SharedLayer get(Key const & key)
    {
        auto itr = _layers.find(key);
        if (itr == _layers.end()) {
            return SharedLayer();
        }
        return itr->second.layer;
    }

    template <typename LayerType>
    inline std::shared_ptr<LayerType> cast(Key const & key)
    {
        return std::static_pointer_cast<LayerType>(get(key));
    }

public:
    void resetCompleteFlags()
    {
        for (auto & layer : _layers) {
            layer.second.completed = false;
        }
    }

    void updateDataMap(DataMap & data, Keys const & keys)
    {
        for (auto & key : keys) {
            auto itr = _layers.find(key);
            if (itr != _layers.end() && itr->second.layer) {
                data.insert(DataPair(key, DataPtr(&itr->second.layer->data)));
            } else {
                data.insert(DataPair(key, DataPtr()));
            }
        }
    }

    void updateDataMap()
    {
        for (auto & layer : _layers) {
            layer.second.prev_data.clear();
            layer.second.next_data.clear();
            if (layer.second.layer) {
                updateDataMap(layer.second.prev_data, layer.second.layer->prev);
                updateDataMap(layer.second.next_data, layer.second.layer->next);
            }
        }
    }

public:
    template <typename Predicated>
    void forEach(Predicated predicated)
    {
        for (auto & layer : _layers) {
            if (layer.second.layer) {
                predicated(*(layer.second.layer));
            }
        }
    }

    void clear()
    {
        for (auto & layer : _layers) {
            if (layer.second.layer) {
                layer.second.layer->clear();
            }
        }
    }

    bool forward(bool reset = true, bool update = true)
    {
        // clang-format off
        if (update) {      updateDataMap(); }
        if ( reset) { resetCompleteFlags(); }
        // clang-format on

        for (auto & layer : _layers) {
            if (layer.second.layer && layer.second.layer->prev.empty()) {
                if (forward(layer.first) == false) {
                    return false;
                }
            }
        }
        return true;
    }

    bool backward(bool reset = true, bool update = true)
    {
        // clang-format off
        if (update) {      updateDataMap(); }
        if ( reset) { resetCompleteFlags(); }
        // clang-format on

        for (auto & layer : _layers) {
            if (layer.second.layer && layer.second.layer->next.empty()) {
                if (backward(layer.first) == false) {
                    return false;
                }
            }
        }
        return true;
    }

public:
    bool forward(Key const & key)
    {
        auto itr = _layers.find(key);
        if (itr == _layers.end()) {
            return false;
        }

        if (itr->second.completed) {
            return true;
        } else {
            itr->second.completed = true;
        }

        if (itr->second.layer) {
            for (auto & key : itr->second.layer->prev) {
                if (forward(key) == false) {
                    return false;
                }
            }
            if (itr->second.layer->forward(itr->second.prev_data) == false) {
                return false;
            }
            for (auto & key : itr->second.layer->next) {
                if (forward(key) == false) {
                    return false;
                }
            }
        }
        return true;
    }

    bool backward(Key const & key)
    {
        auto itr = _layers.find(key);
        if (itr == _layers.end()) {
            return false;
        }

        if (itr->second.completed) {
            return true;
        } else {
            itr->second.completed = true;
        }

        if (itr->second.layer) {
            for (auto & key : itr->second.layer->next) {
                if (backward(key) == false) {
                    return false;
                }
            }
            if (itr->second.layer->backward(itr->second.next_data) == false) {
                return false;
            }
            for (auto & key : itr->second.layer->prev) {
                if (backward(key) == false) {
                    return false;
                }
            }
        }
        return true;
    }
};

} // namespace flow

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FLOW_FLOWCHART_HPP__

