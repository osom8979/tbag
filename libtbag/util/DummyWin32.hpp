/**
 * @file   DummyWin32.hpp
 * @brief  DummyWin32 class prototype.
 * @author zer0
 * @date   2016-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_DUMMYWIN32_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_DUMMYWIN32_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#if !defined(__PLATFORM_WINDOWS__)
# ifndef far
# define far
# endif

# ifndef near
# define near
# endif

# ifndef BOOL
# define BOOL int
# endif

# ifndef CHAR
# define CHAR char
# endif

# ifndef WCHAR
# define WCHAR wchar_t
# endif

# ifndef LPBOOL
# define LPBOOL BOOL far *
# endif

# ifndef LONG_PTR
# define LONG_PTR long long
# endif

# ifndef DWORD
# define DWORD unsigned long
# endif

# ifndef PDWORD
# define PDWORD DWORD near *
# endif

# ifndef LPDWORD
# define LPDWORD DWORD far *
# endif

# ifndef UINT
# define UINT unsigned int
# endif

# ifndef LONG
# define LONG long
# endif

# ifndef LPSTR
# define LPSTR char far *
# endif

# ifndef LPCSTR
# define LPCSTR char const far *
# endif

# ifndef LPWSTR
# define LPWSTR wchar_t far *
# endif

# ifndef PCWSTR
# define PCWSTR wchar_t const near *
# endif

# ifndef LPCWSTR
# define LPCWSTR wchar_t const far *
# endif

# ifndef PVOID
# define PVOID void near *
# endif

# ifndef HRESULT
# define HRESULT long
# endif

# ifndef LPSECURITY_ATTRIBUTES
# define LPSECURITY_ATTRIBUTES PVOID
# endif

# ifndef SECURITY_INFORMATION
# define SECURITY_INFORMATION DWORD
# endif

# ifndef PSECURITY_INFORMATION
# define PSECURITY_INFORMATION DWORD *
# endif

# ifndef PSECURITY_DESCRIPTOR
# define PSECURITY_DESCRIPTOR PVOID
# endif

# ifndef HWND
# define HWND PVOID
# endif

# ifndef HANDLE
# define HANDLE PVOID
# endif

# ifndef HINSTANCE
# define HINSTANCE PVOID
# endif

# ifndef HMODULE
# define HMODULE HINSTANCE
# endif

# ifndef PHANDLE
# define PHANDLE HANDLE *
# endif

# ifndef _In_
# define _In_
# endif

# ifndef _In_opt_
# define _In_opt_
# endif

# ifndef _Out_
# define _Out_
# endif

# ifndef _Out_opt_
# define _Out_opt_
# endif

# ifndef _Inout_
# define _Inout_
# endif

# ifndef TRUE
# define TRUE 1
# endif

# ifndef FALSE
# define FALSE 0
# endif

# ifndef S_OK
# define S_OK 0x00000000 // Operation successful.
# endif

# ifndef E_FAIL
# define E_FAIL 0x80004005 // Unspecified failure.
# endif

# ifndef ERROR_INVALID_FUNCTION
# define ERROR_INVALID_FUNCTION 0x01 // Incorrect function.
# endif

# ifndef ERROR_INSUFFICIENT_BUFFER
# define ERROR_INSUFFICIENT_BUFFER 0x7A // The data area passed to a system call is too small.
# endif

# ifndef INVALID_FILE_SIZE
# define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)
# endif

# ifndef INVALID_SET_FILE_POINTER
# define INVALID_SET_FILE_POINTER ((DWORD)-1)
# endif

# ifndef FILE_ATTRIBUTE_DIRECTORY
# define FILE_ATTRIBUTE_DIRECTORY 0x00000010
# endif

# ifndef FILE_ATTRIBUTE_ARCHIVE
# define FILE_ATTRIBUTE_ARCHIVE 0x00000020
# endif

# ifndef INVALID_FILE_ATTRIBUTES
# define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
# endif

# ifndef INVALID_HANDLE_VALUE
# define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
# endif

# ifndef CP_ACP
# define CP_ACP 0 // default to ANSI code page
# endif

# ifndef OWNER_SECURITY_INFORMATION
# define OWNER_SECURITY_INFORMATION 0x00000001L
# endif

# ifndef GROUP_SECURITY_INFORMATION
# define GROUP_SECURITY_INFORMATION 0x00000002L
# endif

# ifndef DACL_SECURITY_INFORMATION
# define DACL_SECURITY_INFORMATION 0x00000004L
# endif

# ifndef SACL_SECURITY_INFORMATION
# define SACL_SECURITY_INFORMATION 0x00000008L
# endif

# ifndef TOKEN_DUPLICATE
# define TOKEN_DUPLICATE 0x0002
# endif

# ifndef TOKEN_IMPERSONATE
# define TOKEN_IMPERSONATE 0x0004
# endif

# ifndef TOKEN_QUERY
# define TOKEN_QUERY 0x0008
# endif

# ifndef READ_CONTROL
# define READ_CONTROL 0x00020000L
# endif

# ifndef SYNCHRONIZE
# define SYNCHRONIZE 0x00100000L
# endif

# ifndef STANDARD_RIGHTS_REQUIRED
# define STANDARD_RIGHTS_REQUIRED 0x000F0000L
# endif

# ifndef STANDARD_RIGHTS_READ
# define STANDARD_RIGHTS_READ READ_CONTROL
# endif

# ifndef STANDARD_RIGHTS_WRITE
# define STANDARD_RIGHTS_WRITE READ_CONTROL
# endif

# ifndef STANDARD_RIGHTS_EXECUTE
# define STANDARD_RIGHTS_EXECUTE READ_CONTROL
# endif

# ifndef SECURITY_IMPERSONATION_LEVEL
# define SECURITY_IMPERSONATION_LEVEL int
# endif

# ifndef SecurityImpersonation
# define SecurityImpersonation 2
# endif

# ifndef GENERIC_READ
# define GENERIC_READ 0x80000000L
# endif

# ifndef GENERIC_WRITE
# define GENERIC_WRITE 0x40000000L
# endif

# ifndef GENERIC_EXECUTE
# define GENERIC_EXECUTE 0x20000000L
# endif

# ifndef GENERIC_ALL
# define GENERIC_ALL 0x10000000L
# endif

# ifndef ACCESS_MASK
# define ACCESS_MASK DWORD
# endif

# ifndef ANYSIZE_ARRAY
# define ANYSIZE_ARRAY 1
# endif

# ifndef FILE_READ_DATA
# define FILE_READ_DATA 0x0001
# endif

# ifndef FILE_READ_ATTRIBUTES
# define FILE_READ_ATTRIBUTES 0x0080
# endif

# ifndef FILE_READ_EA
# define FILE_READ_EA 0x0008
# endif

# ifndef FILE_WRITE_DATA
# define FILE_WRITE_DATA 0x0002
# endif

# ifndef FILE_WRITE_ATTRIBUTES
# define FILE_WRITE_ATTRIBUTES 0x0100
# endif

# ifndef FILE_WRITE_EA
# define FILE_WRITE_EA 0x0010
# endif

# ifndef FILE_APPEND_DATA
# define FILE_APPEND_DATA 0x0004
# endif

# ifndef FILE_EXECUTE
# define FILE_EXECUTE 0x0020
# endif

# ifndef FILE_ALL_ACCESS
# define FILE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF)
# endif

# ifndef FILE_GENERIC_READ
# define FILE_GENERIC_READ (STANDARD_RIGHTS_READ | FILE_READ_DATA | FILE_READ_ATTRIBUTES | FILE_READ_EA | SYNCHRONIZE)
# endif

# ifndef FILE_GENERIC_WRITE
# define FILE_GENERIC_WRITE (STANDARD_RIGHTS_WRITE | FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA | FILE_APPEND_DATA | SYNCHRONIZE)
# endif

# ifndef FILE_GENERIC_EXECUTE
# define FILE_GENERIC_EXECUTE (STANDARD_RIGHTS_EXECUTE | FILE_READ_ATTRIBUTES | FILE_EXECUTE | SYNCHRONIZE)
# endif

# ifndef CSIDL_PROFILE
# define CSIDL_PROFILE 0x0028 // USERPROFILE
# endif

# ifndef SUCCEEDED
# define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
# endif

# ifndef FAILED
# define FAILED(hr) (((HRESULT)(hr)) < 0)
# endif

# ifndef MAX_PATH
# define MAX_PATH 260
# endif
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

typedef struct _GENERIC_MAPPING {
    ACCESS_MASK GenericRead;    ///< Specifies an access mask defining read access to an object.
    ACCESS_MASK GenericWrite;   ///< Specifies an access mask defining write access to an object.
    ACCESS_MASK GenericExecute; ///< Specifies an access mask defining execute access to an object.
    ACCESS_MASK GenericAll;     ///< Specifies an access mask defining all possible types of access to an object.
} GENERIC_MAPPING, * PGENERIC_MAPPING;

typedef struct _LUID {
    DWORD LowPart;
    LONG HighPart;
} LUID, *PLUID;

typedef struct _LUID_AND_ATTRIBUTES {
    LUID Luid;
    DWORD Attributes;
} LUID_AND_ATTRIBUTES, * PLUID_AND_ATTRIBUTES;

typedef struct _PRIVILEGE_SET {
    DWORD PrivilegeCount;
    DWORD Control;
    LUID_AND_ATTRIBUTES Privilege[ANYSIZE_ARRAY];
} PRIVILEGE_SET, * PPRIVILEGE_SET;

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct _WIN32_FIND_DATAW {
    DWORD    dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD    nFileSizeHigh;
    DWORD    nFileSizeLow;
    DWORD    dwReserved0;
    DWORD    dwReserved1;
    WCHAR    cFileName[MAX_PATH];
    WCHAR    cAlternateFileName[14];
#ifdef _MAC
    DWORD dwFileType;
    DWORD dwCreatorType;
    WORD  wFinderFlags;
#endif
} WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms679360(v=vs.85).aspx>
 *
 * @remarks
 *  - WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline DWORD GetLastError(void)
{ return ERROR_INVALID_FUNCTION; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd319072(v=vs.85).aspx>
 *
 * @return
 *  The function returns 0 if it does not succeed.
 *
 * @remarks
 *  - Header: Stringapiset.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline int MultiByteToWideChar(_In_      UINT   CodePage,
                               _In_      DWORD  dwFlags,
                               _In_      LPCSTR lpMultiByteStr,
                               _In_      int    cbMultiByte,
                               _Out_opt_ LPWSTR lpWideCharStr,
                               _In_      int    cchWideChar)
{ return 0; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd374130(v=vs.85).aspx>
 *
 * @return
 *  The function returns 0 if it does not succeed.
 *
 * @remarks
 *  - Header: Stringapiset.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline int WideCharToMultiByte(_In_      UINT    CodePage,
                               _In_      DWORD   dwFlags,
                               _In_      LPCWSTR lpWideCharStr,
                               _In_      int     cchWideChar,
                               _Out_opt_ LPSTR   lpMultiByteStr,
                               _In_      int     cbMultiByte,
                               _In_opt_  LPCSTR  lpDefaultChar,
                               _Out_opt_ LPBOOL  lpUsedDefaultChar)
{ return 0; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364944(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is INVALID_FILE_ATTRIBUTES.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline DWORD GetFileAttributesW(_In_ LPCWSTR lpFileName)
{ return INVALID_FILE_ATTRIBUTES; }

/**
 * @see <https://msdn.microsoft.com/en-us/library/windows/desktop/aa364980(v=vs.85).aspx>
 *
 * @return
 *  If the function fails for any other reason, such as if the file does not exist, the return value is zero.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline DWORD GetLongPathNameW(_In_  LPCWSTR lpszShortPath,
                              _Out_ LPWSTR  lpszLongPath,
                              _In_  DWORD   cchBuffer)
{ return 0; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa446639(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Advapi32.lib
 */
