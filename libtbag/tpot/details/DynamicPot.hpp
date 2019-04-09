/**
 * @file   DynamicPot.hpp
 * @brief  DynamicPot class prototype.
 * @author zer0
 * @date   2019-04-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_DETAILS_DYNAMICPOT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_DETAILS_DYNAMICPOT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tpot/details/BasePot.hpp>

#include <string>
#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot    {
namespace details {

/**
 * DynamicPot class prototype.
 *
 * @author zer0
 * @date   2019-04-09
 */
class TBAG_API DynamicPot : public BasePot
{
public:
    using PotMain0 = std::function<void(void)>;
    using PotMain1 = std::function<int(void)>;
    using PotMain2 = std::function<int(RunnerParams&)>;
    using PotMain3 = std::function<int(RunnerParams&, Environments&)>;
    using PotMain4 = std::function<int(RunnerParams&, Environments&, Arguments&)>;
    using PotMain5 = std::function<int(RunnerParams&, Environments&, Arguments&, ValueMap&)>;
    using PotMain6 = std::function<int(RunnerParams&, Environments&, Arguments&, ValueMap&, Storage&)>;
    using PotMain7 = std::function<int(RunnerParams&, Environments&, Arguments&, ValueMap&, Storage&, bool)>;

public:
    using FakeFunction = void;
    using SharedFunc = std::shared_ptr<FakeFunction>;

private:
    std::string const NAME;
    std::string const REMARKS;
    std::string const HELP;
    int const MAIN_TYPE;

private:
    SharedFunc _func;

public:
    DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain0 const & pot);
    DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain1 const & pot);
    DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain2 const & pot);
    DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain3 const & pot);
    DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain4 const & pot);
    DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain5 const & pot);
    DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain6 const & pot);
    DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain7 const & pot);
    virtual ~DynamicPot();

private:
    void checkParams();

public:
    virtual char const * const getName() const TBAG_NOEXCEPT override;
    virtual char const * const getRemarks() const TBAG_NOEXCEPT override;
    virtual char const * const getHelp() const TBAG_NOEXCEPT override;

public:
    virtual int onMain() override;
};

} // namespace details
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_DETAILS_DYNAMICPOT_HPP__

