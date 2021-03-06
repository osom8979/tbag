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

    ProcessState result;
    auto * f = fopen(path.c_str(), "r");
    if (f == nullptr) {
        return E_OPEN;
    }

    // (1) pid  %d
    // (2) comm  %s
    // (3) state  %c
    // (4) ppid  %d
    // (5) pgrp  %d
    // (6) session  %d
    // (7) tty_nr  %d
    // (8) tpgid  %d
    // (9) flags  %u
    // (10) minflt  %lu
    // (11) cminflt  %lu
    // (12) majflt  %lu
    // (13) cmajflt  %lu
    // (14) utime  %lu
    // (15) stime  %lu
    // (16) cutime  %ld
    // (17) cstime  %ld
    // (18) priority  %ld
    // (19) nice  %ld
    // (20) num_threads  %ld
    // (21) itrealvalue  %ld
    // (22) starttime  %llu
    // (23) vsize  %lu
    // (24) rss  %ld
    // (25) rsslim  %lu
    // (26) startcode  %lu  [PT]
    // (27) endcode  %lu  [PT]
    // (28) startstack  %lu  [PT]
    // (29) kstkesp  %lu  [PT]
    // (30) kstkeip  %lu  [PT]
    // (31) signal  %lu
    // (32) blocked  %lu
    // (33) sigignore  %lu
    // (34) sigcatch  %lu
    // (35) wchan  %lu  [PT]
    // (36) nswap  %lu
    // (37) cnswap  %lu
    // (38) exit_signal  %d  (since Linux 2.1.22)
    // (39) processor  %d  (since Linux 2.2.8)
    // (40) rt_priority  %u  (since Linux 2.5.19)
    // (41) policy  %u  (since Linux 2.5.19)
    // (42) delayacct_blkio_ticks  %llu  (since Linux 2.6.18)
    // (43) guest_time  %lu  (since Linux 2.6.24)
    // (44) cguest_time  %ld  (since Linux 2.6.24)
    // (45) start_data  %lu  (since Linux 3.3)  [PT]
    // (46) end_data  %lu  (since Linux 3.3)  [PT]
    // (47) start_brk  %lu  (since Linux 3.3)  [PT]
    // (48) arg_start  %lu  (since Linux 3.5)  [PT]
    // (49) arg_end  %lu  (since Linux 3.5)  [PT]
    // (50) env_start  %lu  (since Linux 3.5)  [PT]
    // (51) env_end  %lu  (since Linux 3.5)  [PT]
    // (52) exit_code  %d  (since Linux 3.5)  [PT]
    TBAG_CONSTEXPR static char const * const scan_format =
            "%d "    "%s "    "%c "   "%d "   "%d "
            "%d "    "%d "    "%d "   "%u "   "%lu "
            "%lu "   "%lu "   "%lu "  "%lu "  "%lu "
            "%ld "   "%ld "   "%ld "  "%ld "  "%ld "
            "%ld "   "%llu "  "%lu "  "%ld "  "%lu "
            "%lu "   "%lu "   "%lu "  "%lu "  "%lu "
            "%lu "   "%lu "   "%lu "  "%lu "  "%lu "
            "%lu "   "%lu "   "%d "   "%d "   "%u "
            "%u "    "%llu "  "%lu "  "%ld "  "%lu "
            "%lu "   "%lu "   "%lu "  "%lu "  "%lu "
            "%lu "   "%d";

    ::fscanf(f, scan_format,
             &result.pid, result.comm, &result.state, &result.ppid, &result.pgrp,
             &result.session, &result.tty_nr, &result.tpgid, &result.flags, &result.minflt,
             &result.cminflt, &result.majflt, &result.cmajflt, &result.utime, &result.stime,
             &result.cutime, &result.cstime, &result.priority, &result.nice, &result.num_threads,
             &result.itrealvalue, &result.starttime, &result.vsize, &result.rss, &result.rsslim,
             &result.startcode, &result.endcode, &result.startstack, &result.kstkesp, &result.kstkeip,
             &result.signal, &result.blocked, &result.sigignore, &result.sigcatch, &result.wchan,
             &result.nswap, &result.cnswap, &result.exit_signal, &result.processor, &result.rt_priority,
             &result.policy, &result.delayacct_blkio_ticks, &result.guest_time, &result.cguest_time, &result.start_data,
             &result.end_data, &result.start_brk, &result.arg_start, &result.arg_end, &result.env_start,
             &result.env_end, &result.exit_code);
    ::fclose(f);

//    std::string content;
//    auto const code = libtbag::filesystem::readFile(path, content);
//    if (isFailure(code)) {
//        return code;
//    }
//    assert(!content.empty());

//    std::istringstream ss(content);
//    ss >> result.pid >> result.comm >> result.state >> result.ppid >> result.pgrp
//       >> result.session >> result.tty_nr >> result.tpgid >> result.flags >> result.minflt
//       >> result.cminflt >> result.majflt >> result.cmajflt >> result.utime >> result.stime
//       >> result.cutime >> result.cstime >> result.priority >> result.nice >> result.num_threads
//       >> result.itrealvalue >> result.starttime >> result.vsize >> result.rss >> result.rsslim
//       >> result.startcode >> result.endcode >> result.startstack >> result.kstkesp >> result.kstkeip
//       >> result.signal >> result.blocked >> result.sigignore >> result.sigcatch >> result.wchan
//       >> result.nswap >> result.cnswap >> result.exit_signal >> result.processor >> result.rt_priority
//       >> result.policy >> result.delayacct_blkio_ticks >> result.guest_time >> result.cguest_time >> result.start_data
//       >> result.end_data >> result.start_brk >> result.arg_start >> result.arg_end >> result.env_start
//       >> result.env_end >> result.exit_code;
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

