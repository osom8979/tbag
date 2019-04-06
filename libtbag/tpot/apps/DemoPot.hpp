/**
 * @file   DemoPot.hpp
 * @brief  DemoPot class prototype.
 * @author zer0
 * @date   2019-04-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_DEMOPOT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_DEMOPOT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tpot/apps/BasePot.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

/**
 * DemoPot class prototype.
 *
 * @author zer0
 * @date   2019-04-06
 */
class TBAG_API DemoPot : public BasePot
{
public:
    DemoPot();
    virtual ~DemoPot();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_DEMOPOT_HPP__

