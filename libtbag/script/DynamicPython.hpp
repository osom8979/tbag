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

private:
    std::string   _path;
    SharedLibrary _lib;

public:
    DynamicPython();
    ~DynamicPython();

public:
    bool isOpen() const;
    std::string getPath() const;
    std::string getError() const;

public:
    bool open(std::string const & path);
    void close();

public:
    void Py_Initialize();
    void Py_Finalize();

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

