/**
 * @file   PcCommon.hpp
 * @brief  Parallel Computing common prototype.
 * @author zer0
 * @date   2019-02-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PCCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PCCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/id/Id.hpp>
#include <libtbag/Type.hpp>

#include <exception>
#include <string>
#include <vector>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {

using PcId = libtbag::id::Id;

TBAG_CONSTEXPR PcId const UNKNOWN_ID = libtbag::id::UNKNOWN_ID;

using PcTypeId     = PcId;
using PcPlatformId = PcId;
using PcDeviceId   = PcId;
using PcContextId  = PcId;

TBAG_CONSTEXPR PcTypeId const PC_TYPE_NONE = 0;
TBAG_CONSTEXPR PcTypeId const PC_TYPE_CPU  = 1;
TBAG_CONSTEXPR PcTypeId const PC_TYPE_CUDA = 2;
TBAG_CONSTEXPR PcTypeId const PC_TYPE_CL   = 3;

struct PcType
{
    PcTypeId type;
};

struct PcPlatform : public PcType
{
    PcPlatformId platform;
};

struct PcDevice : public PcPlatform
{
    PcDeviceId device;
};

struct PcContext : public PcDevice
{
    PcContextId context;
};

using PcPlatformIds = std::vector<PcPlatformId>;
using PcDeviceIds   = std::vector<PcDeviceId>;
using PcContextIds  = std::vector<PcContextId>;

// @formatter:off
static_assert(sizeof(PcType)     == sizeof(PcTypeId)   /* ---------------- */, "size checker.");
static_assert(sizeof(PcPlatform) == sizeof(PcType)     + sizeof(PcPlatformId), "size checker.");
static_assert(sizeof(PcDevice)   == sizeof(PcPlatform) + sizeof(PcDeviceId)  , "size checker.");
static_assert(sizeof(PcContext)  == sizeof(PcDevice)   + sizeof(PcContextId) , "size checker.");
// @formatter:on

struct PcPlatformInfo
{
    std::string  profile;
    std::string  name;
    std::string  vendor;
    std::string  version;
    std::string  extensions;

    PcPlatformInfo() { /* EMPTY. */ }
    ~PcPlatformInfo() { /* EMPTY. */ }
};

struct PcDeviceInfo
{
    using Properties = std::map<std::string, std::string>;

    std::string  name;           ///< Device name.
    std::string  device_version; ///< Version string.
    std::string  driver_version; ///< Driver version string.
    Properties   properties;     ///< Extension properties.
    std::size_t  global_memory;  ///< Size of global device memory in bytes.

    PcDeviceInfo() : global_memory(0) { /* EMPTY. */ }
    ~PcDeviceInfo() { /* EMPTY. */ }

    template <typename ValueType>
    bool insert(std::string const & name, ValueType const & prop)
    { return insert(name, std::to_string(prop)); }

    bool insert(std::string const & name, char const * prop)
    { return insert(name, std::string(prop)); }

    bool insert(std::string const & name, std::string const & prop)
    { return properties.insert(Properties::value_type(name, prop)).second; }

    bool exists(std::string const & name) const
    { return properties.find(name) != properties.end(); }

    std::string get(std::string const & name) const
    {
        auto itr = properties.find(name);
        if (itr != properties.end()) {
            return itr->second;
        }
        return std::string();
    }
};

} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PCCOMMON_HPP__

