/**
 * @file   Process.cpp
 * @brief  Process class implementation.
 * @author zer0
 * @date   2016-11-27
 */

#include <libtbag/process/Process.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

std::string getExecutableSuffix()
{
#if defined(__OS_WINDOWS__)
    return ".exe";
#else
    return "";
#endif
}

std::string getExecutableName(std::string const & name)
{
    return name + getExecutableSuffix();
}

// -----------------------------
// Process class implementation.
// -----------------------------

Process::Process()
{
    // EMPTY.
}

Process::~Process()
{
    // EMPTY.
}

bool Process::exe(std::string const & exe_path, std::string const & work_dir)
{
}

bool Process::exe(std::string const & exe_path)
{
}

int64_t Process::getExitStatus() const
{
}

int Process::getTerminateSignal() const
{
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

