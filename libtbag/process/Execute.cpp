/**
 * @file   Execute.cpp
 * @brief  Execute class implementation.
 * @author zer0
 * @date   2017-08-03
 */

#include <libtbag/process/Execute.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/FindPath.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalProcess.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

int execute(std::string const & file,
            std::vector<std::string> const & args,
            std::vector<std::string> const & envs,
            std::string const & cwd,
            Err * code)
{
    using namespace filesystem;
    using namespace uvpp;
    using namespace uvpp::func;

    Path const EXE_FILE(file);

    if (EXE_FILE.exists() == false) {
        if (code != nullptr) {
            *code = Err::E_ENOENT;
        }
        tDLogE("execute({}) No such file or directory", file);
        return -1;
    }

    if (EXE_FILE.isExecutable() == false) {
        if (code != nullptr) {
            *code = Err::E_EACCES;
        }
        tDLogE("execute({}) Permission denied", file);
        return -1;
    }

    Loop loop;

    Process::Options options;
    options.clear();
    options.setFile(EXE_FILE.toString());
    if (cwd.empty()) {
        options.setWorking(Path::getWorkDir().toString());
    } else {
        options.setWorking(cwd);
    }

    for (auto & arg : args) {
        options.appendArgument(arg);
    }
    for (auto & env : envs) {
        options.appendEnvironment(env);
    }

    options.appendStdio(Process::StdioContainer(Process::STANDARD_INPUT_FD , true));
    options.appendStdio(Process::StdioContainer(Process::STANDARD_OUTPUT_FD, true));
    options.appendStdio(Process::StdioContainer(Process::STANDARD_ERROR_FD , true));

    int64_t exit_code = 0;
    int signal = 0;

    auto proc = loop.newHandle<FuncProcess>(loop, options);
    proc->setOnExit([&](int64_t exit_status, int term_signal){
        exit_code = exit_status;
        signal = term_signal;
        proc->close();
    });

    Err const LOOP_RESULT = loop.run();
    if (isFailure(LOOP_RESULT)) {
        if (code != nullptr) {
            *code = Err::E_ILLSTATE;
        }
        tDLogW("execute({}) Loop {} error", file, getErrName(LOOP_RESULT));
    }

    if (code != nullptr) {
        if (signal != 0) {
            *code = Err::E_UNKSIG;
        } else {
            *code = Err::E_SUCCESS;
        }
    }

    return static_cast<int>(exit_code);
}

int execute(std::string const & file,
            std::vector<std::string> const & args,
            std::vector<std::string> const & envs,
            Err * code)
{
    return execute(file, args, std::vector<std::string>(), std::string(), code);
}

int execute(std::string const & file,
            std::vector<std::string> const & args,
            Err * code)
{
    return execute(file, args, std::vector<std::string>(), code);
}

int execute(std::vector<std::string> const & args, Err * code)
{
    if (args.empty()) {
        if (code != nullptr) {
            *code = Err::E_ILLARGS;
        }
        tDLogW("execute() Illegal arguments");
        return -1;
    }
    return execute(args[0], std::vector<std::string>(args.begin() + 1, args.end()), code);
}

int execute(std::string const & args, Err * code)
{
    return execute(string::splitTokens(args, std::string(" ")), code);
}

int runShell(std::vector<std::string> const & args, Err * code)
{
    return execute(filesystem::findDefaultShell().toString(), args, code);
}

int runShellScript(std::string const & file, Err * code)
{
    return runShell({file}, code);
}

int runShellCommand(std::string const & command, Err * code)
{
    std::vector<std::string> all_arguments;
    if (isWindowsPlatform()) {
        all_arguments.emplace_back("/C"); // Carries out the command specified by string and then terminates.
    } else {
        all_arguments.emplace_back("-c"); // If the -c option is present, then commands are read from string.
    }
    all_arguments.push_back(command);
    return runShell(all_arguments, code);
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

