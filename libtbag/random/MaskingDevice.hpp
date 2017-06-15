/**
 * @file   MaskingDevice.hpp
 * @brief  MaskingDevice class prototype.
 * @author zer0
 * @date   2017-06-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_MASKINGDEVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_MASKINGDEVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <random>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace random {

/**
 * MaskingDevice class prototype.
 *
 * @author zer0
 * @date   2017-06-15
 */
class TBAG_API MaskingDevice : private Noncopyable
{
public:
    using Device = std::random_device;
    using Engine = std::mt19937;
    using Range  = std::uniform_int_distribution<uint32_t>;

private:
    Device _device;
    Engine _engine;
    Range  _range;

public:
    MaskingDevice();
    virtual ~MaskingDevice();

public:
    uint32_t gen();
};

} // namespace random

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_MASKINGDEVICE_HPP__

