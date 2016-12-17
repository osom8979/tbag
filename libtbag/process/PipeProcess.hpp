/**
 * @file   PipeProcess.hpp
 * @brief  PipeProcess class prototype.
 * @author zer0
 * @date   2016-05-17
 * @date   2016-11-27 (Rename: Process -> PipeProcess)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/process/Process.hpp>
#include <libtbag/stream/UvInStream.hpp>
#include <libtbag/stream/UvOutStream.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

/**
 * PipeProcess class prototype.
 *
 * @author zer0
 * @date   2016-05-17
 * @date   2016-12-17 (Refactoring)
 */
class TBAG_API PipeProcess : public Process
{
public:
    struct ProcPimpl;
    friend struct ProcPimpl;

public:
    PipeProcess();
    ~PipeProcess();
};

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROCESS_PIPEPROCESS_HPP__

