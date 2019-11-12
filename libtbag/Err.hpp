/**
 * @file   Err.hpp
 * @brief  Err class prototype.
 * @author zer0
 * @date   2016-12-14
 * @date   2017-05-18 (Rename: ErrorCode -> Err)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ERR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ERR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <exception>
#include <ostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

#ifndef TBAG_ERROR_INFO_MAP
#define TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_UV_XX) \
    /* Success code **************************************************************************************************/\
    _TBAG_XX(_SUCCESS                              ,     0, "No error"                                                )\
    _TBAG_XX(_WARNING                              ,  1001, "Success, but warning"                                    )\
    _TBAG_XX(_RETRY                                ,  1002, "Retry might succeed"                                     )\
    _TBAG_XX(_CONTINUE                             ,  1003, "Wait for the next"                                       )\
    _TBAG_XX(_ASYNCREQ                             ,  1004, "Async request"                                           )\
    _TBAG_XX(_ENQASYNC                             ,  1005, "Enqueue and async"                                       )\
    /* Common error codes ********************************************************************************************/\
    _TBAG_XX(_ILLARGS                              , 10000, "Illegal arguments"                                       )\
    _TBAG_XX(_ILLSTATE                             , 10001, "Illegal state"                                           )\
    _TBAG_XX(_OPEN                                 , 10002, "Open error"                                              )\
    _TBAG_XX(_CLOSE                                , 10003, "Close error"                                             )\
    _TBAG_XX(_GET                                  , 10004, "Getter error"                                            )\
    _TBAG_XX(_SET                                  , 10005, "Setter error"                                            )\
    _TBAG_XX(_CREATE                               , 10006, "Create error"                                            )\
    _TBAG_XX(_INDEX                                , 10007, "Index error"                                             )\
    _TBAG_XX(_ALREADY                              , 10008, "Already state"                                           )\
    _TBAG_XX(_SAMEOBJ                              , 10009, "Same object"                                             )\
    _TBAG_XX(_BADALLOC                             , 10010, "Bad allocation"                                          )\
    _TBAG_XX(_OORANGE                              , 10011, "Out of range"                                            )\
    _TBAG_XX(_NULLPTR                              , 10012, "Null pointer exception"                                  )\
    _TBAG_XX(_BUFFER                               , 10013, "Unknown buffer error"                                    )\
    _TBAG_XX(_QUEUE                                , 10014, "Unknown queue error"                                     )\
    _TBAG_XX(_NREADY                               , 10015, "Not ready"                                               )\
    _TBAG_XX(_EMPTY                                , 10016, "Empty error"                                             )\
    _TBAG_XX(_INIT                                 , 10017, "Initialize error"                                        )\
    _TBAG_XX(_RDERR                                , 10018, "Unknown read error"                                      )\
    _TBAG_XX(_WRERR                                , 10019, "Unknown write error"                                     )\
    _TBAG_XX(_READONLY                             , 10020, "Read only error"                                         )\
    _TBAG_XX(_WRITEONLY                            , 10021, "Write only error"                                        )\
    _TBAG_XX(_CRYPTO                               , 10022, "Cryptographic error"                                     )\
    _TBAG_XX(_PEERAUTH                             , 10023, "Peer could not be authenticated"                         )\
    _TBAG_XX(_AMBIGUOUS                            , 10024, "Ambiguous option"                                        )\
    _TBAG_XX(_BADTYPE                              , 10025, "Incorrect type"                                          )\
    _TBAG_XX(_INTERNAL                             , 10026, "Internal error detected"                                 )\
    _TBAG_XX(_SHUTDOWN                             , 10027, "Shutdown error"                                          )\
    _TBAG_XX(_NFOUND                               , 10028, "Not found error"                                         )\
    _TBAG_XX(_TIMEOUT                              , 10029, "Timeout error"                                           )\
    _TBAG_XX(_PARSING                              , 10030, "Parsing error"                                           )\
    _TBAG_XX(_DECODE                               , 10031, "Decode error"                                            )\
    _TBAG_XX(_ENCODE                               , 10032, "Encode error"                                            )\
    _TBAG_XX(_VERIFIER                             , 10033, "Verifier error"                                          )\
    _TBAG_XX(_EXPIRED                              , 10034, "Expired object error"                                    )\
    _TBAG_XX(_INSERT                               , 10035, "Insert error"                                            )\
    _TBAG_XX(_ERASE                                , 10036, "Erase error"                                             )\
    _TBAG_XX(_CLOSING                              , 10037, "Closing object"                                          )\
    _TBAG_XX(_CLOSED                               , 10038, "Closed object"                                           )\
    _TBAG_XX(_QUERY                                , 10039, "Query error"                                             )\
    _TBAG_XX(_SMALLBUF                             , 10040, "Buffer size is small"                                    )\
    _TBAG_XX(_KEYGEN                               , 10041, "Key generator error"                                     )\
    _TBAG_XX(_UNKEXCP                              , 10042, "Unknown exception"                                       )\
    _TBAG_XX(_UNKSIG                               , 10043, "Unknown signal"                                          )\
    _TBAG_XX(_COPY                                 , 10044, "Unknown Copy error"                                      )\
    _TBAG_XX(_PUSH                                 , 10045, "Unknown Push error"                                      )\
    _TBAG_XX(_POP                                  , 10046, "Unknown Pop error"                                       )\
    _TBAG_XX(_SEND                                 , 10047, "Unknown Send error"                                      )\
    _TBAG_XX(_RECV                                 , 10048, "Unknown Recv error"                                      )\
    _TBAG_XX(_INACCES                              , 10049, "Inaccessible error"                                      )\
    _TBAG_XX(_EXITPROC                             , 10050, "Terminated process"                                      )\
    _TBAG_XX(_EEXITPROC                            , 10051, "Abnormally terminated process"                           )\
    _TBAG_XX(_EXCHANGE                             , 10052, "Error exchange"                                          )\
    _TBAG_XX(_FILTERED                             , 10053, "Filtered."                                               )\
    _TBAG_XX(_VERSION                              , 10054, "Incompatible version error"                              )\
    _TBAG_XX(_CONNECTION_RESET                     , 10055, "A connection was forcibly closed by a peer"              )\
    _TBAG_XX(_NO_RESPONSE                          , 10056, "No response"                                             )\
    _TBAG_XX(_NOT_WS_RESPONSE                      , 10057, "Not WebSocket response"                                  )\
    _TBAG_XX(_UNKNOWN_ID                           , 10058, "Unknown ID"                                              )\
    _TBAG_XX(_REJECTED                             , 10059, "Rejected"                                                )\
    _TBAG_XX(_NETWORK                              , 10060, "Unknown network error"                                   )\
    _TBAG_XX(_MODIFY                               , 10061, "Modify error"                                            )\
    _TBAG_XX(_RESOURCE_EXHAUSTED                   , 10062, "Some resource has been exhausted"                        )\
    _TBAG_XX(_INACCESSIBLE_BLOCK                   , 10063, "Inaccessible block"                                      )\
    _TBAG_XX(_SCRIPT                               , 10064, "Unknown script error"                                    )\
    /* OpenSSL/ReSSL error code **************************************************************************************/\
    _TBAG_XX(_SSL                                  , 11000, "SSL library error"                                       )\
    _TBAG_XX(_SSL_NONE                             , 11001, "SSL I/O operation completed"                             )\
    _TBAG_XX(_SSLWREAD                             , 11002, "SSL want read"                                           )\
    _TBAG_XX(_SSLEREAD                             , 11003, "SSL empty read buffer"                                   )\
    _TBAG_XX(_SSLWWRITE                            , 11004, "SSL want write"                                          )\
    _TBAG_XX(_SSLEWRITE                            , 11005, "SSL empty write buffer"                                  )\
    _TBAG_XX(_SSLWX509                             , 11006, "SSL want x509 loopup"                                    )\
    /* OpenCL error code *********************************************************************************************/\
    _TBAG_XX(_OPENCL                               , 12000, "Unknown OpenCL error"                                    )\
    /* CUDA error code ***********************************************************************************************/\
    _TBAG_XX(_CUDA_INVALID_VALUE                   , 13001, "CUDA Error Invalid Value"                                )\
    _TBAG_XX(_CUDA_MEMORY_ALLOCATION               , 13002, "CUDA Error Memory Allocation"                            )\
    _TBAG_XX(_CUDA_INITIALIZATION_ERROR            , 13003, "CUDA Error Initialization Error"                         )\
    _TBAG_XX(_CUDA_CUDART_UNLOADING                , 13004, "CUDA Error Cudart Unloading"                             )\
    _TBAG_XX(_CUDA_PROFILER_DISABLED               , 13005, "CUDA Error Profiler Disabled"                            )\
    _TBAG_XX(_CUDA_PROFILER_NOT_INITIALIZED        , 13006, "CUDA Error Profiler Not Initialized"                     )\
    _TBAG_XX(_CUDA_PROFILER_ALREADY_STARTED        , 13007, "CUDA Error Profiler Already Started"                     )\
    _TBAG_XX(_CUDA_PROFILER_ALREADY_STOPPED        , 13008, "CUDA Error Profiler Already Stopped"                     )\
    _TBAG_XX(_CUDA_INVALID_CONFIGURATION           , 13009, "CUDA Error Invalid Configuration"                        )\
    _TBAG_XX(_CUDA_INVALID_PITCH_VALUE             , 13012, "CUDA Error Invalid Pitch Value"                          )\
    _TBAG_XX(_CUDA_INVALID_SYMBOL                  , 13013, "CUDA Error Invalid Symbol"                               )\
    _TBAG_XX(_CUDA_INVALID_HOST_POINTER            , 13016, "CUDA Error Invalid Host Pointer"                         )\
    _TBAG_XX(_CUDA_INVALID_DEVICE_POINTER          , 13017, "CUDA Error Invalid Device Pointer"                       )\
    _TBAG_XX(_CUDA_INVALID_TEXTURE                 , 13018, "CUDA Error Invalid Texture"                              )\
    _TBAG_XX(_CUDA_INVALID_TEXTURE_BINDING         , 13019, "CUDA Error Invalid Texture Binding"                      )\
    _TBAG_XX(_CUDA_INVALID_CHANNEL_DESCRIPTOR      , 13020, "CUDA Error Invalid Channel Descriptor"                   )\
    _TBAG_XX(_CUDA_INVALID_MEMCPY_DIRECTION        , 13021, "CUDA Error Invalid Memcpy Direction"                     )\
    _TBAG_XX(_CUDA_ADDRESS_OF_CONSTANT             , 13022, "CUDA Error Address Of Constant"                          )\
    _TBAG_XX(_CUDA_TEXTURE_FETCH_FAILED            , 13023, "CUDA Error Texture Fetch Failed"                         )\
    _TBAG_XX(_CUDA_TEXTURE_NOT_BOUND               , 13024, "CUDA Error Texture Not Bound"                            )\
    _TBAG_XX(_CUDA_SYNCHRONIZATION_ERROR           , 13025, "CUDA Error Synchronization Error"                        )\
    _TBAG_XX(_CUDA_INVALID_FILTER_SETTING          , 13026, "CUDA Error Invalid Filter Setting"                       )\
    _TBAG_XX(_CUDA_INVALID_NORM_SETTING            , 13027, "CUDA Error Invalid Norm Setting"                         )\
    _TBAG_XX(_CUDA_MIXED_DEVICE_EXECUTION          , 13028, "CUDA Error Mixed Device Execution"                       )\
    _TBAG_XX(_CUDA_NOT_YET_IMPLEMENTED             , 13031, "CUDA Error Not Yet Implemented"                          )\
    _TBAG_XX(_CUDA_MEMORY_VALUE_TOO_LARGE          , 13032, "CUDA Error Memory Value Too Large"                       )\
    _TBAG_XX(_CUDA_INSUFFICIENT_DRIVER             , 13035, "CUDA Error Insufficient Driver"                          )\
    _TBAG_XX(_CUDA_INVALID_SURFACE                 , 13037, "CUDA Error Invalid Surface"                              )\
    _TBAG_XX(_CUDA_DUPLICATE_VARIABLE_NAME         , 13043, "CUDA Error Duplicate Variable Name"                      )\
    _TBAG_XX(_CUDA_DUPLICATE_TEXTURE_NAME          , 13044, "CUDA Error Duplicate Texture Name"                       )\
    _TBAG_XX(_CUDA_DUPLICATE_SURFACE_NAME          , 13045, "CUDA Error Duplicate Surface Name"                       )\
    _TBAG_XX(_CUDA_DEVICES_UNAVAILABLE             , 13046, "CUDA Error Devices Unavailable"                          )\
    _TBAG_XX(_CUDA_INCOMPATIBLE_DRIVER_CONTEXT     , 13049, "CUDA Error Incompatible Driver Context"                  )\
    _TBAG_XX(_CUDA_MISSING_CONFIGURATION           , 13052, "CUDA Error Missing Configuration"                        )\
    _TBAG_XX(_CUDA_PRIOR_LAUNCH_FAILURE            , 13053, "CUDA Error Prior Launch Failure"                         )\
    _TBAG_XX(_CUDA_LAUNCH_MAX_DEPTH_EXCEEDED       , 13065, "CUDA Error Launch Max Depth Exceeded"                    )\
    _TBAG_XX(_CUDA_LAUNCH_FILE_SCOPED_TEX          , 13066, "CUDA Error Launch File Scoped Tex"                       )\
    _TBAG_XX(_CUDA_LAUNCH_FILE_SCOPED_SURF         , 13067, "CUDA Error Launch File Scoped Surf"                      )\
    _TBAG_XX(_CUDA_SYNC_DEPTH_EXCEEDED             , 13068, "CUDA Error Sync Depth Exceeded"                          )\
    _TBAG_XX(_CUDA_LAUNCH_PENDING_COUNT_EXCEEDED   , 13069, "CUDA Error Launch Pending Count Exceeded"                )\
    _TBAG_XX(_CUDA_INVALID_DEVICE_FUNCTION         , 13098, "CUDA Error Invalid Device Function"                      )\
    _TBAG_XX(_CUDA_NO_DEVICE                       , 13100, "CUDA Error No Device"                                    )\
    _TBAG_XX(_CUDA_INVALID_DEVICE                  , 13101, "CUDA Error Invalid Device"                               )\
    _TBAG_XX(_CUDA_STARTUP_FAILURE                 , 13127, "CUDA Error Startup Failure"                              )\
    _TBAG_XX(_CUDA_INVALID_KERNEL_IMAGE            , 13200, "CUDA Error Invalid Kernel Image"                         )\
    _TBAG_XX(_CUDA_DEVICE_UNINITILIALIZED          , 13201, "CUDA Error Device Uninitilialized"                       )\
    _TBAG_XX(_CUDA_MAP_BUFFER_OBJECT_FAILED        , 13205, "CUDA Error Map Buffer Object Failed"                     )\
    _TBAG_XX(_CUDA_UNMAP_BUFFER_OBJECT_FAILED      , 13206, "CUDA Error Unmap Buffer Object Failed"                   )\
    _TBAG_XX(_CUDA_ARRAY_IS_MAPPED                 , 13207, "CUDA Error Array Is Mapped"                              )\
    _TBAG_XX(_CUDA_ALREADY_MAPPED                  , 13208, "CUDA Error Already Mapped"                               )\
    _TBAG_XX(_CUDA_NO_KERNEL_IMAGE_FOR_DEVICE      , 13209, "CUDA Error No Kernel Image For Device"                   )\
    _TBAG_XX(_CUDA_ALREADY_ACQUIRED                , 13210, "CUDA Error Already Acquired"                             )\
    _TBAG_XX(_CUDA_NOT_MAPPED                      , 13211, "CUDA Error Not Mapped"                                   )\
    _TBAG_XX(_CUDA_NOT_MAPPED_AS_ARRAY             , 13212, "CUDA Error Not Mapped As Array"                          )\
    _TBAG_XX(_CUDA_NOT_MAPPED_AS_POINTER           , 13213, "CUDA Error Not Mapped As Pointer"                        )\
    _TBAG_XX(_CUDA_ECCUNCORRECTABLE                , 13214, "CUDA Error ECCUncorrectable"                             )\
    _TBAG_XX(_CUDA_UNSUPPORTED_LIMIT               , 13215, "CUDA Error Unsupported Limit"                            )\
    _TBAG_XX(_CUDA_DEVICE_ALREADY_IN_USE           , 13216, "CUDA Error Device Already In Use"                        )\
    _TBAG_XX(_CUDA_PEER_ACCESS_UNSUPPORTED         , 13217, "CUDA Error Peer Access Unsupported"                      )\
    _TBAG_XX(_CUDA_INVALID_PTX                     , 13218, "CUDA Error Invalid Ptx"                                  )\
    _TBAG_XX(_CUDA_INVALID_GRAPHICS_CONTEXT        , 13219, "CUDA Error Invalid Graphics Context"                     )\
    _TBAG_XX(_CUDA_NVLINK_UNCORRECTABLE            , 13220, "CUDA Error Nvlink Uncorrectable"                         )\
    _TBAG_XX(_CUDA_JIT_COMPILER_NOT_FOUND          , 13221, "CUDA Error Jit Compiler Not Found"                       )\
    _TBAG_XX(_CUDA_INVALID_SOURCE                  , 13300, "CUDA Error Invalid Source"                               )\
    _TBAG_XX(_CUDA_FILE_NOT_FOUND                  , 13301, "CUDA Error File Not Found"                               )\
    _TBAG_XX(_CUDA_SHARED_OBJECT_SYMBOL_NOT_FOUND  , 13302, "CUDA Error Shared Object Symbol Not Found"               )\
    _TBAG_XX(_CUDA_SHARED_OBJECT_INIT_FAILED       , 13303, "CUDA Error Shared Object Init Failed"                    )\
    _TBAG_XX(_CUDA_OPERATING_SYSTEM                , 13304, "CUDA Error Operating System"                             )\
    _TBAG_XX(_CUDA_INVALID_RESOURCE_HANDLE         , 13400, "CUDA Error Invalid Resource Handle"                      )\
    _TBAG_XX(_CUDA_ILLEGAL_STATE                   , 13401, "CUDA Error Illegal State"                                )\
    _TBAG_XX(_CUDA_SYMBOL_NOT_FOUND                , 13500, "CUDA Error Symbol Not Found"                             )\
    _TBAG_XX(_CUDA_NOT_READY                       , 13600, "CUDA Error Not Ready"                                    )\
    _TBAG_XX(_CUDA_ILLEGAL_ADDRESS                 , 13700, "CUDA Error Illegal Address"                              )\
    _TBAG_XX(_CUDA_LAUNCH_OUT_OF_RESOURCES         , 13701, "CUDA Error Launch Out Of Resources"                      )\
    _TBAG_XX(_CUDA_LAUNCH_TIMEOUT                  , 13702, "CUDA Error Launch Timeout"                               )\
    _TBAG_XX(_CUDA_LAUNCH_INCOMPATIBLE_TEXTURING   , 13703, "CUDA Error Launch Incompatible Texturing"                )\
    _TBAG_XX(_CUDA_PEER_ACCESS_ALREADY_ENABLED     , 13704, "CUDA Error Peer Access Already Enabled"                  )\
    _TBAG_XX(_CUDA_PEER_ACCESS_NOT_ENABLED         , 13705, "CUDA Error Peer Access Not Enabled"                      )\
    _TBAG_XX(_CUDA_SET_ON_ACTIVE_PROCESS           , 13708, "CUDA Error Set On Active Process"                        )\
    _TBAG_XX(_CUDA_CONTEXT_IS_DESTROYED            , 13709, "CUDA Error Context Is Destroyed"                         )\
    _TBAG_XX(_CUDA_ASSERT                          , 13710, "CUDA Error Assert"                                       )\
    _TBAG_XX(_CUDA_TOO_MANY_PEERS                  , 13711, "CUDA Error Too Many Peers"                               )\
    _TBAG_XX(_CUDA_HOST_MEMORY_ALREADY_REGISTERED  , 13712, "CUDA Error Host Memory Already Registered"               )\
    _TBAG_XX(_CUDA_HOST_MEMORY_NOT_REGISTERED      , 13713, "CUDA Error Host Memory Not Registered"                   )\
    _TBAG_XX(_CUDA_HARDWARE_STACK_ERROR            , 13714, "CUDA Error Hardware Stack Error"                         )\
    _TBAG_XX(_CUDA_ILLEGAL_INSTRUCTION             , 13715, "CUDA Error Illegal Instruction"                          )\
    _TBAG_XX(_CUDA_MISALIGNED_ADDRESS              , 13716, "CUDA Error Misaligned Address"                           )\
    _TBAG_XX(_CUDA_INVALID_ADDRESS_SPACE           , 13717, "CUDA Error Invalid Address Space"                        )\
    _TBAG_XX(_CUDA_INVALID_PC                      , 13718, "CUDA Error Invalid Pc"                                   )\
    _TBAG_XX(_CUDA_LAUNCH_FAILURE                  , 13719, "CUDA Error Launch Failure"                               )\
    _TBAG_XX(_CUDA_COOPERATIVE_LAUNCH_TOO_LARGE    , 13720, "CUDA Error Cooperative Launch Too Large"                 )\
    _TBAG_XX(_CUDA_NOT_PERMITTED                   , 13800, "CUDA Error Not Permitted"                                )\
    _TBAG_XX(_CUDA_NOT_SUPPORTED                   , 13801, "CUDA Error Not Supported"                                )\
    _TBAG_XX(_CUDA_SYSTEM_NOT_READY                , 13802, "CUDA Error System Not Ready"                             )\
    _TBAG_XX(_CUDA_SYSTEM_DRIVER_MISMATCH          , 13803, "CUDA Error System Driver Mismatch"                       )\
    _TBAG_XX(_CUDA_COMPAT_NOT_SUPPORTED_ON_DEVICE  , 13804, "CUDA Error Compat Not Supported On Device"               )\
    _TBAG_XX(_CUDA_STREAM_CAPTURE_UNSUPPORTED      , 13900, "CUDA Error Stream Capture Unsupported"                   )\
    _TBAG_XX(_CUDA_STREAM_CAPTURE_INVALIDATED      , 13901, "CUDA Error Stream Capture Invalidated"                   )\
    _TBAG_XX(_CUDA_STREAM_CAPTURE_MERGE            , 13902, "CUDA Error Stream Capture Merge"                         )\
    _TBAG_XX(_CUDA_STREAM_CAPTURE_UNMATCHED        , 13903, "CUDA Error Stream Capture Unmatched"                     )\
    _TBAG_XX(_CUDA_STREAM_CAPTURE_UNJOINED         , 13904, "CUDA Error Stream Capture Unjoined"                      )\
    _TBAG_XX(_CUDA_STREAM_CAPTURE_ISOLATION        , 13905, "CUDA Error Stream Capture Isolation"                     )\
    _TBAG_XX(_CUDA_STREAM_CAPTURE_IMPLICIT         , 13906, "CUDA Error Stream Capture Implicit"                      )\
    _TBAG_XX(_CUDA_CAPTURED_EVENT                  , 13907, "CUDA Error Captured Event"                               )\
    _TBAG_XX(_CUDA_STREAM_CAPTURE_WRONG_THREAD     , 13908, "CUDA Error Stream Capture Wrong Thread"                  )\
    _TBAG_XX(_CUDA_UNKNOWN                         , 13999, "CUDA Error Unknown"                                      )\
    /* LMDB error code ***********************************************************************************************/\
    _TBAG_XX(_LMDB                                 , 14000, "Unknown LMDB error"                                      )\
    /* ICU error code ************************************************************************************************/\
    _TBAG_XX(_ICU_CONV                             , 15000, "ICU Convert error"                                       )\
    /* HTTP Status Code (https://en.wikipedia.org/wiki/List_of_HTTP_status_codes) ************************************/\
    _TBAG_XX(_HTTP_100                             , 70100, "HTTP 100 Continue"                                       )\
    _TBAG_XX(_HTTP_101                             , 70101, "HTTP 101 Switching Protocols"                            )\
    _TBAG_XX(_HTTP_102                             , 70102, "HTTP 102 Processing (WebDAV; RFC 2518)"                  )\
    _TBAG_XX(_HTTP_103                             , 70103, "HTTP 103 Early Hints (RFC 8297)"                         )\
    _TBAG_XX(_HTTP_200                             , 70200, "HTTP 200 OK"                                             )\
    _TBAG_XX(_HTTP_201                             , 70201, "HTTP 201 Created"                                        )\
    _TBAG_XX(_HTTP_202                             , 70202, "HTTP 202 Accepted"                                       )\
    _TBAG_XX(_HTTP_203                             , 70203, "HTTP 203 Non-Authoritative Information (since HTTP/1.1)" )\
    _TBAG_XX(_HTTP_204                             , 70204, "HTTP 204 No Content"                                     )\
    _TBAG_XX(_HTTP_205                             , 70205, "HTTP 205 Reset Content"                                  )\
    _TBAG_XX(_HTTP_206                             , 70206, "HTTP 206 Partial Content (RFC 7233)"                     )\
    _TBAG_XX(_HTTP_207                             , 70207, "HTTP 207 Multi-Status (WebDAV; RFC 4918)"                )\
    _TBAG_XX(_HTTP_208                             , 70208, "HTTP 208 Already Reported (WebDAV; RFC 5842)"            )\
    _TBAG_XX(_HTTP_226                             , 70226, "HTTP 226 IM Used (RFC 3229)"                             )\
    _TBAG_XX(_HTTP_300                             , 70300, "HTTP 300 Multiple Choices"                               )\
    _TBAG_XX(_HTTP_301                             , 70301, "HTTP 301 Moved Permanently"                              )\
    _TBAG_XX(_HTTP_302                             , 70302, "HTTP 302 Found (Previously 'Moved temporarily')"         )\
    _TBAG_XX(_HTTP_303                             , 70303, "HTTP 303 See Other (since HTTP/1.1)"                     )\
    _TBAG_XX(_HTTP_304                             , 70304, "HTTP 304 Not Modified (RFC 7232)"                        )\
    _TBAG_XX(_HTTP_305                             , 70305, "HTTP 305 Use Proxy (since HTTP/1.1)"                     )\
    _TBAG_XX(_HTTP_306                             , 70306, "HTTP 306 Switch Proxy"                                   )\
    _TBAG_XX(_HTTP_307                             , 70307, "HTTP 307 Temporary Redirect (since HTTP/1.1)"            )\
    _TBAG_XX(_HTTP_308                             , 70308, "HTTP 308 Permanent Redirect (RFC 7538)"                  )\
    _TBAG_XX(_HTTP_400                             , 70400, "HTTP 400 Bad Request"                                    )\
    _TBAG_XX(_HTTP_401                             , 70401, "HTTP 401 Unauthorized (RFC 7235)"                        )\
    _TBAG_XX(_HTTP_402                             , 70402, "HTTP 402 Payment Required"                               )\
    _TBAG_XX(_HTTP_403                             , 70403, "HTTP 403 Forbidden"                                      )\
    _TBAG_XX(_HTTP_404                             , 70404, "HTTP 404 Not Found"                                      )\
    _TBAG_XX(_HTTP_405                             , 70405, "HTTP 405 Method Not Allowed"                             )\
    _TBAG_XX(_HTTP_406                             , 70406, "HTTP 406 Not Acceptable"                                 )\
    _TBAG_XX(_HTTP_407                             , 70407, "HTTP 407 Proxy Authentication Required (RFC 7235)"       )\
    _TBAG_XX(_HTTP_408                             , 70408, "HTTP 408 Request Timeout"                                )\
    _TBAG_XX(_HTTP_409                             , 70409, "HTTP 409 Conflict"                                       )\
    _TBAG_XX(_HTTP_410                             , 70410, "HTTP 410 Gone"                                           )\
    _TBAG_XX(_HTTP_411                             , 70411, "HTTP 411 Length Required"                                )\
    _TBAG_XX(_HTTP_412                             , 70412, "HTTP 412 Precondition Failed (RFC 7232)"                 )\
    _TBAG_XX(_HTTP_413                             , 70413, "HTTP 413 Payload Too Large (RFC 7231)"                   )\
    _TBAG_XX(_HTTP_414                             , 70414, "HTTP 414 URI Too Long (RFC 7231)"                        )\
    _TBAG_XX(_HTTP_415                             , 70415, "HTTP 415 Unsupported Media Type (RFC 7231)"              )\
    _TBAG_XX(_HTTP_416                             , 70416, "HTTP 416 Range Not Satisfiable (RFC 7233)"               )\
    _TBAG_XX(_HTTP_417                             , 70417, "HTTP 417 Expectation Failed"                             )\
    _TBAG_XX(_HTTP_418                             , 70418, "HTTP 418 I'm a teapot (RFC 2324, RFC 7168)"              )\
    _TBAG_XX(_HTTP_421                             , 70421, "HTTP 421 Misdirected Request (RFC 7540)"                 )\
    _TBAG_XX(_HTTP_422                             , 70422, "HTTP 422 Unprocessable Entity (WebDAV; RFC 4918)"        )\
    _TBAG_XX(_HTTP_423                             , 70423, "HTTP 423 Locked (WebDAV; RFC 4918)"                      )\
    _TBAG_XX(_HTTP_424                             , 70424, "HTTP 424 Failed Dependency (WebDAV; RFC 4918)"           )\
    _TBAG_XX(_HTTP_425                             , 70425, "HTTP 425 Too Early (RFC 8470)"                           )\
    _TBAG_XX(_HTTP_426                             , 70426, "HTTP 426 Upgrade Required"                               )\
    _TBAG_XX(_HTTP_428                             , 70428, "HTTP 428 Precondition Required (RFC 6585)"               )\
    _TBAG_XX(_HTTP_429                             , 70429, "HTTP 429 Too Many Requests (RFC 6585)"                   )\
    _TBAG_XX(_HTTP_431                             , 70431, "HTTP 431 Request Header Fields Too Large (RFC 6585)"     )\
    _TBAG_XX(_HTTP_451                             , 70451, "HTTP 451 Unavailable For Legal Reasons (RFC 7725)"       )\
    _TBAG_XX(_HTTP_500                             , 70500, "HTTP 500 Internal Server Error"                          )\
    _TBAG_XX(_HTTP_501                             , 70501, "HTTP 501 Not Implemented"                                )\
    _TBAG_XX(_HTTP_502                             , 70502, "HTTP 502 Bad Gateway"                                    )\
    _TBAG_XX(_HTTP_503                             , 70503, "HTTP 503 Service Unavailable"                            )\
    _TBAG_XX(_HTTP_504                             , 70504, "HTTP 504 Gateway Timeout"                                )\
    _TBAG_XX(_HTTP_505                             , 70505, "HTTP 505 HTTP Version Not Supported"                     )\
    _TBAG_XX(_HTTP_506                             , 70506, "HTTP 506 Variant Also Negotiates (RFC 2295)"             )\
    _TBAG_XX(_HTTP_507                             , 70507, "HTTP 507 Insufficient Storage (WebDAV; RFC 4918)"        )\
    _TBAG_XX(_HTTP_508                             , 70508, "HTTP 508 Loop Detected (WebDAV; RFC 5842)"               )\
    _TBAG_XX(_HTTP_510                             , 70510, "HTTP 510 Not Extended (RFC 2774)"                        )\
    _TBAG_XX(_HTTP_511                             , 70511, "HTTP 511 Network Authentication Required (RFC 6585)"     )\
    /* Missing system error in libuv *********************************************************************************/\
    _TBAG_XX(_EBADMSG                              , 80000, "Bad message"                                             )\
    _TBAG_XX(_ECHILD                               , 80001, "No child processes"                                      )\
    _TBAG_XX(_EDEADLK                              , 80002, "Resource deadlock would occur"                           )\
    _TBAG_XX(_EDOM                                 , 80003, "Mathematics argument out of domain of function"          )\
    _TBAG_XX(_EIDRM                                , 80004, "Identifier removed"                                      )\
    _TBAG_XX(_EILSEQ                               , 80005, "Illegal byte sequence"                                   )\
    _TBAG_XX(_EINPROGRESS                          , 80006, "Operation in progress"                                   )\
    _TBAG_XX(_ENETRESET                            , 80007, "Connection aborted by network"                           )\
    _TBAG_XX(_ENODATA                              , 80008, "No message is available on the STREAM head read queue"   )\
    _TBAG_XX(_ENOEXEC                              , 80009, "Executable file format error"                            )\
    _TBAG_XX(_ENOLCK                               , 80010, "No locks available"                                      )\
    _TBAG_XX(_ENOLINK                              , 80011, "Link has been severed"                                   )\
    _TBAG_XX(_ENOMSG                               , 80012, "No message of the desired type"                          )\
    _TBAG_XX(_ENOSR                                , 80013, "No STREAM resources"                                     )\
    _TBAG_XX(_ENOSTR                               , 80014, "Not a STREAM"                                            )\
    _TBAG_XX(_ENOTRECOVERABLE                      , 80015, "State not recoverable"                                   )\
    _TBAG_XX(_EOPNOTSUPP                           , 80016, "Operation not supported on socket"                       )\
    _TBAG_XX(_EOVERFLOW                            , 80017, "Value too large to be stored in data type"               )\
    _TBAG_XX(_EOWNERDEAD                           , 80018, "Previous owner died"                                     )\
    _TBAG_XX(_ETIME                                , 80019, "Stream ioctl() timeout"                                  )\
    _TBAG_XX(_EWOULDBLOCK                          , 80020, "Operation would block"                                   )\
    /* libuv/uvpp ****************************************************************************************************/\
    _TBAG_UV_XX(_E2BIG                             , 90000, "Argument list too long"                                  )\
    _TBAG_UV_XX(_EACCES                            , 90001, "Permission denied"                                       )\
    _TBAG_UV_XX(_EADDRINUSE                        , 90002, "Address already in use"                                  )\
    _TBAG_UV_XX(_EADDRNOTAVAIL                     , 90003, "Address not available"                                   )\
    _TBAG_UV_XX(_EAFNOSUPPORT                      , 90004, "Address family not supported"                            )\
    _TBAG_UV_XX(_EAGAIN                            , 90005, "Resource temporarily unavailable"                        )\
    _TBAG_UV_XX(_EAI_ADDRFAMILY                    , 90006, "Address family not supported"                            )\
    _TBAG_UV_XX(_EAI_AGAIN                         , 90007, "Temporary failure"                                       )\
    _TBAG_UV_XX(_EAI_BADFLAGS                      , 90008, "Bad ai_flags value"                                      )\
    _TBAG_UV_XX(_EAI_BADHINTS                      , 90009, "Invalid value for hints"                                 )\
    _TBAG_UV_XX(_EAI_CANCELED                      , 90010, "Request canceled"                                        )\
    _TBAG_UV_XX(_EAI_FAIL                          , 90011, "Permanent failure"                                       )\
    _TBAG_UV_XX(_EAI_FAMILY                        , 90012, "Ai_family not supported"                                 )\
    _TBAG_UV_XX(_EAI_MEMORY                        , 90013, "Out of memory"                                           )\
    _TBAG_UV_XX(_EAI_NODATA                        , 90014, "No address"                                              )\
    _TBAG_UV_XX(_EAI_NONAME                        , 90015, "Unknown node or service"                                 )\
    _TBAG_UV_XX(_EAI_OVERFLOW                      , 90016, "Argument buffer overflow"                                )\
    _TBAG_UV_XX(_EAI_PROTOCOL                      , 90017, "Resolved protocol is unknown"                            )\
    _TBAG_UV_XX(_EAI_SERVICE                       , 90018, "Service not available for socket type"                   )\
    _TBAG_UV_XX(_EAI_SOCKTYPE                      , 90019, "Socket type not supported"                               )\
    _TBAG_UV_XX(_EALREADY                          , 90020, "Connection already in progress"                          )\
    _TBAG_UV_XX(_EBADF                             , 90021, "Bad file descriptor"                                     )\
    _TBAG_UV_XX(_EBUSY                             , 90022, "Resource busy or locked"                                 )\
    _TBAG_UV_XX(_ECANCELED                         , 90023, "Operation canceled"                                      )\
    _TBAG_UV_XX(_ECHARSET                          , 90024, "Invalid Unicode character"                               )\
    _TBAG_UV_XX(_ECONNABORTED                      , 90025, "Software caused connection abort"                        )\
    _TBAG_UV_XX(_ECONNREFUSED                      , 90026, "Connection refused"                                      )\
    _TBAG_UV_XX(_ECONNRESET                        , 90027, "Connection reset by peer"                                )\
    _TBAG_UV_XX(_EDESTADDRREQ                      , 90028, "Destination address required"                            )\
    _TBAG_UV_XX(_EEXIST                            , 90029, "File already exists"                                     )\
    _TBAG_UV_XX(_EFAULT                            , 90030, "Bad address in system call argument"                     )\
    _TBAG_UV_XX(_EFBIG                             , 90031, "File too large"                                          )\
    _TBAG_UV_XX(_EHOSTUNREACH                      , 90032, "Host is unreachable"                                     )\
    _TBAG_UV_XX(_EINTR                             , 90033, "Interrupted system call"                                 )\
    _TBAG_UV_XX(_EINVAL                            , 90034, "Invalid argument"                                        )\
    _TBAG_UV_XX(_EIO                               , 90035, "I/O error"                                               )\
    _TBAG_UV_XX(_EISCONN                           , 90036, "Socket is already connected"                             )\
    _TBAG_UV_XX(_EISDIR                            , 90037, "Illegal operation on a directory"                        )\
    _TBAG_UV_XX(_ELOOP                             , 90038, "Too many symbolic links encountered"                     )\
    _TBAG_UV_XX(_EMFILE                            , 90039, "Too many open files"                                     )\
    _TBAG_UV_XX(_EMSGSIZE                          , 90040, "Message too long"                                        )\
    _TBAG_UV_XX(_ENAMETOOLONG                      , 90041, "Name too long"                                           )\
    _TBAG_UV_XX(_ENETDOWN                          , 90042, "Network is down"                                         )\
    _TBAG_UV_XX(_ENETUNREACH                       , 90043, "Network is unreachable"                                  )\
    _TBAG_UV_XX(_ENFILE                            , 90044, "File table overflow"                                     )\
    _TBAG_UV_XX(_ENOBUFS                           , 90045, "No buffer space available"                               )\
    _TBAG_UV_XX(_ENODEV                            , 90046, "No such device"                                          )\
    _TBAG_UV_XX(_ENOENT                            , 90047, "No such file or directory"                               )\
    _TBAG_UV_XX(_ENOMEM                            , 90048, "Not enough memory"                                       )\
    _TBAG_UV_XX(_ENONET                            , 90049, "Machine is not on the network"                           )\
    _TBAG_UV_XX(_ENOPROTOOPT                       , 90050, "Protocol not available"                                  )\
    _TBAG_UV_XX(_ENOSPC                            , 90051, "No space left on device"                                 )\
    _TBAG_UV_XX(_ENOSYS                            , 90052, "Function not implemented"                                )\
    _TBAG_UV_XX(_ENOTCONN                          , 90053, "Socket is not connected"                                 )\
    _TBAG_UV_XX(_ENOTDIR                           , 90054, "Not a directory"                                         )\
    _TBAG_UV_XX(_ENOTEMPTY                         , 90055, "Directory not empty"                                     )\
    _TBAG_UV_XX(_ENOTSOCK                          , 90056, "Socket operation on non-socket"                          )\
    _TBAG_UV_XX(_ENOTSUP                           , 90057, "Operation not supported on socket"                       )\
    _TBAG_UV_XX(_EPERM                             , 90058, "Operation not permitted"                                 )\
    _TBAG_UV_XX(_EPIPE                             , 90059, "Broken pipe"                                             )\
    _TBAG_UV_XX(_EPROTO                            , 90060, "Protocol error"                                          )\
    _TBAG_UV_XX(_EPROTONOSUPPORT                   , 90061, "Protocol not supported"                                  )\
    _TBAG_UV_XX(_EPROTOTYPE                        , 90062, "Protocol wrong type for socket"                          )\
    _TBAG_UV_XX(_ERANGE                            , 90063, "Result too large"                                        )\
    _TBAG_UV_XX(_EROFS                             , 90064, "Read-only file system"                                   )\
    _TBAG_UV_XX(_ESHUTDOWN                         , 90065, "Cannot send after transport endpoint shutdown"           )\
    _TBAG_UV_XX(_ESPIPE                            , 90066, "Invalid seek"                                            )\
    _TBAG_UV_XX(_ESRCH                             , 90067, "No such process"                                         )\
    _TBAG_UV_XX(_ETIMEDOUT                         , 90068, "Connection timed out"                                    )\
    _TBAG_UV_XX(_ETXTBSY                           , 90069, "Text file is busy"                                       )\
    _TBAG_UV_XX(_EXDEV                             , 90070, "Cross-device link not permitted"                         )\
    _TBAG_UV_XX(_UNKNOWN                           , 90071, "Unknown error"                                           )\
    _TBAG_UV_XX(_EOF                               , 90072, "End of file"                                             )\
    _TBAG_UV_XX(_ENXIO                             , 90073, "No such device or address"                               )\
    _TBAG_UV_XX(_EMLINK                            , 90074, "Too many links"                                          )\
    _TBAG_UV_XX(_EHOSTDOWN                         , 90075, "Host is down"                                            )\
    _TBAG_UV_XX(_EREMOTEIO                         , 90076, "Remote I/O error"                                        )\
    _TBAG_UV_XX(_ENOTTY                            , 90077, "Inappropriate ioctl for device"                          )\
    _TBAG_UV_XX(_EFTYPE                            , 90078, "Inappropriate file type or format"                       )\
    /* END ************************************************************************************************************/
