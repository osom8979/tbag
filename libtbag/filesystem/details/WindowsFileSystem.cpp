/**
 * @file   WindowsFileSystem.cpp
 * @brief  WindowsFileSystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 */

#include <libtbag/filesystem/details/WindowsFileSystem.hpp>
#include <libtbag/filesystem/details/CommonFileSystem.hpp>
#include <libtbag/log/Log.hpp>

#if defined(__PLATFORM_WINDOWS__)
#include <Windows.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace windows    {

std::string getTempDir()
{
#if defined(__PLATFORM_WINDOWS__)
    // The maximum possible return value is MAX_PATH+1 (261).
    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    DWORD const COPIED_LENGTH = GetTempPathA(BUFFER_LENGTH, buffer);
    // COPIED_LENGTH is not including the terminating null character.

    if (COPIED_LENGTH == 0) {
        __tbag_error_f("GetTempPathA() ERROR: {}", GetLastError());
        return std::string();
    }
    return std::string(buffer);
#else
    return std::string();
#endif
}

std::string getWorkDir()
{
#if defined(__PLATFORM_WINDOWS__)
    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    // Multithreaded applications and shared library code should not use the GetCurrentDirectory function
    // and should avoid using relative path names.
    DWORD const WRITTEN_LENGTH = GetCurrentDirectoryA(BUFFER_LENGTH, buffer);

    if (WRITTEN_LENGTH == 0) {
        cerr << "GetCurrentDirectoryA() FIRST ERROR: " << GetLastError() << endl;
        return std::string();

    } else if (WRITTEN_LENGTH > BUFFER_LENGTH) {
        // If the buffer that is pointed to by lpBuffer is not large enough,
        // the return value specifies the required size of the buffer,
        // in characters, including the null-terminating character.
        std::string result;
        result.resize(WRITTEN_LENGTH);
        result[WRITTEN_LENGTH - 1] = '\0';

        if (GetCurrentDirectoryA(WRITTEN_LENGTH, &result[0]) == 0) {
            cerr << "GetCurrentDirectoryA() SECOND ERROR: " << GetLastError() << endl;
            return std::string();
        }
        return result;
    }

    return std::string(buffer);
#else
    return std::string();
#endif
}

std::string getHomeDir()
{
#if defined(__PLATFORM_WINDOWS__)
    return std::string();
#else
    return std::string();
#endif
}

std::string getExePath()
{
#if defined(__PLATFORM_WINDOWS__)
    return std::string();
#else
    return std::string();
#endif
}

std::string getRealPath(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return std::string();
#else
    return std::string();
#endif
}

bool createDirectory(std::string const & path, int mode)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool removeDirectory(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool rename(std::string const & from, std::string const & to)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool remove(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool exists(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool isDirectory(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool isRegularFile(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool isExecutable(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool isWritable(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

bool isReadable(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return false;
#else
    return false;
#endif
}

std::vector<std::string> scanDir(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return std::vector<std::string>();
#else
    return std::vector<std::string>();
#endif
}

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

