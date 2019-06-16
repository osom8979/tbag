#ifndef CIVETWEB_HEADER_INCLUDED
#define CIVETWEB_HEADER_INCLUDED

#define CIVETWEB_VERSION "1.12"
#define CIVETWEB_VERSION_MAJOR (1)
#define CIVETWEB_VERSION_MINOR (12)
#define CIVETWEB_VERSION_PATCH (0)

#include <stddef.h>
#include <stdio.h>

enum {
    MG_FEATURES_DEFAULT = 0x0u,
    MG_FEATURES_FILES = 0x1u,
    MG_FEATURES_TLS = 0x2u,
    MG_FEATURES_SSL = 0x2u,
    MG_FEATURES_CGI = 0x4u,
    MG_FEATURES_IPV6 = 0x8u,
    MG_FEATURES_WEBSOCKET = 0x10u,
    MG_FEATURES_LUA = 0x20u,
    MG_FEATURES_SSJS = 0x40u,
    MG_FEATURES_CACHE = 0x80u,
    MG_FEATURES_STATS = 0x100u,
    MG_FEATURES_COMPRESSION = 0x200u,
    MG_FEATURES_ALL = 0xFFFFu
};

struct mg_context;
struct mg_connection;

#define MG_MAX_HEADERS (64)

struct mg_header
{
    char const * name;
    char const * value;
};

struct mg_request_info
{
    char const * request_method;
    char const * request_uri;
    char const * local_uri;
#if defined(MG_LEGACY_INTERFACE)
    char const * uri;
#endif
    char const * http_version;
    char const * query_string;
    char const * remote_user;
    char remote_addr[48];
    long long content_length;
    int remote_port;
    int is_ssl;
    void * user_data;
    void * conn_data;
    int num_headers;
    struct mg_header http_headers[MG_MAX_HEADERS];
    struct mg_client_cert * client_cert;
    char const * acceptedWebSocketSubprotocol;
};

struct mg_response_info
{
    int status_code;
    char const * status_text;
    char const * http_version;
    long long content_length;
    int num_headers;
    struct mg_header http_headers[MG_MAX_HEADERS];
};

struct mg_client_cert
{
    void * peer_cert;
    char const * subject;
    char const * issuer;
    char const * serial;
    char const * finger;
};

#if defined(MG_LEGACY_INTERFACE)
struct client_cert
{
    char const * subject;
    char const * issuer;
    char const * serial;
    char const * finger;
};
#endif

struct mg_callbacks
{
    int (*begin_request)(struct mg_connection *);
    void (*end_request)(struct mg_connection const *, int reply_status_code);
    int (*log_message)(struct mg_connection const *, char const * message);
    int (*log_access)(struct mg_connection const *, char const * message);
    int (*init_ssl)(void * ssl_context, void * user_data);
    int (*external_ssl_ctx)(void ** ssl_ctx, void * user_data);
#if defined(MG_LEGACY_INTERFACE)
    int (*websocket_connect)(struct mg_connection const *);
    void (*websocket_ready)(struct mg_connection *);
    int (*websocket_data)(struct mg_connection *, int bits, char * data, size_t data_len);
#endif
    void (*connection_close)(struct mg_connection const *);
    void (*init_lua)(struct mg_connection const * conn, void * lua_context);
#if defined(MG_LEGACY_INTERFACE)
    void (*upload)(struct mg_connection *, char const * file_name);
#endif
    int (*http_error)(struct mg_connection * conn, int status, char const * errmsg);
    void (*init_context)(struct mg_context const * ctx);
    void (*exit_context)(struct mg_context const * ctx);
    void *(*init_thread)(struct mg_context const * ctx, int thread_type);
    void (*exit_thread)(struct mg_context const * ctx, int thread_type, void * thread_pointer);
    int (*init_connection)(struct mg_connection const * conn, void ** conn_data);
};

typedef int (*mg_request_handler)(struct mg_connection * conn, void * cbdata);

typedef int (*mg_websocket_connect_handler)(struct mg_connection const *, void *);
typedef void (*mg_websocket_ready_handler)(struct mg_connection *, void *);
typedef int (*mg_websocket_data_handler)(struct mg_connection *, int, char *, size_t, void *);
typedef void (*mg_websocket_close_handler)(struct mg_connection const *, void *);

struct mg_websocket_subprotocols
{
    int nb_subprotocols;
    char ** subprotocols;
};

typedef int (*mg_authorization_handler)(struct mg_connection * conn, void * cbdata);

struct mg_option
{
    char const * name;
    int type;
    char const * default_value;
};

