/**
 * @file   DynamicPython.hpp
 * @brief  DynamicPython class prototype.
 * @author zer0
 * @date   2018-10-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_DYNAMICPYTHON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_DYNAMICPYTHON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/lib/SharedLibrary.hpp>

#include <cstdio>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

/**
 * DynamicPython class prototype.
 *
 * @author zer0
 * @date   2018-10-31
 */
class TBAG_API DynamicPython : private Noncopyable
{
public:
    using SharedLibrary = libtbag::lib::SharedLibrary;

public:
    using Py_Initialize      = void(*)(void);
    using Py_Finalize        = void(*)(void);
    using PyRun_SimpleString =  int(*)(char const *);
    using PyRun_SimpleFile   =  int(*)(FILE *, char const *);

private:
    std::string   _path;
    SharedLibrary _lib;

private:
    // @formatter:off
    Py_Initialize       _Py_Initialize;
    Py_Finalize         _Py_Finalize;
    PyRun_SimpleString  _PyRun_SimpleString;
    PyRun_SimpleFile    _PyRun_SimpleFile;
    // @formatter:on

public:
    DynamicPython();
    ~DynamicPython();

public:
    bool isOpen() const;
    std::string getPath() const;
    std::string getError() const;

public:
    bool open(std::string const & path);
    bool openDefault();
    void close();

public:
    void Initialize();
    void Finalize();

public:
    int Run_SimpleString(char const * command);
    int Run_SimpleFile(FILE * fp, char const * filename);

public:
    static std::string findPythonExecutable();
    static std::string findPythonLibraryDir(std::string const & python_path);
    static std::string findPythonLibraryDir();
    static std::string findPythonLibrary(std::string const & dir);
    static std::string findPythonLibrary();
};

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_DYNAMICPYTHON_HPP__

