/**
 * @file   TripleIoIndex.cpp
 * @brief  TripleIoIndex class implementation.
 * @author zer0
 * @date   2017-07-14
 */

#include <libtbag/lockfree/TripleIoIndex.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lockfree {

TripleIoIndex::TripleIoIndex(int  input_value,
                             int output_value,
                             int  ready_value, bool cas_loop)
        : COMPARE_AND_SWAP_OUTPUT_LOOP(cas_loop),
          _info(createInfo(input_value, false, output_value, false, ready_value, false))
{
    if (_info.is_lock_free() == false) {
        tDLogW("TripleIoIndex::TripleIoIndex() Info is not a lock free.");
    }
}

TripleIoIndex::~TripleIoIndex()
{
    // EMPTY.
}

TripleIoIndex::Info TripleIoIndex::createInfo(int  input_value, bool  input_flag,
                                              int output_value, bool output_flag,
                                              int  ready_value, bool  ready_flag) TBAG_NOEXCEPT
{
    assert(0 <= COMPARE_AND( input_value) <= static_cast<int>(INDEX_BIT));
    assert(0 <= COMPARE_AND(output_value) <= static_cast<int>(INDEX_BIT));
    assert(0 <= COMPARE_AND( ready_value) <= static_cast<int>(INDEX_BIT));

    Info info;
    info.array[  INPUT_INDEX] = createByte( input_value,  input_flag);
    info.array[ OUTPUT_INDEX] = createByte(output_value, output_flag);
    info.array[  READY_INDEX] = createByte( ready_value,  ready_flag);
    info.array[PADDING_INDEX] = 0x00;
    return info;
}

TripleIoIndex::Info TripleIoIndex::nextInput()
{
    Info info;
    Info desired;

    while (true) {
        info = _info;

        desired = info;
        updateInput(desired, true);
        swapInputAndReady(desired);

        if (_info.compare_exchange_weak(info, desired)) {
            break;
        }
    }

    return desired;
}

TripleIoIndex::Info TripleIoIndex::nextOutput()
{
    Info info;
    Info desired;

    do {
        info = _info;
        if (isUpdate(info.array[READY_INDEX]) == false) {
            return info;
        }

        desired = info;
        updateOutput(desired, false);
        swapOutputAndReady(desired);

        if (_info.compare_exchange_weak(info, desired)) {
            break;
        }
    } while (COMPARE_AND_SWAP_OUTPUT_LOOP);

    return desired;
}

int TripleIoIndex::nextInputValue()
{
    return getValue(getInputByte(nextInput()));
}

int TripleIoIndex::nextOutputValue()
{
    return getValue(getOutputByte(nextOutput()));
}

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

