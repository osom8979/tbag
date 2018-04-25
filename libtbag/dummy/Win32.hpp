/**
 * @file   Win32.hpp
 * @brief  Win32 class prototype.
 * @author zer0
 * @date   2018-01-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_WIN32_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_WIN32_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <cstdint>

#if !defined(TBAG_PLATFORM_WINDOWS)
# ifndef far
# define far
# endif

# ifndef near
# define near
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

# ifndef CALLBACK
# define CALLBACK __stdcall
# endif

# ifndef WINAPI
# define WINAPI __stdcall
# endif

# ifndef WINAPIV
# define WINAPIV __cdecl
# endif

# ifndef APIENTRY
# define APIENTRY WINAPI
# endif

# ifndef APIPRIVATE
# define APIPRIVATE __stdcall
# endif

# ifndef PASCAL
# define PASCAL __stdcall
# endif

# ifndef TRUE
# define TRUE 1
# endif

# ifndef FALSE
# define FALSE 0
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

# ifndef SHORT
# define SHORT short
# endif

# ifndef PSHORT
# define PSHORT SHORT *
# endif

# ifndef USHORT
# define USHORT unsigned short
# endif

# ifndef PUSHORT
# define PUSHORT USHORT *
# endif

# ifndef UINT
# define UINT unsigned int
# endif

# ifndef LONG
# define LONG long
# endif

# ifndef PLONG
# define PLONG LONG *
# endif

# ifndef ULONG
# define ULONG unsigned long
# endif

# ifndef PULONG
# define PULONG ULONG *
# endif

# ifndef DWORD
# define DWORD unsigned long
# endif

# ifndef PDWORD
# define PDWORD DWORD *
# endif

# ifndef LONG64
# define LONG64 int64_t
# endif

# ifndef PLONG64
# define PLONG64 LONG64 *
# endif

# ifndef ULONG64
# define ULONG64 uint64_t
# endif

# ifndef PULONG64
# define PULONG64 ULONG64 *
# endif

# ifndef DWORD64
# define DWORD64 uint64_t
# endif

# ifndef PDWORD64
# define PDWORD64 DWORD64 *
# endif

# ifndef PBOOL
# define PBOOL BOOL near *
# endif

# ifndef LPBOOL
# define LPBOOL BOOL far *
# endif

# ifndef PDWORD
# define PDWORD DWORD near *
# endif

# ifndef LPDWORD
# define LPDWORD DWORD far *
# endif

# ifndef PSTR
# define PSTR char near *
# endif

# ifndef LPSTR
# define LPSTR char far *
# endif

# ifndef PCSTR
# define PCSTR char const near *
# endif

# ifndef LPCSTR
# define LPCSTR char const far *
# endif

# ifndef PWSTR
# define PWSTR wchar_t near *
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

# ifndef LPVOID
# define LPVOID void far *
# endif

# ifndef LONG_PTR
# define LONG_PTR long long
# endif

# ifndef HRESULT
# define HRESULT long
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

# ifndef NOERROR
# define NOERROR 0
# endif

# ifndef S_OK
# define S_OK 0x00000000 // Operation successful.
# endif

# ifndef S_FALSE
# define S_FALSE 0x000000001
# endif

# ifndef E_FAIL
# define E_FAIL 0x80004005 // Unspecified failure.
# endif

# ifndef E_UNEXPECTED
# define E_UNEXPECTED 0x8000FFFF // Unexpected condition.
# endif

# ifndef E_NOTIMPL
# define E_NOTIMPL 0x80004001 // Not implemented
# endif

# ifndef E_OUTOFMEMORY
# define E_OUTOFMEMORY 0x8007000E // Ran out of memory.
# endif

# ifndef E_INVALIDARG
# define E_INVALIDARG 0x80070057 // One or more arguments are invalid.
# endif

# ifndef E_NOINTERFACE
# define E_NOINTERFACE 0x80004002 // No such interface supported.
# endif

# ifndef E_POINTER
# define E_POINTER 0x80004003 // NULL was passed incorrectly for a pointer value.
# endif

# ifndef E_HANDLE
# define E_HANDLE 0x80070006 // Invalid handle
# endif

# ifndef E_ABORT
# define E_ABORT 0x80004004 // Operation aborted
# endif

# ifndef E_ACCESSDENIED
# define E_ACCESSDENIED 0x80070005 // General access denied error.
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

# ifndef CP_UTF8
# define CP_UTF8 65001 // UTF-8.
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

# ifndef IGNORE
# define IGNORE 0 // Ignore signal
# endif

# ifndef INFINITE
# define INFINITE 0xFFFFFFFF // Infinite timeout
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

namespace dummy {
namespace win32 {

/**
 * Determines the concurrency model used for incoming calls to objects created by this thread.
 * This concurrency model can be either apartment-threaded or multithreaded.
 *
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms695279(v=vs.85).aspx>
 */
