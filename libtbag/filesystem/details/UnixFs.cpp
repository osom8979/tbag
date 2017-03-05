/**
 * @file   UnixFs.cpp
 * @brief  Unix Filesystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 * @date   2016-12-13 (Rename: UnixFileSystem -> UnixFs)
 *
 * @remarks
 *  Use the libuv.
 */

#include <libtbag/filesystem/details/UnixFs.hpp>
#include <libtbag/filesystem/details/FsTemplate.hpp-inl>
#include <libtbag/filesystem/details/FsUtils.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/filesystem/details/FsScan.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>

#include <cstdio>

#include <fcntl.h>
#include <sys/stat.h>

#include <unicode/unistr.h>
#include <uv.h>

STATIC_ASSERT_CHECK_IS_SAME(int, uv_file);

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace unix       {

} // namespace unix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

