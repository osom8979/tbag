/**
 * @file   RiseService.cpp
 * @brief  RiseService class implementation.
 * @author zer0
 * @date   2017-07-12
 */

#include <libtbag/loop/RiseService.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalTimer.hpp>
#include <libtbag/uvpp/func/FunctionalSignal.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

// -----------------------------
// RiseComponent implementation.
// -----------------------------

RiseComponent::RiseComponent() : _state(ComState::CS_READY), _rise_try_count(0), _try_count(0)
{
    // EMPTY.
}

RiseComponent::~RiseComponent()
{
    if (_state != ComState::CS_READY) {
        joinIfJoinable();
    }
}

bool RiseComponent::isReadyNoSafe() const TBAG_NOEXCEPT
{
    return _state == ComState::CS_READY;
}

bool RiseComponent::isEndNoSafe() const TBAG_NOEXCEPT
{
    return _state == ComState::CS_EXCEPTION || _state == ComState::CS_END_SUCCESS || _state == ComState::CS_END_FAILURE;
}

bool RiseComponent::isRunningNoSafe() const TBAG_NOEXCEPT
{
    return _state == ComState::CS_RUNNING;
}

bool RiseComponent::isThreadEndFailureNoSafe() const TBAG_NOEXCEPT
{
    return _state == ComState::CS_THREAD_END_FAILURE;
}

bool RiseComponent::isRiseIfFailureNoSafe() const TBAG_NOEXCEPT
{
    return _rise_try_count >= 1;
}

bool RiseComponent::isNeverDieNoSafe() const TBAG_NOEXCEPT
{
    return _rise_try_count == NEVER_DIE_TRY_COUNT;
}

bool RiseComponent::isEndOfTryNoSafe() const TBAG_NOEXCEPT
{
    return isNeverDieNoSafe() == false && isRiseIfFailureNoSafe() && _try_count >= _rise_try_count;
}

bool RiseComponent::runThreadNoSafe()
{
    try {
        Guard const THREAD_MUTEX_GUARD(_thread_mutex);
        _thread = std::thread([&](){
            bool const EXIT_CODE = this->run();

            Guard const ASYNC_MUTEX_GUARD(_mutex);
            if (EXIT_CODE) {
                _state = ComState::CS_END_SUCCESS;
            } else if (isEndOfTryNoSafe() == false) {
                _state = ComState::CS_THREAD_END_FAILURE;
            } else {
                _state = ComState::CS_END_FAILURE;
            }
        });
    } catch (...) {
        return false;
    }
    return true;
}

void RiseComponent::setRiseIfFailure(int try_count)
{
    Guard const MUTEX_GUARD(_mutex);
    _rise_try_count = try_count;
}

void RiseComponent::setNeverDie()
{
    Guard const MUTEX_GUARD(_mutex);
    _rise_try_count = NEVER_DIE_TRY_COUNT;
}

void RiseComponent::clearTryCount()
{
    Guard const MUTEX_GUARD(_mutex);
    _try_count = 0;
}

RiseComponent::ComState RiseComponent::getState() const TBAG_NOEXCEPT
{
    Guard const MUTEX_GUARD(_mutex);
    return _state;
}

int RiseComponent::getTryCount() const TBAG_NOEXCEPT
{
    Guard const MUTEX_GUARD(_mutex);
    return _try_count;
}

bool RiseComponent::isEnd() const TBAG_NOEXCEPT
{
    Guard const MUTEX_GUARD(_mutex);
    return isEndNoSafe();
}

void RiseComponent::joinIfJoinable()
{
    Guard const THREAD_MUTEX_GUARD(_thread_mutex);
    if (_thread.joinable()) {
        _thread.join();
    }
}

void RiseComponent::autoRun()
{
    Guard const MUTEX_GUARD(_mutex);
    if (isRunningNoSafe() || isEndNoSafe()) {
        return;
    }

    if (isReadyNoSafe()) {
        if (runThreadNoSafe()) {
            _state = ComState::CS_RUNNING;
        } else {
            _state = ComState::CS_EXCEPTION;
        }
        return;
    }

    assert(isThreadEndFailureNoSafe());

    if (isEndOfTryNoSafe()) {
        _state = ComState::CS_END_FAILURE;
        return;
    }

    joinIfJoinable();

    _try_count++;
    if (runThreadNoSafe()) {
        _state = ComState::CS_RUNNING;
    } else {
        _state = ComState::CS_EXCEPTION;
    }
}

