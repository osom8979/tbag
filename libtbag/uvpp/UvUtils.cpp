/**
 * @file   UvUtils.cpp
 * @brief  uvpp miscellaneous utilities.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

char ** setupArgs(int argc, char ** argv)
{
    return ::uv_setup_args(argc, argv);
}

std::string getProcessTitle()
{
    std::size_t const BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE] = {0,};

    // If buffer is NULL or size is zero, UV_EINVAL is returned.
    // If size cannot accommodate the process title and terminating NULL character,
    // the function returns UV_ENOBUFS.
    int const CODE = ::uv_get_process_title(buffer, BUFFER_SIZE);
    if (CODE != 0) {
        tDLogE("getProcessTitle() {} error", getUvErrorName(CODE));
        return std::string();
    }
    return std::string(buffer);
}

void setProcessTitle(std::string const & title)
{
    // On platforms with a fixed size buffer for the process title
    // the contents of title will be copied to the buffer
    // and truncated if larger than the available space.
    // Other platforms will return UV_ENOMEM if they cannot allocate
    // enough space to duplicate the contents of title.
    int const CODE = ::uv_set_process_title(title.c_str());
    if (CODE != 0) {
        tDLogE("setProcessTitle() {} error", getUvErrorName(CODE));
    }
}

Err getEnv(std::string const & name, std::string & value)
{
#if (UV_VERSION_MAJOR >= 1) && (UV_VERSION_MINOR >= 12)
    std::size_t size = DEFAULT_ENVIRONMENT_VARIABLE_BUFFER_SIZE;
    std::vector<char> buffer(size);
    int code = ::uv_os_getenv(name.c_str(), &buffer[0], &size);
    if (code == UV_ENOBUFS) {
        assert(size > DEFAULT_ENVIRONMENT_VARIABLE_BUFFER_SIZE);
        buffer.resize(size+1/*NULL_CHAR*/);
        code = ::uv_os_getenv(name.c_str(), &buffer[0], &size);
    }
    if (code == 0) {
        value = std::string(buffer.begin(), buffer.begin() + size);
        return E_SUCCESS;
    }
    return libtbag::convertUvErrorToErr(code);
#else
    char * env_value = ::getenv(name.c_str());
    if (env_value != nullptr) {
        value = std::string(env_value);
        return E_SUCCESS;
    }
    return E_ENFOUND;
#endif
}

Err setEnv(std::string const & name, std::string const & value)
{
#if (UV_VERSION_MAJOR >= 1) && (UV_VERSION_MINOR >= 12)
    return libtbag::convertUvErrorToErr(::uv_os_setenv(name.c_str(), value.c_str()));
#else
    tDLogE("setEnv() Function not implemented.");
    return E_ENOSYS;
#endif
}

Err unsetEnv(std::string const & name)
{
#if (UV_VERSION_MAJOR >= 1) && (UV_VERSION_MINOR >= 12)
    return libtbag::convertUvErrorToErr(::uv_os_unsetenv(name.c_str()));
#else
    tDLogE("unsetEnv() Function not implemented.");
    return E_ENOSYS;
#endif
}

std::string getHostName()
{
#if (UV_VERSION_MAJOR >= 1) && (UV_VERSION_MINOR >= 16)
    char buffer[UV_MAXHOSTNAMESIZE] = {0,};
    std::size_t size = UV_MAXHOSTNAMESIZE;
    int const CODE = ::uv_os_gethostname(buffer, &size);
    if (CODE == 0) {
        return std::string(&buffer[0]);
    }
    return std::string();
#elif (UV_VERSION_MAJOR >= 1) && (UV_VERSION_MINOR >= 12)
    std::size_t size = DEFAULT_ENVIRONMENT_VARIABLE_BUFFER_SIZE;
    std::vector<char> buffer(size);
    int code = ::uv_os_gethostname(&buffer[0], &size);
    if (code == UV_ENOBUFS) {
        assert(size > DEFAULT_ENVIRONMENT_VARIABLE_BUFFER_SIZE);
        buffer.resize(size+1/*NULL_CHAR*/);
        code = ::uv_os_gethostname(&buffer[0], &size);
    }
    if (code == 0) {
        return std::string(buffer.begin(), buffer.begin() + size);
    }
    return std::string();
#else
    tDLogE("getHostName() Function not implemented.");
    return std::string();
#endif
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

