/**
 * @file   BuilderPot.hpp
 * @brief  BuilderPot class prototype.
 * @author zer0
 * @date   2019-06-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_BUILDERPOT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_BUILDERPOT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tpot/details/BasePot.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

/**
 * BuilderPot class prototype.
 *
 * @author zer0
 * @date   2019-06-07
 */
class TBAG_API BuilderPot : public libtbag::tpot::details::BasePot
{
public:
    BuilderPot();
    virtual ~BuilderPot();

public:
    static char const * const name() TBAG_NOEXCEPT;
    static char const * const remarks() TBAG_NOEXCEPT;
    static char const * const help() TBAG_NOEXCEPT;

public:
    virtual char const * const getName() const TBAG_NOEXCEPT override;
    virtual char const * const getRemarks() const TBAG_NOEXCEPT override;
    virtual char const * const getHelp() const TBAG_NOEXCEPT override;

public:
    virtual int onMain() override;
};

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_BUILDERPOT_HPP__

