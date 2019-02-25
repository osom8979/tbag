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

std::size_t write7zip(std::string const & output_filename, std::vector<std::string> const & input_filenames)
{
    struct archive * a = archive_write_new();
    int code;

    code = archive_write_add_filter_lzma(a);
    if (code != ARCHIVE_OK) {
        tDLogE("write7zip() lzma filter error: {}", code);
        return 0;
    }

    assert(a != nullptr);

    code = archive_write_set_format_7zip(a);
    if (code != ARCHIVE_OK) {
        tDLogE("write7zip() 7zip format error: {}", code);
        return 0;
    }

    code = archive_write_open_filename(a, output_filename.c_str());
    if (code != ARCHIVE_OK) {
        tDLogE("write7zip() file open error: {}", code);
        return 0;
    }

    std::size_t success_count = 0;
    for (auto & item : input_filenames) {
        libtbag::filesystem::Path path(item);

        struct archive_entry * entry = archive_entry_new();
        assert(entry != nullptr);

        archive_entry_set_pathname(entry, path.getName().c_str());
        archive_entry_set_size(entry, path.getState().size);
        archive_entry_set_filetype(entry, AE_IFREG);
        archive_entry_set_perm(entry, 0644);

        code = archive_write_header(a, entry);
        if (code == ARCHIVE_OK) {
            libtbag::util::Buffer buffer;
            auto const READ_CODE = libtbag::filesystem::readFile(item, buffer);
            if (isSuccess(READ_CODE)) {
                auto const write_size = archive_write_data(a, buffer.data(), buffer.size());
                assert(path.getState().size == buffer.size());
                assert(write_size == buffer.size());
            } else {
                tDLogE("write7zip() read item({}) error: {}", item, READ_CODE);
            }
        } else {
            tDLogE("write7zip() header write error: {}", code);
        }

        archive_entry_free(entry);
        ++success_count;
    }

    code = archive_write_close(a);
    if (code != ARCHIVE_OK) {
        tDLogE("write7zip() archive close error: {}", code);
    }

    code = archive_write_free(a);
    if (code != ARCHIVE_OK) {
        tDLogE("write7zip() archive free error: {}", code);
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

