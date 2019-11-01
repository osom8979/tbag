/**
 * @file   DynamicPot.cpp
 * @brief  DynamicPot class implementation.
 * @author zer0
 * @date   2019-04-09
 */

#include <libtbag/tpot/details/DynamicPot.hpp>
#include <libtbag/debug/Assert.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot    {
namespace details {

DynamicPot::DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain0 const & pot)
        : NAME(n), REMARKS(r), HELP(h), MAIN_TYPE(0)
{
    _func = SharedFunc((FakeFunction*)new PotMain0(pot), [](FakeFunction * f){
        delete (PotMain0*)f;
    });
    checkParams();
}

DynamicPot::DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain1 const & pot)
        : NAME(n), REMARKS(r), HELP(h), MAIN_TYPE(1)
{
    _func = SharedFunc((FakeFunction*)new PotMain1(pot), [](FakeFunction * f){
        delete (PotMain1*)f;
    });
    checkParams();
}

DynamicPot::DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain2 const & pot)
        : NAME(n), REMARKS(r), HELP(h), MAIN_TYPE(2)
{
    _func = SharedFunc((FakeFunction*)new PotMain2(pot), [](FakeFunction * f){
        delete (PotMain2*)f;
    });
    checkParams();
}

DynamicPot::DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain3 const & pot)
        : NAME(n), REMARKS(r), HELP(h), MAIN_TYPE(3)
{
    _func = SharedFunc((FakeFunction*)new PotMain3(pot), [](FakeFunction * f){
        delete (PotMain3*)f;
    });
    checkParams();
}

DynamicPot::DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain4 const & pot)
        : NAME(n), REMARKS(r), HELP(h), MAIN_TYPE(4)
{
    _func = SharedFunc((FakeFunction*)new PotMain4(pot), [](FakeFunction * f){
        delete (PotMain4*)f;
    });
    checkParams();
}

DynamicPot::DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain5 const & pot)
        : NAME(n), REMARKS(r), HELP(h), MAIN_TYPE(5)
{
    _func = SharedFunc((FakeFunction*)new PotMain5(pot), [](FakeFunction * f){
        delete (PotMain5*)f;
    });
    checkParams();
}

DynamicPot::DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain6 const & pot)
        : NAME(n), REMARKS(r), HELP(h), MAIN_TYPE(6)
{
    _func = SharedFunc((FakeFunction*)new PotMain6(pot), [](FakeFunction * f){
        delete (PotMain6*)f;
    });
    checkParams();
}

DynamicPot::DynamicPot(std::string const & n, std::string const & r, std::string const & h, PotMain7 const & pot)
        : NAME(n), REMARKS(r), HELP(h), MAIN_TYPE(7)
{
    _func = SharedFunc((FakeFunction*)new PotMain7(pot), [](FakeFunction * f){
        delete (PotMain7*)f;
    });
    checkParams();
}

DynamicPot::~DynamicPot()
{
    // EMPTY.
}

void DynamicPot::checkParams()
{
    assert(!NAME.empty());
    assert(0 <= COMPARE_AND(MAIN_TYPE) <= 7);
    assert(static_cast<bool>(_func));
}

char const * const DynamicPot::getName() const TBAG_NOEXCEPT
{
    return NAME.c_str();
}

char const * const DynamicPot::getRemarks() const TBAG_NOEXCEPT
{
    return REMARKS.c_str();
}

char const * const DynamicPot::getHelp() const TBAG_NOEXCEPT
{
    return HELP.c_str();
}

int DynamicPot::onMain()
{
    auto * fake_func = _func.get();
    assert(fake_func != nullptr);

    // clang-format off
    switch (MAIN_TYPE) {
    case 0:
        (*((PotMain0*)fake_func))();
        return EXIT_SUCCESS;
    case 1: return (*((PotMain1*)fake_func))();
    case 2: return (*((PotMain2*)fake_func))(params());
    case 3: return (*((PotMain3*)fake_func))(envs());
    case 4: return (*((PotMain4*)fake_func))(args());
    case 5: return (*((PotMain5*)fake_func))(values());
    case 6: return (*((PotMain6*)fake_func))(storage());
    case 7: return (*((PotMain7*)fake_func))(verbose());
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    // clang-format on
    return EXIT_FAILURE;
}

} // namespace details
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

