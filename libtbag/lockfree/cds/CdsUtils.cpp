/**
 * @file   CdsUtils.cpp
 * @brief  CdsUtils class implementation.
 * @author zer0
 * @date   2018-11-08
 */

#include <libtbag/lockfree/cds/CdsUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cds/init.h>  // for cds::Initialize and cds::Terminate
#include <cds/gc/hp.h> // for cds::HP (Hazard Pointer) SMR

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lockfree {
namespace cds      {

void initialize()
{
    ::cds::Initialize();
}

void release()
{
    ::cds::Terminate();
    ::cds::gc::HP hpGC;
}

bool isThreadAttached()
{
    return ::cds::threading::Manager::isThreadAttached();
}

void attachThread()
{
    ::cds::threading::Manager::attachThread();
}

void detachThread()
{
    ::cds::threading::Manager::detachThread();
}

void initHazardPointerSingleton()
{
    // Hazard Pointer SMR (Safe Memory Reclamation)
    ::cds::gc::HP volatile __hazard_pointer__;
}

} // namespace cds
} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

