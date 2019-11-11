/**
 * @file   ExtraPref.hpp
 * @brief  ExtraPref class prototype.
 * @author zer0
 * @date   2019-11-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_EXTRAPREF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_EXTRAPREF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/pref/Preferences.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref    {
namespace details {

/**
 * ExtraPref class prototype.
 *
 * XML format:
 * @code{.xml}
 *  <extra>
 *    <!-- insert your custom xml -->
 *    <!-- ... -->
 *  </extra>
 * @endcode
 *
 * @author zer0
 * @date   2019-11-05
 */
class TBAG_API ExtraPref : public libtbag::pref::Preferences::NodeInterface
{
public:
    TBAG_CONSTEXPR static char const * const ROOT_NAME = "extra";

private:
    std::string _xml;

public:
    ExtraPref();
    virtual ~ExtraPref();

public:
    inline std::string       & xml()       TBAG_NOEXCEPT { return _xml; }
    inline std::string const & xml() const TBAG_NOEXCEPT { return _xml; }

public:
    std::string name() const override;
    bool init() override;
    void clear() override;
    void load(Element const & element) override;
    void save(Element & element) const override;
};

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_EXTRAPREF_HPP__

