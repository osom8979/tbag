/**
 * @file   TripleIoIndex.cpp
 * @brief  TripleIoIndex class implementation.
 * @author zer0
 * @date   2017-07-14
 */

#include <libtbag/lockfree/TripleIoIndex.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lockfree {

TripleIoIndex::TripleIoIndex(bool cas_loop)
        : COMPARE_AND_SWAP_OUTPUT_LOOP(cas_loop), _input(0x00), _output(0x01), _ready(0x02)
{
}

TripleIoIndex::~TripleIoIndex()
{
    // EMPTY.
}

int TripleIoIndex::updateNextInput()
{
    while (true) {
        Info ready = _ready.load();
        Info desired = getInfo(getIndex(_input), true);
        if (_ready.compare_exchange_weak(ready, desired)) {
            _input = ready;
            break;
        }
    }
    return getInputIndex();
}

int TripleIoIndex::updateNextOutput()
{
    while (COMPARE_AND_SWAP_OUTPUT_LOOP) {
        Info ready = _ready.load();
        if (isUpdate(ready) == false) {
            break;
        }
        Info desired = getInfo(getIndex(ready), false);
        if (_ready.compare_exchange_weak(ready, desired)) {
            _output.store(ready);
            break;
        }
    }
    return getOutputIndex();
}

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

