/**
 * @file   TmxTag.hpp
 * @brief  TmxTag class prototype.
 * @author zer0
 * @date   2019-08-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTAG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTAG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxTag class prototype.
 *
 * @author zer0
 * @date   2019-08-17
 */
struct TBAG_API TmxTag : protected libtbag::dom::xml::XmlHelper
{
    char const * TAG;

    TmxTag(char const * tag);
    virtual ~TmxTag();

    virtual Err onRead(Element const & elem);
    virtual Err onWrite(Element & elem) const;

    bool validateTagName(Element const & elem) const;

    Err read(Element const & elem);
    Err read(std::string const & xml);

    Err write(Element & elem) const;
    Err write(std::string & xml) const;

    template <typename T>
    static void read(Element const & elem, char const * tag, std::vector<T> & vec)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(TmxTag, T);
        std::vector<T> result;
        foreachElement(elem, tag, [&](Element const & e){
            T v;
            if (isSuccess(v.read(e))) {
                result.emplace_back(std::move(v));
            }
        });
        vec.swap(result);
    }

    template <typename T>
    static void read(Element const & elem, std::vector<T> & vec)
    {
        read(elem, T::TAG_NAME, vec);
    }

    template <typename Itr>
    static std::size_t write(Element & elem, char const * tag, Itr begin, Itr end)
    {
        std::size_t result = 0;
        for (; begin != end; ++begin) {
            newElement(elem, tag, [&](Element & p){
                if (isSuccess(begin->write(p))) {
                    ++result;
                }
            });
        }
        return result;
    }

    template <typename T>
    static std::size_t write(Element & elem, char const * tag, std::vector<T> const & vec)
    {
        return write(elem, tag, vec.begin(), vec.end());
    }

    template <typename T>
    static std::size_t write(Element & elem, std::vector<T> const & vec)
    {
        return write(elem, T::TAG_NAME, vec);
    }
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTAG_HPP__

