/**
 * @file   Common.cpp
 * @brief  Common filesystem methods.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/filesystem/Common.hpp>
#include <uv.h>

#if defined(__PLATFORM_WINDOWS__)
#include <Windows.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace common     {

static_assert(std::is_same<int, uv_file>::value, "int must be the same type as uv_file");

std::string getRepresentationDirectory(DirFunction func)
{
    std::size_t path_length = MAX_PATH_BUFFER_SIZE;
    std::vector<char> buffer;

    buffer.resize(path_length);

    if (func(&buffer[0], &path_length) != 0) {
        return std::string();
    }
    return std::string(buffer.begin(), buffer.begin() + path_length);
}

std::string getWorkDir()
{
    return getRepresentationDirectory(&uv_cwd);
}

std::string getHomeDir()
{
    return getRepresentationDirectory(&uv_os_homedir);
}

std::string getExePath()
{
    return getRepresentationDirectory(&uv_exepath);
}

std::string getExeDir()
{
    std::string path = getExePath();
    if (path.empty()) {
        return std::string();
    }

    // Separate directory & filename.
    std::string separator = std::string() + PATH_SEPARATOR;
    std::size_t last_separator_index = path.rfind(separator);
    return path.substr(0, last_separator_index);
}

std::string getOsTempDir()
{
    return getRepresentationDirectory(&uv_os_tmpdir);
}

std::string createUniqueTempDir(std::string const & prefix)
{
    std::string const PATH_TEMPLATE = prefix + "TB_XXXXXX";
    uv_fs_t request;
    std::string result;
    if (uv_fs_mkdtemp(nullptr, &request, PATH_TEMPLATE.c_str(), nullptr) == 0) {
        result = request.path;
    }
    uv_fs_req_cleanup(&request);
    return result;
}

bool checkAccessMode(std::string const & path, int mode)
{
    uv_fs_t request;
    int error_code = uv_fs_access(nullptr, &request, path.c_str(), mode, nullptr);
    uv_fs_req_cleanup(&request);
    return (error_code == 0 ? true : false);
}

bool isExistsMode(std::string const & path)
{
    return checkAccessMode(path, ACCESS_MODE_EXISTS);
}

bool isExecutableMode(std::string const & path)
{
    return checkAccessMode(path, ACCESS_MODE_EXECUTE);
}

bool isWritableMode(std::string const & path)
{
    return checkAccessMode(path, ACCESS_MODE_WRITE);
}

bool isReadableMode(std::string const & path)
{
    return checkAccessMode(path, ACCESS_MODE_READ);
}

uint64_t getStatus(std::string const & path)
{
    uint64_t result = 0;
    uv_fs_t  request;

    int error_code = uv_fs_stat(nullptr, &request, path.c_str(), nullptr);
    if (error_code == 0 && request.result == 0) {
        result = request.statbuf.st_mode;
    }
    uv_fs_req_cleanup(&request);

    return result;
}

uint64_t getPermission(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return getStatus(path) & (S_IRUSR | S_IWUSR);
#else
    return getStatus(path) & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

uint64_t getFixedPermission(uint64_t mode)
{
#if defined(__PLATFORM_WINDOWS__)
    return mode & (S_IRUSR | S_IWUSR);
#else
    return mode & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

bool checkFileType(std::string const & path, uint64_t type)
{
    return ((getStatus(path) & FILE_TYPE_S_IFMT) == type ? true : false);
}

bool isDirectory(std::string const & path)
{
    return checkFileType(path, FILE_TYPE_S_IFDIR);
}

bool isRegularFile(std::string const & path)
{
    return checkFileType(path, FILE_TYPE_S_IFREG);
}

std::set<std::string> scanDir(std::string const & dir_path)
{
    std::set<std::string> result;

    uv_fs_t request;
    uv_dirent_t dictate;

    int element_count = uv_fs_scandir(nullptr, &request, dir_path.c_str(), 0, nullptr);
    if (element_count > 0) {
        while (UV_EOF != uv_fs_scandir_next(&request, &dictate)) {
            result.insert(std::string(dictate.name));
        }
    }
    uv_fs_req_cleanup(&request);

    return result;
}

bool createDir(std::string const & path, int mode)
{
#if defined(__PLATFORM_WINDOWS__)
    return CreateDirectoryA(&path[0], NULL) == TRUE ? true : false;
#else
    uv_fs_t request;
    int error_code = uv_fs_mkdir(nullptr, &request, path.c_str(), mode, nullptr);
    uv_fs_req_cleanup(&request);

    // EXISTS DIRECTORY: (error_code == UV_EEXIST)
    return (error_code == 0 ? true : false);
#endif
}

bool removeDir(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return RemoveDirectoryA(&path[0]) == TRUE ? true : false;
#else
    uv_fs_t request;
    int error_code = uv_fs_rmdir(nullptr, &request, path.c_str(), nullptr);
    uv_fs_req_cleanup(&request);

    return (error_code == 0 ? true : false);
#endif
}

bool rename(std::string const & from, std::string const & to)
{
    uv_fs_t request;
    int error_code = uv_fs_rename(nullptr, &request, from.c_str(), to.c_str(), nullptr);
    uv_fs_req_cleanup(&request);

    return (error_code == 0 ? true : false);
}

bool remove(std::string const & path)
{
    return (::remove(path.c_str()) == 0 ? true : false);
}

int open(std::string const & path, int flags = (FILE_OPEN_FLAG_READ_WRITE | FILE_OPEN_CREATE), int mode = 0644)
{
    uv_fs_t request;
    int result = uv_fs_open(nullptr, &request, path.c_str(), flags, mode, nullptr);
    uv_fs_req_cleanup(&request);

    // Assert Check:
    //  - Source code: uv/test/test-fs-event.c
    //  - Method prototype: static void create_file(const char* name)
    return result;
}

bool close(int fd)
{
    uv_fs_t request;
    int result = uv_fs_close(nullptr, &request, static_cast<uv_file>(fd), nullptr);
    uv_fs_req_cleanup(&request);

    // Assert Check:
    //  - Source code: uv/test/test-fs-event.c
    //  - Method prototype: static void create_file(const char* name)
    return (result == 0 ? true : false);
}

int write(int fd, char const * buffer, std::size_t buffer_size, int64_t offset)
{
    uv_fs_t request;
    uv_buf_t buf;
    buf.len = buffer_size;
    buf.base = const_cast<char*>(buffer);
    int result = uv_fs_write(nullptr, &request, static_cast<uv_file>(fd), &buf, 1, offset, nullptr);
    uv_fs_req_cleanup(&request);

    return result;
}

int read(int fd, char * buffer, std::size_t buffer_size, int64_t offset)
{
    uv_fs_t request;
    uv_buf_t buf;
    buf.len = buffer_size;
    buf.base = buffer;
    int result = uv_fs_read(nullptr, &request, static_cast<uv_file>(fd), &buf, 1, offset, nullptr);
    uv_fs_req_cleanup(&request);

    return result;
}

} // namespace common
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

