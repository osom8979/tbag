/**
 * @file   MainArguments.hpp
 * @brief  MainArguments class prototype.
 * @author zer0
 * @date   2019-05-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_MAINARGUMENTS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_MAINARGUMENTS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * MainArguments class prototype.
 *
 * @author zer0
 * @date   2019-05-22
 */
class TBAG_API MainArguments : private Noncopyable
{
public:
    struct Cache
    {
        std::vector<char*> args_ptrs;
        std::vector<char*> envs_ptrs;
    };

private:
    std::vector<std::string> _args;
    std::vector<std::string> _envs;

private:
    Cache _cache;

public:
    MainArguments();
    MainArguments(std::string const & program);
    MainArguments(std::vector<std::string> const & args);
    MainArguments(std::string const & program,
                  std::vector<std::string> const & args);
    MainArguments(std::vector<std::string> const & args,
                  std::vector<std::string> const & envs);
    MainArguments(std::string const & program,
                  std::vector<std::string> const & args,
                  std::vector<std::string> const & envs);
    ~MainArguments();

public:
    void setArgs(std::string const & program);
    void setArgs(std::vector<std::string> const & args);
    void setArgs(std::string const & program, std::vector<std::string> const & args);
    void setEnvs(std::vector<std::string> const & args);

public:
    void updateCache();

public:
    int argc() const;
    char ** argv();
    int envc() const;
    char ** envs();
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_MAINARGUMENTS_HPP__