#endif

/**
 * List of error code.
 *
 * @author zer0
 * @date 2016-12-14
 * @date 2017-05-18 (Rename: ErrorCode -> Err)
 */
enum class Err : int
{
    __START_NUMBER__ = -1,
#define _TBAG_XX(name, num, msg) E##name = num,
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
};

static_assert(static_cast<int>(Err::E_SUCCESS) == 0, "The success code must be 0.");

#define _TBAG_XX(name, num, msg) TBAG_CONSTEXPR Err const E##name = Err::E##name;
TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX

TBAG_CONSTEXPR Err const E_ASYNC_REQUEST          = E_ASYNCREQ;
TBAG_CONSTEXPR Err const E_ENQUEUE_AND_ASYNC      = E_ENQASYNC;
TBAG_CONSTEXPR Err const E_ILLEGAL_ARGUMENTS      = E_ILLARGS ;
TBAG_CONSTEXPR Err const E_ILLEGAL_STATE          = E_ILLSTATE;
TBAG_CONSTEXPR Err const E_OUT_OF_RANGE           = E_OORANGE ;
TBAG_CONSTEXPR Err const E_NOT_READY              = E_NREADY  ;
TBAG_CONSTEXPR Err const E_UNSUPPORTED_OPERATION  = E_ENOSYS  ;
TBAG_CONSTEXPR Err const E_READ_ERROR             = E_RDERR   ;
TBAG_CONSTEXPR Err const E_WRITE_ERROR            = E_WRERR   ;
TBAG_CONSTEXPR Err const E_SMALL_BUFFER           = E_SMALLBUF;
TBAG_CONSTEXPR Err const E_UNKNOWN_EXCEPTION      = E_UNKEXCP ;
TBAG_CONSTEXPR Err const E_UNKNOWN_SIGNAL         = E_UNKSIG  ;
TBAG_CONSTEXPR Err const E_UNKNOWN_PROCESS_ID     = E_EBADF   ;
TBAG_CONSTEXPR Err const E_LOCALE_CONVERTER_ERROR = E_ICU_CONV;

