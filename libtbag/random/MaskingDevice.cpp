/**
 * @file   MaskingDevice.cpp
 * @brief  MaskingDevice class implementation.
 * @author zer0
 * @date   2017-06-15
 */

#include <libtbag/random/MaskingDevice.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace random {

MaskingDevice::MaskingDevice() : _device(), _engine(_device()), _range(1)
{
    // EMPTY.
}

MaskingDevice::~MaskingDevice()
{
    // EMPTY.
}

uint32_t MaskingDevice::gen()
{
    return _range(_engine);
}

} // namespace random

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

