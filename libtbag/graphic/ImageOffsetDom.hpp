/**
 * @file   ImageOffsetDom.hpp
 * @brief  ImageOffsetDom class prototype.
 * @author zer0
 * @date   2018-08-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEOFFSETDOM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEOFFSETDOM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/3rd/tinyxml2/tinyxml2.h>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/macro/compiler.hpp>
#include <libtbag/iterator/IteratorBypass.hpp>

#include <string>

#if defined(TBAG_COMP_CLANG) && !defined(NDEBUG)
# include <map>
#else
# include <unordered_map>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

/**
 * ImageOffsetDom class prototype.
 *
 * @author zer0
 * @date   2018-08-04
 */
class TBAG_API ImageOffsetDom
{
public:
    using Document = tinyxml2::XMLDocument;
    using Element  = tinyxml2::XMLElement;
    using Node     = tinyxml2::XMLNode;
    using Printer  = tinyxml2::XMLPrinter;
    using Path     = libtbag::filesystem::Path;

public:
    TBAG_CONSTEXPR static char const * const   IMAGES_KEY = "images";
    TBAG_CONSTEXPR static char const * const    IMAGE_KEY = "image";
    TBAG_CONSTEXPR static char const * const     NAME_KEY = "name";
    TBAG_CONSTEXPR static char const * const     FILE_KEY = "file";
    TBAG_CONSTEXPR static char const * const        X_KEY = "x";
    TBAG_CONSTEXPR static char const * const        Y_KEY = "y";
    TBAG_CONSTEXPR static char const * const    WIDTH_KEY = "width";
    TBAG_CONSTEXPR static char const * const   HEIGHT_KEY = "height";
    TBAG_CONSTEXPR static char const * const ORIGIN_X_KEY = "origin_x";
    TBAG_CONSTEXPR static char const * const ORIGIN_Y_KEY = "origin_y";
    TBAG_CONSTEXPR static char const * const   WEIGHT_KEY = "weight";

public:
    struct Item
    {
        std::string file;
        int x        = 0;
        int y        = 0;
        int width    = 0;
        int height   = 0;
        int origin_x = 0;
        int origin_y = 0;
        int weight   = 0;
        std::string body;
    };

public:
#if defined(TBAG_COMP_CLANG) && !defined(NDEBUG)
    using Items = std::map<std::string, Item>;
#else
    using Items = std::unordered_map<std::string, Item>;
#endif

public:
    struct load_from_memory { /* EMPTY. */ };

private:
    Items _items;

public:
    TBAG_ITERATOR_MAP_BYPASS(Items, _items);

public:
    ImageOffsetDom();
    explicit ImageOffsetDom(std::string const & path);
    explicit ImageOffsetDom(std::string const & xml, load_from_memory const & UNUSED_PARAM(v));
    ImageOffsetDom(ImageOffsetDom const & obj);
    ImageOffsetDom(ImageOffsetDom && obj);
    ~ImageOffsetDom();

public:
    ImageOffsetDom & operator =(ImageOffsetDom const & obj);
    ImageOffsetDom & operator =(ImageOffsetDom && obj);

public:
    void copy(ImageOffsetDom const & obj);
    void swap(ImageOffsetDom & obj);

public:
    inline friend void swap(ImageOffsetDom & lh, ImageOffsetDom & rh) { lh.swap(rh); }

public:
    inline Items       & atItems()       TBAG_NOEXCEPT { return _items; }
    inline Items const & atItems() const TBAG_NOEXCEPT { return _items; }

public:
    bool find(std::string const & name, Item * result = nullptr) const;

public:
    std::string toXmlString() const;

public:
    bool saveToFile(std::string const & path) const;

public:
    bool loadFromMemory(std::string const & xml);
    bool loadFromFile(std::string const & path);

public:
    static ImageOffsetDom readFile(std::string const & path);
    static ImageOffsetDom readXml(std::string const & xml);
};

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEOFFSETDOM_HPP__