inline BOOL GetFileSecurityW(_In_      LPCWSTR              lpFileName,
                             _In_      SECURITY_INFORMATION RequestedInformation,
                             _Out_opt_ PSECURITY_DESCRIPTOR pSecurityDescriptor,
                             _In_      DWORD                nLength,
                             _Out_     LPDWORD              lpnLengthNeeded)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa379295(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Advapi32.lib
 */
inline BOOL OpenProcessToken(_In_  HANDLE  ProcessHandle,
                             _In_  DWORD   DesiredAccess,
                             _Out_ PHANDLE TokenHandle)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms683179(v=vs.85).aspx>
 *
 * @return
 *  The return value is a pseudo handle to the current process.
 *
 * @remarks
 *  - Header: WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline HANDLE GetCurrentProcess(void)
{ return nullptr; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms724211(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline BOOL CloseHandle(_In_ HANDLE hObject)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa446616(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Advapi32.lib
 */
inline BOOL DuplicateToken(_In_  HANDLE                       ExistingTokenHandle,
                           _In_  SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
                           _Out_ PHANDLE                      DuplicateTokenHandle)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa379266(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Advapi32.lib
 */
inline void MapGenericMask(_Inout_ PDWORD           AccessMask,
                           _In_    PGENERIC_MAPPING GenericMapping)
{ /* EMPTY. */ }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa379266(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Advapi32.lib
 */
inline BOOL AccessCheck(_In_      PSECURITY_DESCRIPTOR pSecurityDescriptor,
                        _In_      HANDLE               ClientToken,
                        _In_      DWORD                DesiredAccess,
                        _In_      PGENERIC_MAPPING     GenericMapping,
                        _Out_opt_ PPRIVILEGE_SET       PrivilegeSet,
                        _Inout_   LPDWORD              PrivilegeSetLength,
                        _Out_     LPDWORD              GrantedAccess,
                        _Out_     LPBOOL               AccessStatus)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364992(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline DWORD GetTempPathA(_In_  DWORD nBufferLength,
                          _Out_ LPSTR lpBuffer)
{ return 0; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364934(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline DWORD GetCurrentDirectoryA(_In_  DWORD nBufferLength,
                           _Out_ LPSTR lpBuffer)
{ return 0; }

/**
 * @see <https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181.aspx>
 *
 * @return
 *  If this function succeeds, it returns S_OK. Otherwise, it returns an HRESULT error code.
 *
 * @remarks
 *  - Header: Shlobj.h
 *  - Library: Shell32.lib
 */
inline HRESULT SHGetFolderPathA(_In_  HWND   hwndOwner,
                                _In_  int    nFolder,
                                _In_  HANDLE hToken,
                                _In_  DWORD  dwFlags,
                                _Out_ LPSTR  pszPath)
{ return E_FAIL; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms683197(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is 0 (zero).
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline DWORD GetModuleFileNameA(_In_opt_ HMODULE hModule,
                                _Out_    LPSTR   lpFilename,
                                _In_     DWORD   nSize)
{ return 0; }

/**
 * @see <https://msdn.microsoft.com/en-us/library/windows/desktop/aa364963(v=vs.85).aspx>
 *
 * @return
 *  If the function fails for any other reason, the return value is zero.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline DWORD GetFullPathNameW(_In_  LPCWSTR  lpFileName,
                              _In_  DWORD    nBufferLength,
                              _Out_ LPWSTR   lpBuffer,
                              _Out_ LPWSTR  *lpFilePart)
{ return 0; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa363855(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline BOOL CreateDirectoryW(_In_     LPCWSTR               lpPathName,
                             _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365488(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline BOOL RemoveDirectoryW(_In_ LPCWSTR lpPathName)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa363915(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero (0).
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline BOOL DeleteFileW(_In_ LPCWSTR lpFileName)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365239(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline BOOL MoveFileW(_In_ LPCWSTR lpExistingFileName,
                      _In_ LPCWSTR lpNewFileName)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb773584(v=vs.85).aspx>
 *
 * @return
 *  TRUE if the file exists; otherwise, FALSE.
 *
 * @remarks
 *  - Header: Shlwapi.h
 *  - Library: Shlwapi.lib
 */
inline BOOL PathFileExistsW(_In_ LPCWSTR pszPath)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms647539(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Strsafe.h
 */
inline HRESULT StringCchLengthW(_In_  LPCWSTR   psz,
                                _In_  size_t    cchMax,
                                _Out_ size_t  * pcch)
{ return E_FAIL; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms647527(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Strsafe.h
 */
inline HRESULT StringCchCopyW(_Out_ LPWSTR  pszDest,
                              _In_  size_t  cchDest,
                              _In_  LPCWSTR pszSrc)
{ return E_FAIL; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms647518(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Strsafe.h
 */
inline HRESULT StringCchCatW(_Inout_ LPWSTR  pszDest,
                             _In_    size_t  cchDest,
                             _In_    LPCWSTR pszSrc)
{ return E_FAIL; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb759938(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Shlwapi.h
 *  - Library: Shlwapi.lib
 */
inline int StrCmpW(_In_ PCWSTR psz1,
                   _In_ PCWSTR psz2)
{ return 0; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364418(v=vs.85).aspx>
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365200(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is INVALID_HANDLE_VALUE.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline HANDLE FindFirstFileW(_In_  LPCWSTR            lpFileName,
                             _Out_ LPWIN32_FIND_DATAW lpFindFileData)
{ return INVALID_HANDLE_VALUE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364428(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline BOOL FindNextFileW(_In_  HANDLE             hFindFile,
                          _Out_ LPWIN32_FIND_DATAW lpFindFileData)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364413(v=vs.85).aspx>
 *
 * @return
 *  If the function fails, the return value is zero.
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
inline BOOL FindClose(_Inout_ HANDLE hFindFile)
{ return FALSE; }

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_DUMMYWIN32_HPP__

