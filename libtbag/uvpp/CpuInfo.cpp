/**
 * @file   CpuInfo.cpp
 * @brief  CpuInfo class implementation.
 * @author zer0
 * @date   2017-05-25
 */

#include <libtbag/uvpp/CpuInfo.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

std::vector<CpuInfo> getCpuInfos()
{
    uv_cpu_info_t * cpu_infos = nullptr;
    int count = 0;

    // Gets information about the CPUs on the system.
    // The cpu_infos array will have count elements and needs to be freed with uv_free_cpu_info().
    int const CODE = ::uv_cpu_info(&cpu_infos, &count);
    if (convertUvErrorToErrWithLogging("getCpuInfos()", CODE) != Err::E_SUCCESS) {
        return std::vector<CpuInfo>();
    }

    assert(count > 0);
    std::vector<CpuInfo> result(count);

    uv_cpu_info_t * cursor = cpu_infos;
    for (int i = 0; i < count; ++i) {
        result[i].model.assign(cursor->model);
        result[i].speed = cursor->speed;
        result[i].times.user = cursor->cpu_times.user;
        result[i].times.nice = cursor->cpu_times.nice;
        result[i].times.sys  = cursor->cpu_times.sys ;
        result[i].times.idle = cursor->cpu_times.idle;
        result[i].times.irq  = cursor->cpu_times.irq ;
        ++cursor;
    }

    // Frees the cpu_infos array previously allocated with uv_cpu_info().
    ::uv_free_cpu_info(cpu_infos, count);

    return result;
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

