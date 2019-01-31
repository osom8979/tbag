/**
 * @file   PotManager.cpp
 * @brief  PotManager class implementation.
 * @author zer0
 * @date   2019-01-30
 */

#include <libtbag/tpot/apps/PotManager.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/string/Format.hpp>

#include <libtbag/tpot/apps/GamePot.hpp>
#include <libtbag/tpot/apps/GmPot.hpp>
#include <libtbag/tpot/apps/LuaPot.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

PotManager::PotManager()
{
    // EMPTY.
}

PotManager::~PotManager()
{
    // EMPTY.
}

bool PotManager::insertPot(Pot const & pot)
{
    assert(static_cast<bool>(pot));
    return _pots.insert(std::make_pair(std::string(pot->getName()), pot)).second;
}

std::size_t PotManager::registerDefaultPots()
{
    std::size_t count = 0;
#ifndef __INSERT_NEW_POT
#define __INSERT_NEW_POT(t) if (insertPot<t>()) { count++; }
#endif
    __INSERT_NEW_POT(GamePot);
    __INSERT_NEW_POT(GmPot);
    __INSERT_NEW_POT(LuaPot);
#undef __INSERT_NEW_POT
    return count;
}

std::size_t PotManager::getMaximumLengthOfNames() const
{
    std::size_t size = 0;
    for (auto & pot : _pots) {
        size = libtbag::algorithm::getMax(size, strlen(pot.second->getName()));
    }
    return size;
}

std::string PotManager::getRemarks() const
{
    using namespace libtbag::string;

    auto const MORE_MARGIN_SIZE = 2;
    auto const PADDING_SIZE = getMaximumLengthOfNames() + MORE_MARGIN_SIZE;
    auto const FORMAT_STRING = std::string(" - {:<") + std::to_string(PADDING_SIZE) + "}{}";

    std::stringstream ss;
    ss << "List of application:" << std::endl;
    for (auto & pot : _pots) {
        ss << fformat(FORMAT_STRING, pot.second->getName(), pot.second->getRemarks()) << std::endl;
    }
    return ss.str();
}

int PotManager::run(RunnerParams const & params)
{
    if (params.args.empty()) {
        std::cerr << "Empty argument." << std::endl;
        return EXIT_FAILURE;
    }

    auto itr = _pots.find(params.args[0]);
    if (itr == _pots.end()) {
        std::cerr << "Unknown application name: " << params.args[0] << std::endl;
        return EXIT_FAILURE;
    }

    assert(static_cast<bool>(itr->second));
    itr->second->setParams(params);
    return itr->second->onMain();
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

