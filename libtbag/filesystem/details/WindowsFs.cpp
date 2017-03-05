/**
 * @file   WindowsFs.cpp
 * @brief  Windows Filesystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 * @date   2016-12-13 (Rename: WindowsFileSystem -> WindowsFs)
 */

#include <libtbag/filesystem/details/WindowsFs.hpp>
#include <libtbag/filesystem/details/FsTemplate.hpp-inl>
#include <libtbag/filesystem/details/FsUtils.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/filesystem/details/FsScan.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <cassert>

#include <unicode/unistr.h>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
# include <Shlobj.h>  // SHGetFolderPath
# include <Shlwapi.h> // PathFileExists
# include <Strsafe.h> // StringCchLength, etc ...
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif
#include <libtbag/proxy/windows/String.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace windows    {

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

