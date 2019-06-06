/**
 * @file   PotManager.cpp
 * @brief  PotManager class implementation.
 * @author zer0
 * @date   2019-01-30
 * @date   2019-04-09 (Move: libtbag/tpot/app -> libtbag/tpot)
 */

#include <libtbag/tpot/PotManager.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/string/Format.hpp>

#include <libtbag/tpot/apps/DemoPot.hpp>
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
    return insertPot(pot->getName(), pot);
}

bool PotManager::insertPot(std::string const & name, Pot const & pot)
{
    assert(static_cast<bool>(pot));
    return _pots.insert(std::make_pair(std::string(name), pot)).second;
}

bool PotManager::insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain0 const & pot)
{
    return insertPot(n, std::make_shared<DynamicPot>(n, r, h, pot));
}

bool PotManager::insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain1 const & pot)
{
    return insertPot(n, std::make_shared<DynamicPot>(n, r, h, pot));
}

bool PotManager::insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain2 const & pot)
{
    return insertPot(n, std::make_shared<DynamicPot>(n, r, h, pot));
}

bool PotManager::insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain3 const & pot)
{
    return insertPot(n, std::make_shared<DynamicPot>(n, r, h, pot));
}

bool PotManager::insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain4 const & pot)
{
    return insertPot(n, std::make_shared<DynamicPot>(n, r, h, pot));
}

bool PotManager::insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain5 const & pot)
{
    return insertPot(n, std::make_shared<DynamicPot>(n, r, h, pot));
}

bool PotManager::insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain6 const & pot)
{
    return insertPot(n, std::make_shared<DynamicPot>(n, r, h, pot));
}

bool PotManager::insertPot(std::string const & n, std::string const & r, std::string const & h, PotMain7 const & pot)
{
    return insertPot(n, std::make_shared<DynamicPot>(n, r, h, pot));
}

std::size_t PotManager::registerDefaultPots()
{
    std::size_t count = 0;
    using namespace libtbag::tpot::apps;
#ifndef __INSERT_NEW_POT
#define __INSERT_NEW_POT(t) if (insertPot(t::name(), std::make_shared<t>())) { count++; }
#endif
    __INSERT_NEW_POT(DemoPot)
    __INSERT_NEW_POT(LuaPot)
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

int PotManager::help(std::string const & app_name, bool verbose)
{
    if (app_name.empty()) {
        if (verbose) {
            std::cerr << "Empty application name." << std::endl;
        }
        return EXIT_FAILURE;
    }

    auto itr = _pots.find(app_name);
    if (itr == _pots.end()) {
        if (verbose) {
            std::cerr << "Not found application: " << app_name << std::endl;
        }
        return EXIT_FAILURE;
    }

    auto const message = itr->second->getHelp();
    if (message == nullptr) {
        if (verbose) {
            std::cerr << "Empty help message." << std::endl;
        }
    } else {
        std::cout << message << std::endl;
    }
    return EXIT_SUCCESS;
}

int PotManager::runOrLutjit(RunnerParams const & params)
{
    return run(params, libtbag::tpot::apps::LuaPot::name());
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

int PotManager::run(RunnerParams const & params, std::string const & default_mode)
{
    assert(!default_mode.empty());

    Pots::iterator itr;
    if (!params.args.empty() && _pots.find(params.args[0]) != _pots.end()) {
        itr = _pots.find(params.args[0]);
    } else {
        itr = _pots.find(default_mode);
        if (itr == _pots.end()) {
            std::cerr << "Unknown default application name: " << default_mode << std::endl;
            return EXIT_FAILURE;
        }
    }

    assert(static_cast<bool>(itr->second));
    itr->second->setParams(params);
    return itr->second->onMain();
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

