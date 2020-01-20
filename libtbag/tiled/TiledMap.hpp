/**
 * @file   TiledMap.hpp
 * @brief  TiledMap class prototype.
 * @author zer0
 * @date   2020-01-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_TILEDMAP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_TILEDMAP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/tiled/details/TmxMap.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled {

/**
 * TiledMap class prototype.
 *
 * @author zer0
 * @date   2020-01-07
 */
class TBAG_API TiledMap : private Noncopyable
{
public:
    using TmxMap = libtbag::tiled::details::TmxMap;
    using TmxTileSet = libtbag::tiled::details::TmxTileSet;

public:
    struct Callbacks
    {
        Callbacks() { /* EMPTY. */ }
        virtual ~Callbacks() { /* EMPTY. */ }

        virtual void onInitMap(TmxMap const & map) { /* EMPTY. */ }
    };

private:
    Callbacks * _cb;

private:
    TmxMap _map;

public:
    TiledMap(Callbacks * cb = nullptr);
    virtual ~TiledMap();

public:
    inline Callbacks * getCallbacks() const TBAG_NOEXCEPT { return _cb; }
    inline void setCallbacks(Callbacks * cb) TBAG_NOEXCEPT { _cb = cb; }

public:
    inline TmxMap       & map()       TBAG_NOEXCEPT { return _map; }
    inline TmxMap const & map() const TBAG_NOEXCEPT { return _map; }

public:
    Err readFromFile(std::string const & path, bool auto_init = true);
    Err readFromXmlText(std::string const & xml, bool auto_init = true);

public:
    Err writeToFile(std::string const & path) const;
    Err writeToXmlText(std::string & xml) const;

public:
    void init();
};

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_TILEDMAP_HPP__

