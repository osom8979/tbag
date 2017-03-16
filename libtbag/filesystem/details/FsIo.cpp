/**
 * @file   FsIo.cpp
 * @brief  FsIo class implementation.
 * @author zer0
 * @date   2017-03-16
 */

#include <libtbag/filesystem/details/FsIo.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

ufile open(std::string const & path, int flags, int mode)
{
    uv_fs_t req = {0,};
    ufile f = uv_fs_open(nullptr, &req, path.c_str(), flags, mode, nullptr);
    uv_fs_req_cleanup(&req);
    return f;
}

bool close(ufile file)
{
    uv_fs_t req = {0,};
    int const ERROR_CODE = uv_fs_close(nullptr, &req, file, nullptr);
    uv_fs_req_cleanup(&req);
    return ERROR_CODE == 0;
}

int read(ufile file, binf const * infos, std::size_t infos_size, int64_t offset)
{
    std::vector<uv_buf_t> uv_infos;
    uv_infos.resize(infos_size);
    for (std::size_t i = 0; i < infos_size; ++i) {
        uv_infos[i].base = (infos + i)->buffer;
        uv_infos[i].len  = (infos + i)->size;
    }

    uv_fs_t req = {0,};
    int const READ_SIZE = uv_fs_read(nullptr, &req, file,
                                     &uv_infos[0], static_cast<unsigned int>(uv_infos.size()),
                                     offset, nullptr);
    uv_fs_req_cleanup(&req);
    return READ_SIZE;
}

int write(ufile file, binf const * infos, std::size_t infos_size, int64_t offset)
{
    std::vector<uv_buf_t> uv_infos;
    uv_infos.resize(infos_size);
    for (std::size_t i = 0; i < infos_size; ++i) {
        uv_infos[i].base = (infos + i)->buffer;
        uv_infos[i].len  = (infos + i)->size;
    }

    uv_fs_t req = {0,};
    int const WRITE_SIZE = uv_fs_write(nullptr, &req, file,
                                     &uv_infos[0], static_cast<unsigned int>(uv_infos.size()),
                                     offset, nullptr);
    uv_fs_req_cleanup(&req);
    return WRITE_SIZE;
}

int read2(ufile file, char const * buffer, std::size_t size, int64_t offset)
{
    uv_buf_t buf;
    buf.base = (char*)buffer;
    buf.len  = size;

    uv_fs_t req = {0,};
    int const READ_SIZE = uv_fs_read(nullptr, &req, file, &buf, 1U, offset, nullptr);
    uv_fs_req_cleanup(&req);
    return READ_SIZE;
}

int write2(ufile file, char const * buffer, std::size_t size, int64_t offset)
{
    uv_buf_t buf;
    buf.base = (char*)buffer;
    buf.len  = size;

    uv_fs_t req = {0,};
    int const WRITE_SIZE = uv_fs_write(nullptr, &req, file, &buf, 1U, offset, nullptr);
    uv_fs_req_cleanup(&req);
    return WRITE_SIZE;
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