#if defined(MG_LEGACY_INTERFACE)
enum {
    CONFIG_TYPE_UNKNOWN = 0x0,
    CONFIG_TYPE_NUMBER = 0x1,
    CONFIG_TYPE_STRING = 0x2,
    CONFIG_TYPE_FILE = 0x3,
    CONFIG_TYPE_DIRECTORY = 0x4,
    CONFIG_TYPE_BOOLEAN = 0x5,
    CONFIG_TYPE_EXT_PATTERN = 0x6,
    CONFIG_TYPE_STRING_LIST = 0x7,
    CONFIG_TYPE_STRING_MULTILINE = 0x8
};
#endif

enum {
    MG_CONFIG_TYPE_UNKNOWN = 0x0,
    MG_CONFIG_TYPE_NUMBER = 0x1,
    MG_CONFIG_TYPE_STRING = 0x2,
    MG_CONFIG_TYPE_FILE = 0x3,
    MG_CONFIG_TYPE_DIRECTORY = 0x4,
    MG_CONFIG_TYPE_BOOLEAN = 0x5,
    MG_CONFIG_TYPE_EXT_PATTERN = 0x6,
    MG_CONFIG_TYPE_STRING_LIST = 0x7,
    MG_CONFIG_TYPE_STRING_MULTILINE = 0x8,
    MG_CONFIG_TYPE_YES_NO_OPTIONAL = 0x9
};

struct mg_server_port
{
    int protocol;
    int port;
    int is_ssl;
    int is_redirect;
    int _reserved1;
    int _reserved2;
    int _reserved3;
    int _reserved4;
};

#define mg_server_ports mg_server_port

#if defined(MG_LEGACY_INTERFACE)
#define mg_lock mg_lock_connection
#define mg_unlock mg_unlock_connection
#endif

#if defined(MG_LEGACY_INTERFACE)
enum {
    WEBSOCKET_OPCODE_CONTINUATION = 0x0,
    WEBSOCKET_OPCODE_TEXT = 0x1,
    WEBSOCKET_OPCODE_BINARY = 0x2,
    WEBSOCKET_OPCODE_CONNECTION_CLOSE = 0x8,
    WEBSOCKET_OPCODE_PING = 0x9,
    WEBSOCKET_OPCODE_PONG = 0xa
};
#endif

enum {
    MG_WEBSOCKET_OPCODE_CONTINUATION = 0x0,
    MG_WEBSOCKET_OPCODE_TEXT = 0x1,
    MG_WEBSOCKET_OPCODE_BINARY = 0x2,
    MG_WEBSOCKET_OPCODE_CONNECTION_CLOSE = 0x8,
    MG_WEBSOCKET_OPCODE_PING = 0x9,
    MG_WEBSOCKET_OPCODE_PONG = 0xa
};

#undef PRINTF_FORMAT_STRING
#if defined(_MSC_VER) && _MSC_VER >= 1400
# include <sal.h>
# if defined(_MSC_VER) && _MSC_VER > 1400
#  define PRINTF_FORMAT_STRING(s) _Printf_format_string_ s
# else
#  define PRINTF_FORMAT_STRING(s) __format_string s
# endif
#else
# define PRINTF_FORMAT_STRING(s) s
#endif

#ifdef __GNUC__
#define PRINTF_ARGS(x, y) __attribute__((format(printf, x, y)))
#else
#define PRINTF_ARGS(x, y)
#endif

struct mg_form_data_handler
{
    int (*field_found)(char const * key, char const * filename, char * path, size_t pathlen, void * user_data);
    int (*field_get)(char const * key, char const * value, size_t valuelen, void * user_data);
    int (*field_store)(char const * path, long long file_size, void * user_data);
    void * user_data;
};

#if defined(MG_LEGACY_INTERFACE)
enum {
    FORM_FIELD_STORAGE_SKIP = 0x0,
    FORM_FIELD_STORAGE_GET = 0x1,
    FORM_FIELD_STORAGE_STORE = 0x2,
    FORM_FIELD_STORAGE_ABORT = 0x10
};
#endif

enum {
    MG_FORM_FIELD_STORAGE_SKIP = 0x0,
    MG_FORM_FIELD_STORAGE_GET = 0x1,
    MG_FORM_FIELD_STORAGE_STORE = 0x2,
    MG_FORM_FIELD_STORAGE_ABORT = 0x10
};

enum {
    MG_FORM_FIELD_HANDLE_GET = 0x1,
    MG_FORM_FIELD_HANDLE_NEXT = 0x8,
    MG_FORM_FIELD_HANDLE_ABORT = 0x10
};

typedef void *(*mg_thread_func_t)(void *);

struct mg_client_options
{
    char const * host;
    int port;
    char const * client_cert;
    char const * server_cert;
    char const * host_name;
};

enum { TIMEOUT_INFINITE = -1 };
enum { MG_TIMEOUT_INFINITE = -1 };

#endif
