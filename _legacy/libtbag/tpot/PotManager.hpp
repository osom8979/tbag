/**
 * @file   PotManager.hpp
 * @brief  PotManager class prototype.
 * @author zer0
 * @date   2019-01-30
 * @date   2019-04-09 (Move: libtbag/tpot/app -> libtbag/tpot)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_POTMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_POTMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tpot/details/BasePot.hpp>
#include <libtbag/tpot/details/DynamicPot.hpp>

#include <string>
#include <map>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * PotManager class prototype.
 *
 * @author zer0
 * @date   2019-01-30
 * @date   2019-04-09 (Move: libtbag/tpot/app -> libtbag/tpot)
 */
class TBAG_API PotManager : private Noncopyable
{
public:
    using BasePot      = libtbag::tpot::details::BasePot;
    using DynamicPot   = libtbag::tpot::details::DynamicPot;
    using RunnerParams = BasePot::RunnerParams;

    using Pot  = std::shared_ptr<BasePot>;
    using Pots = std::map<std::string, Pot>;

    using PotMain0 = DynamicPot::PotMain0;
    using PotMain1 = DynamicPot::PotMain1;
    using PotMain2 = DynamicPot::PotMain2;
    using PotMain3 = DynamicPot::PotMain3;
    using PotMain4 = DynamicPot::PotMain4;
    using PotMain5 = DynamicPot::PotMain5;
    using PotMain6 = DynamicPot::PotMain6;
    using PotMain7 = DynamicPot::PotMain7;

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
    bool insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain0 const & pot);
    bool insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain1 const & pot);
    bool insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain2 const & pot);
    bool insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain3 const & pot);
    bool insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain4 const & pot);
    bool insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain5 const & pot);
    bool insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain6 const & pot);
    bool insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain7 const & pot);

public:
    std::size_t registerDefaultPots();

public:
    std::size_t getMaximumLengthOfNames() const;
    std::string getRemarks() const;

public:
    int help(std::string const & app_name, bool verbose);
    int runOrLutjit(RunnerParams const & params);
    int run(RunnerParams const & params);
    int run(RunnerParams const & params, std::string const & default_mode);
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_POTMANAGER_HPP__