// NNG (nng_errno_enum)
TBAG_CONSTEXPR Err const E_NNG_EINTR        = E_EINTR        ;
TBAG_CONSTEXPR Err const E_NNG_ENOMEM       = E_ENOMEM       ;
TBAG_CONSTEXPR Err const E_NNG_EINVAL       = E_EINVAL       ;
TBAG_CONSTEXPR Err const E_NNG_EBUSY        = E_EBUSY        ;
TBAG_CONSTEXPR Err const E_NNG_ETIMEDOUT    = E_ETIMEDOUT    ;
TBAG_CONSTEXPR Err const E_NNG_ECONNREFUSED = E_ECONNREFUSED ;
TBAG_CONSTEXPR Err const E_NNG_ECLOSED      = E_CLOSED       ;
TBAG_CONSTEXPR Err const E_NNG_EAGAIN       = E_EAGAIN       ;
TBAG_CONSTEXPR Err const E_NNG_ENOTSUP      = E_ENOTSUP      ;
TBAG_CONSTEXPR Err const E_NNG_EADDRINUSE   = E_EADDRINUSE   ;
TBAG_CONSTEXPR Err const E_NNG_ESTATE       = E_ILLSTATE     ;
TBAG_CONSTEXPR Err const E_NNG_ENOENT       = E_ENOENT       ;
TBAG_CONSTEXPR Err const E_NNG_EPROTO       = E_EPROTO       ;
TBAG_CONSTEXPR Err const E_NNG_EUNREACHABLE = E_EHOSTUNREACH ;
TBAG_CONSTEXPR Err const E_NNG_EADDRINVAL   = E_EADDRNOTAVAIL;
TBAG_CONSTEXPR Err const E_NNG_EPERM        = E_EPERM        ;
TBAG_CONSTEXPR Err const E_NNG_EMSGSIZE     = E_EMSGSIZE     ;
TBAG_CONSTEXPR Err const E_NNG_ECONNABORTED = E_ECONNABORTED ;
TBAG_CONSTEXPR Err const E_NNG_ECONNRESET   = E_ECONNRESET   ;
TBAG_CONSTEXPR Err const E_NNG_ECANCELED    = E_ECANCELED    ;
TBAG_CONSTEXPR Err const E_NNG_ENOFILES     = E_EMFILE       ;
TBAG_CONSTEXPR Err const E_NNG_ENOSPC       = E_ENOSPC       ;
TBAG_CONSTEXPR Err const E_NNG_EEXIST       = E_EEXIST       ;
TBAG_CONSTEXPR Err const E_NNG_EREADONLY    = E_READONLY     ;
TBAG_CONSTEXPR Err const E_NNG_EWRITEONLY   = E_WRITEONLY    ;
TBAG_CONSTEXPR Err const E_NNG_ECRYPTO      = E_CRYPTO       ;
TBAG_CONSTEXPR Err const E_NNG_EPEERAUTH    = E_PEERAUTH     ;
TBAG_CONSTEXPR Err const E_NNG_ENOARG       = E_ILLARGS      ;
TBAG_CONSTEXPR Err const E_NNG_EAMBIGUOUS   = E_AMBIGUOUS    ;
TBAG_CONSTEXPR Err const E_NNG_EBADTYPE     = E_BADTYPE      ;
TBAG_CONSTEXPR Err const E_NNG_EINTERNAL    = E_INTERNAL     ;

