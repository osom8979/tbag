/**
 * @file   TpotRunner.hpp
 * @brief  TpotRunner class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTRUNNER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTRUNNER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotParams class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
struct TpotParams
{
    bool enable_tty;
};

/**
 * TpotRunner class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 *
 * @remarks
 *  Separate initializer and business logic.
 */
class TBAG_API TpotRunner final : public Noncopyable
{
private:
    TpotParams _params;

public:
    TpotRunner(TpotParams const & params);
    ~TpotRunner();

public:
    int run();
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTRUNNER_HPP__

