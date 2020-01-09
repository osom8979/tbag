/**
 * @file   TempDirGuard.cpp
 * @brief  TempDirGuard class implementation.
 * @author zer0
 * @date   2018-03-24
 */

#include <libtbag/filesystem/TempDirGuard.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

TempDirGuard::TempDirGuard() : _auto_remove(false), _temp_dir()
{
    // EMPTY.
}

TempDirGuard::TempDirGuard(std::string const & case_name,
                           std::string const & name,
                           std::string const & prefix,
                           bool auto_create,
                           bool auto_remove)
        : _auto_remove(false), _temp_dir()
{
    init(case_name, name, prefix, auto_create, auto_remove);
}

TempDirGuard::TempDirGuard(std::string const & case_name,
                           std::string const & name,
                           bool auto_create,
                           bool auto_remove)
        : _auto_remove(false), _temp_dir()
{
    init(case_name, name, auto_create, auto_remove);
}

TempDirGuard::~TempDirGuard()
{
    release();
}

void TempDirGuard::init(std::string const & case_name,
                        std::string const & name,
                        std::string const & prefix,
                        bool auto_create,
                        bool auto_remove)
{
    _auto_remove = auto_remove;
    _temp_dir = Path(prefix) / (case_name + "-" + name);
    if (!auto_create) {
        return;
    }
    if (!_temp_dir.createDir()) {
        tDLogW("TempDirGuard::init() Create directory failure: {}", _temp_dir);
    }
}

void TempDirGuard::init(std::string const & case_name,
                        std::string const & name,
                        bool auto_create,
                        bool auto_remove)
{
    init(case_name, name, getDefaultPrefix(), auto_create, auto_remove);
}

void TempDirGuard::release()
{
    if (_auto_remove) {
        if (!_temp_dir.removeAll()) {
            tDLogW("TempDirGuard::release() Remove directory failure: {}", _temp_dir);
        }
    }
}

Path TempDirGuard::getDefaultPrefix()
{
    return Path::getExeDir() / DEFAULT_TEMP_DIR_NAME;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