inline int toInt(Err code) TBAG_NOEXCEPT { return static_cast<int>(code); }
inline Err toErr(int code) TBAG_NOEXCEPT { return static_cast<Err>(code); }

TBAG_API char const * getErrName(Err code) TBAG_NOEXCEPT;
TBAG_API char const * getErrDetail(Err code) TBAG_NOEXCEPT;

TBAG_API Err convertUvErrorToErr(int uv_error_code) TBAG_NOEXCEPT;
TBAG_API Err convertUvErrorToErrWithLogging(char const * prefix, int uv_error_code);

TBAG_API Err convertSystemErrorToErr(int system_error);
TBAG_API Err getGlobalSystemError();

/**
 * Err exception.
 *
 * @author zer0
 * @date   2018-04-07
 */
struct ErrException : public std::exception
{
    Err const CODE;

    ErrException(Err c) : CODE(c) { /* EMPTY. */ }
    virtual ~ErrException() { /* EMPTY. */ }

    virtual const char * what() const TBAG_NOEXCEPT
    { return getErrName(CODE); }
};

// ----------------
// libuv debugging.
// ----------------

/**
 * @remarks
 *  Same this code:
 *  @code
 *    const char* uv_strerror(int err);
 *  @endcode
 */
TBAG_API char const * getUvErrorDetail(int uv_error_code);

/**
 * @remarks
 *  Same this code:
 *  @code
 *    const char* uv_err_name(int err);
 *  @endcode
 */
TBAG_API char const * getUvErrorName(int uv_error_code);

// --------------
// Output Stream.
// --------------

template <class CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, Err const & err)
{
    return os << libtbag::getErrName(err);
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

inline bool isSuccess(libtbag::Err err) TBAG_NOEXCEPT
{
    return err == libtbag::E_SUCCESS;
}

inline bool isFailure(libtbag::Err err) TBAG_NOEXCEPT
{
    return !isSuccess(err);
}

inline bool isSuccessAnyway(libtbag::Err err) TBAG_NOEXCEPT
{
    // clang-format off
    switch (err) {
    case libtbag::E_SUCCESS:  return true;
    case libtbag::E_WARNING:  return true;
    case libtbag::E_RETRY:    return true;
    case libtbag::E_CONTINUE: return true;
    case libtbag::E_ASYNCREQ: return true;
    case libtbag::E_ENQASYNC: return true;
    default:                  return false;
    }
    // clang-format on
}

inline bool isFailureAnyway(libtbag::Err err) TBAG_NOEXCEPT
{
    return !isSuccessAnyway(err);
}

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ERR_HPP__

