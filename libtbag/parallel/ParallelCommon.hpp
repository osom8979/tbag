/**
 * @file   ParallelCommon.hpp
 * @brief  Parallel Computing common prototype.
 * @author zer0
 * @date   2019-02-21
 * @date   2019-03-23 (Rename: PcCommon -> ParallelCommon)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PARALLELCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PARALLELCOMMON_HPP__

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

using ParallelId = libtbag::id::Id;

TBAG_CONSTEXPR ParallelId const UNKNOWN_ID = libtbag::id::UNKNOWN_ID;

using ParallelTypeId     = ParallelId;
using ParallelPlatformId = ParallelId;
using ParallelDeviceId   = ParallelId;
using ParallelContextId  = ParallelId;

TBAG_CONSTEXPR ParallelTypeId const PC_TYPE_NONE = 0;
TBAG_CONSTEXPR ParallelTypeId const PC_TYPE_CPU  = 1;
TBAG_CONSTEXPR ParallelTypeId const PC_TYPE_CUDA = 2;
TBAG_CONSTEXPR ParallelTypeId const PC_TYPE_CL   = 3;

struct ParallelType
{
    ParallelTypeId type;
};

struct ParallelPlatform : public ParallelType
{
    ParallelPlatformId platform;
};

struct ParallelDevice : public ParallelPlatform
{
    ParallelDeviceId device;
};

struct ParallelContext : public ParallelDevice
{
    ParallelContextId context;
};

using ParallelPlatformIds = std::vector<ParallelPlatformId>;
using ParallelDeviceIds   = std::vector<ParallelDeviceId>;
using ParallelContextIds  = std::vector<ParallelContextId>;

// @formatter:off
static_assert(sizeof(ParallelType)     == sizeof(ParallelTypeId)   /* ---------------------- */, "size checker.");
static_assert(sizeof(ParallelPlatform) == sizeof(ParallelType)     + sizeof(ParallelPlatformId), "size checker.");
static_assert(sizeof(ParallelDevice)   == sizeof(ParallelPlatform) + sizeof(ParallelDeviceId)  , "size checker.");
static_assert(sizeof(ParallelContext)  == sizeof(ParallelDevice)   + sizeof(ParallelContextId) , "size checker.");
// @formatter:on

struct ParallelPlatformInfo
{
    std::string  profile;
    std::string  name;
    std::string  vendor;
    std::string  version;
    std::string  extensions;

    ParallelPlatformInfo() { /* EMPTY. */ }
    ~ParallelPlatformInfo() { /* EMPTY. */ }
};

struct ParallelDeviceInfo
{
    using Properties = std::map<std::string, std::string>;

    std::string  name;           ///< Device name.
    std::string  device_version; ///< Version string.
    std::string  driver_version; ///< Driver version string.
    Properties   properties;     ///< Extension properties.
    std::size_t  global_memory;  ///< Size of global device memory in bytes.

    ParallelDeviceInfo() : global_memory(0) { /* EMPTY. */ }
    ~ParallelDeviceInfo() { /* EMPTY. */ }

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PARALLELCOMMON_HPP__

