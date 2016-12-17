/**
 * @file   tbproc.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-05-17
 * @date   2016-10-14 (Rename project: process_test -> tbproc)
 */

#include <csignal>
#include <cstring>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

static const char * const OUTPUT_FILE_NAME = "tbproc.txt";

static const char * const PARAM_NAME_OUT   = "out";
static const char * const PARAM_NAME_ERR   = "err";
static const char * const PARAM_NAME_SLEEP = "sleep";
static const char * const PARAM_NAME_FILE  = "file";

enum class ParamType : int
{
    UNKNOWN = 0,
    OUTPUT,
    ERROR,
    SLEEP,
    FILE,
};

ParamType getParamType(std::string const & command)
{
    if (command == PARAM_NAME_OUT) {
        return ParamType::OUTPUT;
    } else if (command == PARAM_NAME_ERR) {
        return ParamType::ERROR;
    } else if (command == PARAM_NAME_SLEEP) {
        return ParamType::SLEEP;
    } else if (command == PARAM_NAME_FILE) {
        return ParamType::FILE;
    }
    return ParamType::UNKNOWN;
}

std::vector<std::string> getArguments(int argc, char ** argv)
{
    std::vector<std::string> result;
    for (int i = 0; i < argc; ++i) {
        result.push_back(argv[i]);
    }
    return result;
}

template <typename OutputStream>
void runOutput(OutputStream & ostream, char const * argument = nullptr)
{
    std::string output;

    if (argument == nullptr) {
        std::cin >> output;
    } else {
        output.assign(argument);
    }

    ostream << output;
}

void runOutputCommand(std::vector<std::string> const & args)
{
    if (args.size() >= 3) {
        runOutput(std::cout, args[2].c_str());
    } else {
        runOutput(std::cout);
    }
}

void runErrorCommand(std::vector<std::string> const & args)
{
    if (args.size() >= 3) {
        runOutput(std::cerr, args[2].c_str());
    } else {
        runOutput(std::cerr);
    }
}

static volatile std::sig_atomic_t g_signal_status;

void signal_handler(int signal)
{
    g_signal_status = signal;
    std::cerr << "Signal: " << signal << std::endl;
    std::exit(signal);
}

void runSleepCommand(std::vector<std::string> const & args)
{
    std::cout << "Run sleep...\n";
    std::signal(SIGKILL, signal_handler);
    std::this_thread::sleep_for(std::chrono::minutes(1));
}

void runFileCommand(std::vector<std::string> const & args)
{
    std::ofstream file(OUTPUT_FILE_NAME);
    if (args.size() >= 3) {
        runOutput(file, args[2].c_str());
    } else {
        runOutput(file);
    }
    file.close();
}

int main(int argc, char ** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " {"
                  << PARAM_NAME_OUT   << "|"
                  << PARAM_NAME_ERR   << "|"
                  << PARAM_NAME_SLEEP << "|"
                  << PARAM_NAME_FILE  << "} {argument}\n";
        return 1;
    }

    auto args = getArguments(argc, argv);
    auto type = getParamType(args[1]);

    switch (type) {
    case ParamType::OUTPUT: runOutputCommand(args); break;
    case ParamType::ERROR : runErrorCommand (args); break;
    case ParamType::SLEEP : runSleepCommand (args); break;
    case ParamType::FILE  : runFileCommand  (args); break;
    default:
        std::cout << "Unknown command.\n";
        return 1;
    }

    return 0;
}

