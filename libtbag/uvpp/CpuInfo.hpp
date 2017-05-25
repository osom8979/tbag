/**
 * @file   CpuInfo.hpp
 * @brief  CpuInfo class prototype.
 * @author zer0
 * @date   2017-05-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_CPUINFO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_CPUINFO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <cstdint>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

struct CpuTimes
{
    uint64_t user;
    uint64_t nice;
    uint64_t sys;
    uint64_t idle;
    uint64_t irq;
};

struct CpuInfo
{
    std::string model;
    int speed;
    CpuTimes times;
};

TBAG_API std::vector<CpuInfo> getCpuInfos();

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_CPUINFO_HPP__

