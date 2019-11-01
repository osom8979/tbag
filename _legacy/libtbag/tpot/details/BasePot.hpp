/**
 * @file   BasePot.hpp
 * @brief  BasePot class prototype.
 * @author zer0
 * @date   2019-01-30
 * @date   2019-04-09 (Move: libtbag/tpot/app -> libtbag/tpot/details)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_DETAILS_BASEPOT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_DETAILS_BASEPOT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/app/ex/DefaultApp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot    {
namespace details {

/**
 * BasePot class prototype.
 *
 * @author zer0
 * @date   2019-01-30
 * @date   2019-04-09 (Move: libtbag/tpot/app -> libtbag/tpot/details)
 */
class TBAG_API BasePot : private Noncopyable
{
public:
    using DefaultApp   = libtbag::app::ex::DefaultApp;
    using RunnerParams = DefaultApp::RunnerParams;
    using Environments = DefaultApp::Environments;
    using Arguments    = DefaultApp::Arguments;
    using ValueMap     = DefaultApp::ValueMap;
    using Storage      = DefaultApp::Storage;

private:
    RunnerParams _params;

public:
    BasePot();
    virtual ~BasePot();

public:
    void setParams(RunnerParams const & params) TBAG_NOEXCEPT
    { _params = params; }

public:
    RunnerParams       & params()       TBAG_NOEXCEPT { return _params; }
    RunnerParams const & params() const TBAG_NOEXCEPT { return _params; }

    Environments       & envs()       TBAG_NOEXCEPT { return _params.envs; }
    Environments const & envs() const TBAG_NOEXCEPT { return _params.envs; }

    Arguments       & args()       TBAG_NOEXCEPT { return _params.args; }
    Arguments const & args() const TBAG_NOEXCEPT { return _params.args; }

    ValueMap       & values()       TBAG_NOEXCEPT { return _params.values; }
    ValueMap const & values() const TBAG_NOEXCEPT { return _params.values; }

    Storage       & storage()       TBAG_NOEXCEPT { return _params.storage; }
    Storage const & storage() const TBAG_NOEXCEPT { return _params.storage; }

    bool verbose() const TBAG_NOEXCEPT { return _params.verbose; }

public:
    virtual char const * const getName() const TBAG_NOEXCEPT = 0;
    virtual char const * const getRemarks() const TBAG_NOEXCEPT = 0;
    virtual char const * const getHelp() const TBAG_NOEXCEPT = 0;

public:
    virtual int onMain() = 0;
};

} // namespace details
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_DETAILS_BASEPOT_HPP__