typedef enum tagCOINIT {
    COINIT_APARTMENTTHREADED  = 0x2,
    COINIT_MULTITHREADED      = 0x0,
    COINIT_DISABLE_OLE1DDE    = 0x4,
    COINIT_SPEED_OVER_MEMORY  = 0x8
} COINIT;

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

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

# ifndef IID
# define IID GUID
# endif

# ifndef REFGUID
# define REFGUID const GUID &
# endif

# ifndef REFIID
# define REFIID const IID &
# endif

# ifndef REFCLSID
# define REFCLSID const IID &
# endif

# ifndef REFFMTID
# define REFFMTID const IID &
# endif

# ifndef WIN32_DUMMY_INLINE
# define WIN32_DUMMY_INLINE inline
# endif

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms679360(v=vs.85).aspx>
 *
 * @remarks
 *  - WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
WIN32_DUMMY_INLINE DWORD GetLastError(void)
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
WIN32_DUMMY_INLINE int MultiByteToWideChar(_In_      UINT   CodePage,
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
WIN32_DUMMY_INLINE int WideCharToMultiByte(_In_      UINT    CodePage,
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
WIN32_DUMMY_INLINE DWORD GetFileAttributesW(_In_ LPCWSTR lpFileName)
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
WIN32_DUMMY_INLINE DWORD GetLongPathNameW(_In_  LPCWSTR lpszShortPath,
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
WIN32_DUMMY_INLINE BOOL GetFileSecurityW(_In_      LPCWSTR              lpFileName,
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
WIN32_DUMMY_INLINE BOOL OpenProcessToken(_In_  HANDLE  ProcessHandle,
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
WIN32_DUMMY_INLINE HANDLE GetCurrentProcess(void)
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
WIN32_DUMMY_INLINE BOOL CloseHandle(_In_ HANDLE hObject)
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
WIN32_DUMMY_INLINE BOOL DuplicateToken(_In_  HANDLE                       ExistingTokenHandle,
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
WIN32_DUMMY_INLINE void MapGenericMask(_Inout_ PDWORD           AccessMask,
                           _In_    PGENERIC_MAPPING GenericMapping)
{ /* EMPTY. */ }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa379266(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Advapi32.lib
 */
WIN32_DUMMY_INLINE BOOL AccessCheck(_In_      PSECURITY_DESCRIPTOR pSecurityDescriptor,
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
WIN32_DUMMY_INLINE DWORD GetTempPathA(_In_  DWORD nBufferLength,
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
WIN32_DUMMY_INLINE DWORD GetCurrentDirectoryA(_In_  DWORD nBufferLength,
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
WIN32_DUMMY_INLINE HRESULT SHGetFolderPathA(_In_  HWND   hwndOwner,
                                _In_  int    nFolder,
                                _In_  HANDLE hToken,
                                _In_  DWORD  dwFlags,
                                _Out_ LPSTR  pszPath)
{ return E_NOTIMPL; }

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
WIN32_DUMMY_INLINE DWORD GetModuleFileNameA(_In_opt_ HMODULE hModule,
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
WIN32_DUMMY_INLINE DWORD GetFullPathNameW(_In_  LPCWSTR  lpFileName,
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
WIN32_DUMMY_INLINE BOOL CreateDirectoryW(_In_     LPCWSTR               lpPathName,
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
WIN32_DUMMY_INLINE BOOL RemoveDirectoryW(_In_ LPCWSTR lpPathName)
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
WIN32_DUMMY_INLINE BOOL DeleteFileW(_In_ LPCWSTR lpFileName)
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
WIN32_DUMMY_INLINE BOOL MoveFileW(_In_ LPCWSTR lpExistingFileName,
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
WIN32_DUMMY_INLINE BOOL PathFileExistsW(_In_ LPCWSTR pszPath)
{ return FALSE; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms647539(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Strsafe.h
 */
WIN32_DUMMY_INLINE HRESULT StringCchLengthW(_In_  LPCWSTR   psz,
                                _In_  size_t    cchMax,
                                _Out_ size_t  * pcch)
{ return E_NOTIMPL; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms647527(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Strsafe.h
 */
WIN32_DUMMY_INLINE HRESULT StringCchCopyW(_Out_ LPWSTR  pszDest,
                              _In_  size_t  cchDest,
                              _In_  LPCWSTR pszSrc)
{ return E_NOTIMPL; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms647518(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Strsafe.h
 */
WIN32_DUMMY_INLINE HRESULT StringCchCatW(_Inout_ LPWSTR  pszDest,
                             _In_    size_t  cchDest,
                             _In_    LPCWSTR pszSrc)
{ return E_NOTIMPL; }

/**
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb759938(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Shlwapi.h
 *  - Library: Shlwapi.lib
 */
WIN32_DUMMY_INLINE int StrCmpW(_In_ PCWSTR psz1,
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
WIN32_DUMMY_INLINE HANDLE FindFirstFileW(_In_  LPCWSTR            lpFileName,
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
WIN32_DUMMY_INLINE BOOL FindNextFileW(_In_  HANDLE             hFindFile,
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
WIN32_DUMMY_INLINE BOOL FindClose(_Inout_ HANDLE hFindFile)
{ return FALSE; }

/**
 * Retrieves the process identifier of the calling process.
 *
 * @see <https://msdn.microsoft.com/en-us/library/windows/desktop/ms683180(v=vs.85).aspx>
 *
 * @return
 *  The return value is the process identifier of the calling process.
 *
 * @remarks
 *  - Header: WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
WIN32_DUMMY_INLINE DWORD GetCurrentProcessId(void)
{ return 0; }

/**
 * Captures a stack back trace by walking up the stack and recording the information for each frame.
 *
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb204633(v=vs.85).aspx>
 *
 * @return
 *  The number of captured frames.
 *
 * @remarks
 *  - Header: WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
WIN32_DUMMY_INLINE USHORT CaptureStackBackTrace(
        _In_      ULONG  FramesToSkip,
        _In_      ULONG  FramesToCapture,
        _Out_     PVOID  *BackTrace,
        _Out_opt_ PULONG BackTraceHash)
{ return 0; }

/**
 * Initializes the COM library for use by the calling thread, sets the thread's concurrency model,
 * and creates a new apartment for the thread if one is required.
 *
 * You should call Windows::Foundation::Initialize to initialize the thread instead of CoInitializeEx
 * if you want to use the Windows Runtime APIs or if you want to use both COM and Windows Runtime components.
 * Windows::Foundation::Initialize is sufficient to use for COM components.
 *
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms695279(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Objbase.h
 *  - Library: Ole32.lib
 */
WIN32_DUMMY_INLINE HRESULT CoInitializeEx(_In_opt_ LPVOID pvReserved, _In_ DWORD dwCoInit)
{ return E_NOTIMPL; }

/**
 * Closes the COM library on the current thread,
 * unloads all DLLs loaded by the thread,
 * frees any other resources that the thread maintains,
 * and forces all RPC connections on the thread to close.
 *
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms688715(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Objbase.h
 *  - Library: Ole32.lib
 */
WIN32_DUMMY_INLINE void CoUninitialize(void)
{ /* EMPTY. */ }

/**
 * The DebugCreate function creates a new client object and returns an interface pointer to it.
 *
 * @see <https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/dbgeng/nf-dbgeng-debugcreate>
 */
WIN32_DUMMY_INLINE HRESULT DebugCreate(REFIID InterfaceId, PVOID * Interface)
{ return E_NOTIMPL; }

# ifndef __uuidof
# define __uuidof(i) _GUID{0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}}
# endif

// -------------
// COM INTERFACE
// -------------

/** IUnknown interface
 *
 * Enables clients to get pointers to other interfaces on a given object through the QueryInterface method,
 * and manage the existence of the object through the AddRef and Release methods.
 * All other COM interfaces are inherited, directly or indirectly, from IUnknown.
 * Therefore, the three methods in IUnknown are the first entries in the VTable for every interface.
 *
 * @see <https://msdn.microsoft.com/en-us/library/windows/desktop/ms680509(v=vs.85).aspx>
 */
struct IUnknown
{
    /** Increments the reference count for an interface on an object. */
    WIN32_DUMMY_INLINE ULONG AddRef()
    { return 0; }

    /** Retrieves pointers to the supported interfaces on an object. */
    WIN32_DUMMY_INLINE HRESULT QueryInterface(_In_  REFIID riid,
                                              _Out_ void   **ppvObject)
    { return E_NOTIMPL; }

    /** Decrements the reference count for an interface on an object. */
    WIN32_DUMMY_INLINE ULONG Release()
    { return 0; }
};

/**
 * IDebugSymbols interface.
 *
 * @see <https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/dbgeng/nn-dbgeng-idebugsymbols>
 */
struct IDebugSymbols : public IUnknown
{
    /**
     * The GetNameByOffset method returns the name of the symbol
     * at the specified location in the target's virtual address space.
     */
    WIN32_DUMMY_INLINE HRESULT GetNameByOffset(ULONG64  Offset,
                                               PSTR     NameBuffer,
                                               ULONG    NameBufferSize,
                                               PULONG   NameSize,
                                               PULONG64 Displacement)
    { return E_NOTIMPL; }

    /**
     * The GetLineByOffset method returns the source filename and
     * the line number within the source file of an instruction in the target.
     */
    WIN32_DUMMY_INLINE HRESULT GetLineByOffset(ULONG64  Offset,
                                               PULONG   Line,
                                               PSTR     FileBuffer,
                                               ULONG    FileBufferSize,
                                               PULONG   FileSize,
                                               PULONG64 Displacement)
    { return E_NOTIMPL; }
};

# ifndef DEBUG_ATTACH_DEFAULT
# define DEBUG_ATTACH_DEFAULT                   0x00000000
# endif

# ifndef DEBUG_ATTACH_NONINVASIVE
# define DEBUG_ATTACH_NONINVASIVE               0x00000001
# endif

# ifndef DEBUG_ATTACH_EXISTING
# define DEBUG_ATTACH_EXISTING                  0x00000002
# endif

# ifndef DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND
# define DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND    0x00000004
# endif

# ifndef DEBUG_ATTACH_INVASIVE_NO_INITIAL_BREAK
# define DEBUG_ATTACH_INVASIVE_NO_INITIAL_BREAK 0x00000008
# endif

# ifndef DEBUG_ATTACH_INVASIVE_RESUME_PROCESS
# define DEBUG_ATTACH_INVASIVE_RESUME_PROCESS   0x00000010
# endif

# ifndef DEBUG_ATTACH_NONINVASIVE_ALLOW_PARTIAL
# define DEBUG_ATTACH_NONINVASIVE_ALLOW_PARTIAL 0x00000020
# endif

/**
 * IDebugClient interface.
 *
 * @see <https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/dbgeng/nn-dbgeng-idebugclient>
 */
struct IDebugClient : public IUnknown
{
    /** The AttachProcess method connects the debugger engine to a user-modeprocess. */
    WIN32_DUMMY_INLINE HRESULT AttachProcess(ULONG64 Server,
                                             ULONG   ProcessId,
                                             ULONG   AttachFlags)
    { return E_NOTIMPL; }
};

# ifndef DEBUG_WAIT_DEFAULT
# define DEBUG_WAIT_DEFAULT 0x00000000 // Wait flags.
# endif

/**
 * IDebugControl interface.
 *
 * @see <https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/dbgeng/nn-dbgeng-idebugcontrol>
 */
struct IDebugControl : public IUnknown
{
    /** The WaitForEvent method waits for an event that breaks into the debugger engine application. */
    WIN32_DUMMY_INLINE HRESULT WaitForEvent(ULONG Flags, ULONG Timeout)
    { return E_NOTIMPL; }
};

} // namespace win32
} // namespace dummy

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_WIN32_HPP__

