/**
 * @file   ProcessState.cpp
 * @brief  ProcessState class implementation.
 * @author zer0
 * @date   2019-10-20
 */

#include <libtbag/system/linux/ProcessState.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/String.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <cstdio>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {
namespace linux  {

ErrProcessState getProcessState(int pid)
{
    if (!isLinuxPlatform()) {
        return E_ENOSYS;
    }

    // /proc/[pid]/stat
    // Status information about the process. This is used by ps(1).
    // It is defined in the kernel source file fs/proc/array.c.
    auto const path = libtbag::string::fformat("/proc/{}/stat", pid);
    if (!libtbag::filesystem::Path(path).isReadable()) {
        return E_RDERR;
    }

    std::string content;
    auto const code = libtbag::filesystem::readFile(path, content);
    if (isFailure(code)) {
        return code;
    }
    assert(!content.empty());

    std::istringstream ss(content);
    ProcessState result;
    ss >> result.pid >> result.comm >> result.state >> result.ppid >> result.pgrp
       >> result.session >> result.tty_nr >> result.tpgid >> result.flags >> result.minflt
       >> result.cminflt >> result.majflt >> result.cmajflt >> result.utime >> result.stime
       >> result.cutime >> result.cstime >> result.priority >> result.nice >> result.num_threads
       >> result.itrealvalue >> result.starttime >> result.vsize >> result.rss >> result.rsslim
       >> result.startcode >> result.endcode >> result.startstack >> result.kstkesp >> result.kstkeip
       >> result.signal >> result.blocked >> result.sigignore >> result.sigcatch >> result.wchan
       >> result.nswap >> result.cnswap >> result.exit_signal >> result.processor >> result.rt_priority
       >> result.policy >> result.delayacct_blkio_ticks >> result.guest_time >> result.cguest_time >> result.start_data
       >> result.end_data >> result.start_brk >> result.arg_start >> result.arg_end >> result.env_start
       >> result.env_end >> result.exit_code;
    return { E_SUCCESS, result };
}

std::string getPrintableTextOfProcessState(ProcessState const & state)
{
    std::stringstream ss;
    // clang-format off
    ss << "pid: "                     << state.pid
       << "\ncomm: "                  << state.comm
       << "\nstate: "                 << state.state
       << "\nppid: "                  << state.ppid
       << "\npgrp: "                  << state.pgrp
       << "\nsession: "               << state.session
       << "\ntty_nr: "                << state.tty_nr
       << "\ntpgid: "                 << state.tpgid
       << "\nflags: "                 << state.flags
       << "\nminflt: "                << state.minflt
       << "\ncminflt: "               << state.cminflt
       << "\nmajflt: "                << state.majflt
       << "\ncmajflt: "               << state.cmajflt
       << "\nutime: "                 << state.utime
       << "\nstime: "                 << state.stime
       << "\ncutime: "                << state.cutime
       << "\ncstime: "                << state.cstime
       << "\npriority: "              << state.priority
       << "\nnice: "                  << state.nice
       << "\nnum_threads: "           << state.num_threads
       << "\nitrealvalue: "           << state.itrealvalue
       << "\nstarttime: "             << state.starttime
       << "\nvsize: "                 << state.vsize
       << "\nrss: "                   << state.rss
       << "\nrsslim: "                << state.rsslim
       << "\nstartcode: "             << state.startcode
       << "\nendcode: "               << state.endcode
       << "\nstartstack: "            << state.startstack
       << "\nkstkesp: "               << state.kstkesp
       << "\nkstkeip: "               << state.kstkeip
       << "\nsignal: "                << state.signal
       << "\nblocked: "               << state.blocked
       << "\nsigignore: "             << state.sigignore
       << "\nsigcatch: "              << state.sigcatch
       << "\nwchan: "                 << state.wchan
       << "\nnswap: "                 << state.nswap
       << "\ncnswap: "                << state.cnswap
       << "\nexit_signal: "           << state.exit_signal
       << "\nprocessor: "             << state.processor
       << "\nrt_priority: "           << state.rt_priority
       << "\npolicy: "                << state.policy
       << "\ndelayacct_blkio_ticks: " << state.delayacct_blkio_ticks
       << "\nguest_time: "            << state.guest_time
       << "\ncguest_time: "           << state.cguest_time
       << "\nstart_data: "            << state.start_data
       << "\nend_data: "              << state.end_data
       << "\nstart_brk: "             << state.start_brk
       << "\narg_start: "             << state.arg_start
       << "\narg_end: "               << state.arg_end
       << "\nenv_start: "             << state.env_start
       << "\nenv_end: "               << state.env_end
       << "\nexit_code: "             << state.exit_code;
    // clang-format on
    return ss.str();
}

} // namespace linux
} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

