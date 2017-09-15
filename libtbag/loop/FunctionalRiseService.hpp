/**
 * @file   FunctionalRiseService.hpp
 * @brief  FunctionalRiseService class prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_FUNCTIONALRISESERVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_FUNCTIONALRISESERVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/loop/RiseService.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

/**
 * FunctionalRiseService class prototype.
 *
 * @author zer0
 * @date   2017-07-12
 */
class FunctionalRiseService : public RiseService
{
public:
    using SharedCom    = std::shared_ptr<RiseComponent>;
    using SharedComSet = std::set<SharedCom>;

    using Callback = std::function<void(SharedComSet&)>;
    using InterruptCallback = std::function<bool(SharedComSet&)>;

private:
    Callback _on_create_cb;
    Callback _on_timer_cb;
    Callback _on_destroy_cb;

    InterruptCallback _on_interrupt_cb;

public:
    FunctionalRiseService(uint64_t repeat = RISE_REPEAT_MILLISECONDS, bool verbose = false) : RiseService(repeat, verbose)
    { /* EMPTY. */ }
    virtual ~FunctionalRiseService()
    { /* EMPTY. */ }

public:
    void setOnCreate (Callback const & cb) {  _on_create_cb = cb; }
    void setOnTimer  (Callback const & cb) {   _on_timer_cb = cb; }
    void setOnDestroy(Callback const & cb) { _on_destroy_cb = cb; }

    void setOnInterrupt(InterruptCallback const & cb) { _on_interrupt_cb = cb; }

protected:
    virtual void onCreate(SharedComSet & coms) override
    {
        if (static_cast<bool>(_on_create_cb)) {
            _on_create_cb(coms);
        }
    }

    virtual void onTimer(SharedComSet & coms) override
    {
        if (static_cast<bool>(_on_timer_cb)) {
            _on_timer_cb(coms);
        }
    }

    virtual void onDestroy(SharedComSet & coms) override
    {
        if (static_cast<bool>(_on_destroy_cb)) {
            _on_destroy_cb(coms);
        }
    }

    virtual bool onInterrupt(SharedComSet & coms) override
    {
        if (static_cast<bool>(_on_interrupt_cb)) {
            return _on_interrupt_cb(coms);
        }
        return RiseService::TERMINATE_LOOP_FLAG;
    }
};

using FuncRiseService = FunctionalRiseService;

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_FUNCTIONALRISESERVICE_HPP__

