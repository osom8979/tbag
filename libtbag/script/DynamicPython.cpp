/**
 * @file   DynamicPython.cpp
 * @brief  DynamicPython class implementation.
 * @author zer0
 * @date   2018-10-31
 */

#include <libtbag/script/DynamicPython.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/FindPath.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/process/StdProcess.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <sstream>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

DynamicPython::DynamicPython()
{
    // EMPTY.
}

DynamicPython::~DynamicPython()
{
    // EMPTY.
}

bool DynamicPython::isOpen() const
{
    return _lib.isOpen();
}

std::string DynamicPython::getPath() const
{
    return _path;
}

std::string DynamicPython::getError() const
{
    return _lib.getError();
}

bool DynamicPython::open(std::string const & path)
{
    bool const RESULT = _lib.open(path);
    if (!RESULT) {
        return false;
    }

    _path = path;

    // clang-format off
    _Py_Initialize      = (Py_Initialize)      _lib.symbol("Py_Initialize");
    _Py_Finalize        = (Py_Finalize)        _lib.symbol("Py_Finalize");
    _PyRun_SimpleString = (PyRun_SimpleString) _lib.symbol("PyRun_SimpleString");
    _PyRun_SimpleFile   = (PyRun_SimpleFile)   _lib.symbol("PyRun_SimpleFile");
    // clang-format on
    return true;
}

bool DynamicPython::openDefault()
{
    return open(findPythonLibrary());
}

void DynamicPython::close()
{
    _lib.close();
    _path.clear();
}

void DynamicPython::Initialize()
{
    _Py_Initialize();
}

void DynamicPython::Finalize()
{
    _Py_Finalize();
}

int DynamicPython::Run_SimpleString(char const * command)
{
    return _PyRun_SimpleString(command);
}

int DynamicPython::Run_SimpleFile(FILE * fp, char const * filename)
{
    return _PyRun_SimpleFile(fp, filename);
}

std::string DynamicPython::findPythonExecutable()
{
    return libtbag::filesystem::findFirstUtf8ExecuteFile(R"(^python$)").toString();
}

TBAG_CONSTEXPR static char const * const PYTHON_LIBDIR_SCRIPT =
        "import distutils.sysconfig as sc;"
        "print(sc.get_config_var('LIBDIR'));";

std::string DynamicPython::findPythonLibraryDir(std::string const & python_path)
{
    std::string output;
    std::string error;

    int64_t exit = 0;
    int term = 0;

    using namespace libtbag::process;
    auto const CODE = subprocess(python_path, {std::string("-c"), std::string(PYTHON_LIBDIR_SCRIPT)}, {},
                                 std::string(), std::string(), &exit, &term, &output, &error);
    if (isFailure(CODE)) {
        tDLogE("DynamicPython::findPythonLibraryDir({}) Run subprocess error: {}", python_path, CODE);
        return std::string();
    }

    if (!error.empty()) {
        tDLogW("DynamicPython::findPythonLibraryDir({}) Print error message: {}", python_path, error);
    }
    if (exit != 0 || term != 0) {
        tDLogW("DynamicPython::findPythonLibraryDir({}) Exit error: (exit:{}, term:{})", python_path, exit, term);
    }
    return libtbag::string::trim(output);
}

std::string DynamicPython::findPythonLibraryDir()
{
    return findPythonLibraryDir(findPythonExecutable());
}

std::string DynamicPython::findPythonLibrary(std::string const & dir)
{
    using namespace libtbag::filesystem;
    std::stringstream ss;
    ss << getLibraryPrefix() << "python.*" << "\\" << getLibrarySuffix();
    auto const FILES = findUtf8File({dir}, ss.str());
    return FILES.empty() ? std::string() : FILES[0];
}

std::string DynamicPython::findPythonLibrary()
{
    return findPythonLibrary(findPythonLibraryDir());
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

