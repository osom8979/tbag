/**
 * @file   TimeId.cpp
 * @brief  TimeId class implementation.
 * @author zer0
 * @date   2016-07-26
 */

#include <libtbag/id/generator/TimeId.hpp>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace id        {
namespace generator {

static Id getTimeSinceEpochCount()
{
    return static_cast<Id>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

Id genTimeId(bool sleep_wait)
{
    Id id = getTimeSinceEpochCount();

    // Prevent the same ID.
    while (getTimeSinceEpochCount() == id) {
        if (sleep_wait) {
            std::this_thread::sleep_for(std::chrono::high_resolution_clock::duration(1));
        }
    }

    return id;
}

} // namespace generator
} // namespace id

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