// ---------------------------
// RiseService implementation.
// ---------------------------

RiseService::RiseService(uint64_t repeat, bool verbose) : REPEAT(repeat < 1 ? 1 : repeat), VERBOSE(verbose)
{
    // EMPTY.
}

RiseService::~RiseService()
{
    // EMPTY.
}

Err RiseService::run()
{
    return run(SharedComSet());
}

Err RiseService::run(SharedComSet const & init_coms)
{
    tDLogD("RiseService::run() == BEGIN ==");

    using Loop       = libtbag::uvpp::Loop;
    using FuncTimer  = libtbag::uvpp::func::FuncTimer;
    using FuncSignal = libtbag::uvpp::func::FuncSignal;

    Loop loop;
    auto timer = loop.newHandle<FuncTimer>(loop);
    if (static_cast<bool>(timer) == false) {
        tDLogA("RiseService::run() Timer handle is expired.");
        return Err::E_BADALLOC;
    }

    auto signal = loop.newHandle<FuncSignal>(loop);
    if (static_cast<bool>(signal) == false) {
        tDLogA("RiseService::run() Signal handle is expired.");
        return Err::E_BADALLOC;
    }

    Err const TIMER_START_CODE = timer->start(0, REPEAT);
    Err const SIGNAL_START_CODE = signal->start(signal::TBAG_SIGNAL_INTERRUPT);

    if (SIGNAL_START_CODE != Err::E_SUCCESS || TIMER_START_CODE != Err::E_SUCCESS) {
        tDLogE("RiseService::run() Handle start (T:{}, S:{}) error", getErrName(TIMER_START_CODE), getErrName(SIGNAL_START_CODE));
        timer->close();
        signal->close();
        loop.run();
        return SIGNAL_START_CODE;
    }

    SharedComSet coms = init_coms;
    onCreate(coms);

    std::size_t const COMS_SIZE = coms.size();
    timer->setOnTimer([&](){
        std::size_t end_count = 0;
        for (auto & com : coms) {
            assert(static_cast<bool>(com));
            if (com->isEnd()) {
                ++end_count;
            } else {
                com->autoRun();
            }
        }

        assert(static_cast<bool>(timer));
        assert(end_count <= COMS_SIZE);

        if (VERBOSE) {
            tDLogD("RiseService::run() Component end checker: ({}/{})", end_count, COMS_SIZE);
        }

        onTimer(coms);

        if (end_count == COMS_SIZE) {
            timer->close();
            signal->close();
        }
    });
    signal->setOnSignal([&](int signum) {
        if (onInterrupt(coms) == TERMINATE_LOOP_FLAG) {
            timer->close();
            signal->close();
        }
    });

    tDLogD("RiseService::run() Timer repeat: {}ms", REPEAT);
    Err const LOOP_EXIT_CODE = loop.run();
    if (LOOP_EXIT_CODE != Err::E_SUCCESS) {
        tDLogE("RiseService::run() Loop end {} error", libtbag::getErrName(LOOP_EXIT_CODE));
    }

    tDLogD("RiseService::run() Waiting component threads ...");

    std::size_t com_index = 1;
    for (auto & com : coms) {
        assert(static_cast<bool>(com));
        tDLogD("RiseService::run() Waiting component ({}/{})", com_index, COMS_SIZE);
        com->joinIfJoinable();
        ++com_index;
    }

    onDestroy(coms);

    tDLogD("RiseService::run() == END ==");
    return Err::E_SUCCESS;
}

void RiseService::onCreate(SharedComSet & coms)
{
    // EMPTY.
}

void RiseService::onTimer(SharedComSet & coms)
{
    // EMPTY.
}

void RiseService::onDestroy(SharedComSet & coms)
{
    // EMPTY.
}

bool RiseService::onInterrupt(SharedComSet & coms)
{
    return TERMINATE_LOOP_FLAG;
}

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

