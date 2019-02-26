/**
 * @file   Archive.cpp
 * @brief  Archive class implementation.
 * @author zer0
 * @date   2019-02-25
 */

#include <libtbag/archive/Archive.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/system/SysInfo.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <archive.h>
#include <archive_entry.h>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

static char const * __get_archive_error_string(struct archive * a) TBAG_NOEXCEPT
{
    auto const * e = archive_error_string(a);
    return e ? e : "Unknown error";
}

std::size_t writeArchive(std::string const & output_filename,
                         std::vector<std::string> const & input_filenames,
                         std::string const & format,
                         CompressType compress)
{
    auto * a = archive_write_new();
    assert(a != nullptr);

    int code = ARCHIVE_FATAL;
    switch (compress) {
    case CompressType::CT_NONE:
        code = archive_write_add_filter_none(a);
        break;
    case CompressType::CT_GZIP:
        code = archive_write_add_filter_gzip(a);
        break;
    case CompressType::CT_BZIP2:
        code = archive_write_add_filter_bzip2(a);
        break;
    case CompressType::CT_LZMA:
        code = archive_write_add_filter_lzma(a);
        break;
    case CompressType::CT_XZ:
        code = archive_write_add_filter_xz(a);
        break;
    default:
        tDLogE("writeArchive() Unknown compress type: {}", (int)compress);
        return 0;
    }

    if (code != ARCHIVE_OK) {
        tDLogE("writeArchive() filter error({}): {}", code, __get_archive_error_string(a));
        return 0;
    }

    if (compress == CompressType::CT_GZIP) {
        // We're not able to specify an arbitrary timestamp for gzip.
        // The next best thing is to omit the timestamp entirely.
        code = archive_write_set_filter_option(a, "gzip", "timestamp", nullptr);
        if (code != ARCHIVE_OK) {
            tDLogE("writeArchive() filter option(gzip/timestamp) error({}): {}", code, __get_archive_error_string(a));
            return 0;
        }
    }

    code = archive_write_set_format_by_name(a, format.c_str());
    if (code != ARCHIVE_OK) {
        tDLogE("writeArchive() format({}) error({}): {}", format, code, __get_archive_error_string(a));
        return 0;
    }

    // do not pad the last block!!
    code = archive_write_set_bytes_in_last_block(a, 1);
    if (code != ARCHIVE_OK) {
        tDLogE("writeArchive() bytes in last block error({}): {}", code, __get_archive_error_string(a));
        return 0;
    }

    code = archive_write_open_filename(a, output_filename.c_str());
    if (code != ARCHIVE_OK) {
        tDLogE("writeArchive() file open error({}), {}", code, __get_archive_error_string(a));
        return 0;
    }

    auto const PAGE_SIZE = static_cast<std::size_t>(libtbag::system::getPageSize());
    libtbag::util::Buffer buffer(PAGE_SIZE);

    std::size_t left = 0;
    std::size_t next = 0;
    std::size_t read = 0;
    std::size_t written = 0;
    std::size_t success_count = 0;

    for (auto & item : input_filenames) {
        using namespace libtbag::filesystem;
        Path path(item);
        File file(item);

        auto const PERMISSION = details::getPermission(item);
        auto const FIXED_PERMISSION = details::getFixedPermission(PERMISSION);

        auto const FILE_STATE = file.getState();
        auto const FILE_SIZE = FILE_STATE.size;

        archive_entry * entry = archive_entry_new();
        assert(entry != nullptr);

        archive_entry_copy_sourcepath(entry, item.c_str());
        archive_entry_copy_pathname(entry, path.getName().c_str());

        archive_entry_set_size(entry, FILE_SIZE);
        archive_entry_set_filetype(entry, AE_IFREG);

        archive_entry_set_uid(entry, FILE_STATE.uid);
        archive_entry_set_gid(entry, FILE_STATE.gid);
        //archive_entry_set_uname(entry, uname);
        //archive_entry_set_gname(entry, gname);
        archive_entry_set_perm(entry, FIXED_PERMISSION);

        archive_entry_acl_clear(entry);
        archive_entry_xattr_clear(entry);
        archive_entry_set_fflags(entry, 0, 0);

        if (format == COMPRESS_FORMAT_PAX || format == COMPRESS_FORMAT_PAXR) {
            // Sparse files are a GNU tar extension.
            // Do not use them in standard tar files.
            archive_entry_sparse_clear(entry);
        }

        code = archive_write_header(a, entry);
        if (code == ARCHIVE_OK) {
            left = FILE_SIZE;
            next = 0;
            read = 0;
            written = 0;

            while (left > 0) {
                next = (left > PAGE_SIZE ? PAGE_SIZE : left);
                read = static_cast<std::size_t>(file.read(buffer.data(), next));
                if (read != next) {
                    tDLogE("writeArchive() read size mismatch: {}/{}", item, next);
                    break;
                }

                written = archive_write_data(a, buffer.data(), next);
                if (written != next) {
                    tDLogE("writeArchive() written size mismatch: {}/{}", written, next);
                    break;
                }
                left -= next;
            }

            if (left == 0) {
                ++success_count;
            }
        } else {
            tDLogE("writeArchive() header write error({}), {}", code, __get_archive_error_string(a));
        }
        archive_entry_free(entry);
    }

    code = archive_write_close(a);
    if (code != ARCHIVE_OK) {
        tDLogE("writeArchive() archive close error({}): {}", code, __get_archive_error_string(a));
    }

    code = archive_write_free(a);
    if (code != ARCHIVE_OK) {
        tDLogE("writeArchive() archive free error({}): {}", code, __get_archive_error_string(a));
    }

    return success_count;
}

Archive::Archive()
{
    // EMPTY.
}

Archive::~Archive()
{
    // EMPTY.
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

