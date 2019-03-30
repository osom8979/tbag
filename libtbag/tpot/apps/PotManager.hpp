/**
 * @file   PotManager.hpp
 * @brief  PotManager class prototype.
 * @author zer0
 * @date   2019-01-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_POTMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_POTMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tpot/apps/BasePot.hpp>

#include <string>
#include <map>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

/**
 * PotManager class prototype.
 *
 * @author zer0
 * @date   2019-01-30
 */
class TBAG_API PotManager : private Noncopyable
{
public:
    using RunnerParams = BasePot::RunnerParams;
    using Pot  = std::shared_ptr<BasePot>;
    using Pots = std::map<std::string, Pot>;

private:
    Pots _pots;

public:
    PotManager();
    virtual ~PotManager();

public:
    Pots       & pots()       TBAG_NOEXCEPT { return _pots; }
    Pots const & pots() const TBAG_NOEXCEPT { return _pots; }

public:
    bool insertPot(Pot const & pot);
    bool insertPot(std::string const & name, Pot const & pot);

public:
    template <typename PotType>
    bool insertPot()
    {
        return insertPot(std::make_shared<PotType>());
    }

public:
    std::size_t registerDefaultPots();

public:
    std::size_t getMaximumLengthOfNames() const;
    std::string getRemarks() const;

public:
    int runOrLutjit(RunnerParams const & params);
    int run(RunnerParams const & params);
    int run(RunnerParams const & params, std::string const & default_mode);
};

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_APPS_POTMANAGER_HPP__

