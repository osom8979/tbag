/**
 * @file   HttpServer.hpp
 * @brief  HttpServer class prototype.
 * @author zer0
 * @date   2020-03-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/bitwise/BitFlags.hpp>
#include <libtbag/http/CivetWebBypass.hpp>
#include <libtbag/http/CivetWebServer.hpp>
#include <libtbag/http/CivetWebEventHandler.hpp>
#include <libtbag/http/CivetWebSocketEventHandler.hpp>

#include <string>
#include <vector>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpServer class prototype.
 *
 * @author zer0
 * @date   2020-03-10
 */
class TBAG_API HttpServer : private Noncopyable
{
private:
    static void __close_cb(CivetServer * s);

public:
    template <typename T>
    using EventHandler = libtbag::http::CivetWebEventHandler<T>;
    using EventFunctional = libtbag::http::CivetWebEventFunctional;

public:
    using Options = CivetServer::Options;
    using UniqueCivetServer = std::unique_ptr<CivetServer, decltype(&HttpServer::__close_cb)>;
    using SharedCivetHandler = std::shared_ptr<CivetHandler>;
    using RequestMap = std::unordered_map<std::string, SharedCivetHandler>;

public:
    TBAG_CONSTEXPR static char const * const OPT_LISTENING_PORTS                   = "listening_ports";
    TBAG_CONSTEXPR static char const * const OPT_NUM_THREADS                       = "num_threads";
    TBAG_CONSTEXPR static char const * const OPT_RUN_AS_USER                       = "run_as_user";
    TBAG_CONSTEXPR static char const * const OPT_TCP_NODELAY                       = "tcp_nodelay";
    TBAG_CONSTEXPR static char const * const OPT_MAX_REQUEST_SIZE                  = "max_request_size";
    TBAG_CONSTEXPR static char const * const OPT_LINGER_TIMEOUT_MS                 = "linger_timeout_ms";
    TBAG_CONSTEXPR static char const * const OPT_ALLOW_SENDFILE_CALL               = "allow_sendfile_call";
    TBAG_CONSTEXPR static char const * const OPT_CASE_SENSITIVE                    = "case_sensitive";
    TBAG_CONSTEXPR static char const * const OPT_THROTTLE                          = "throttle";
    TBAG_CONSTEXPR static char const * const OPT_ACCESS_LOG_FILE                   = "access_log_file";
    TBAG_CONSTEXPR static char const * const OPT_ERROR_LOG_FILE                    = "error_log_file";
    TBAG_CONSTEXPR static char const * const OPT_ENABLE_KEEP_ALIVE                 = "enable_keep_alive";
    TBAG_CONSTEXPR static char const * const OPT_REQUEST_TIMEOUT_MS                = "request_timeout_ms";
    TBAG_CONSTEXPR static char const * const OPT_KEEP_ALIVE_TIMEOUT_MS             = "keep_alive_timeout_ms";
    TBAG_CONSTEXPR static char const * const OPT_WEBSOCKET_TIMEOUT_MS              = "websocket_timeout_ms";
    TBAG_CONSTEXPR static char const * const OPT_ENABLE_WEBSOCKET_PING_PONG        = "enable_websocket_ping_pong";
    TBAG_CONSTEXPR static char const * const OPT_DECODE_URL                        = "decode_url";
    TBAG_CONSTEXPR static char const * const OPT_LUA_BACKGROUND_SCRIPT             = "lua_background_script";
    TBAG_CONSTEXPR static char const * const OPT_LUA_BACKGROUND_SCRIPT_PARAMS      = "lua_background_script_params";
    TBAG_CONSTEXPR static char const * const OPT_CGI_TIMEOUT_MS                    = "cgi_timeout_ms";
    TBAG_CONSTEXPR static char const * const OPT_DOCUMENT_ROOT                     = "document_root";
    TBAG_CONSTEXPR static char const * const OPT_CGI_PATTERN                       = "cgi_pattern";
    TBAG_CONSTEXPR static char const * const OPT_CGI_ENVIRONMENT                   = "cgi_environment";
    TBAG_CONSTEXPR static char const * const OPT_PUT_DELETE_AUTH_FILE              = "put_delete_auth_file";
    TBAG_CONSTEXPR static char const * const OPT_CGI_INTERPRETER                   = "cgi_interpreter";
    TBAG_CONSTEXPR static char const * const OPT_PROTECT_URI                       = "protect_uri";
    TBAG_CONSTEXPR static char const * const OPT_AUTHENTICATION_DOMAIN             = "authentication_domain";
    TBAG_CONSTEXPR static char const * const OPT_ENABLE_AUTH_DOMAIN_CHECK          = "enable_auth_domain_check";
    TBAG_CONSTEXPR static char const * const OPT_SSI_PATTERN                       = "ssi_pattern";
    TBAG_CONSTEXPR static char const * const OPT_ENABLE_DIRECTORY_LISTING          = "enable_directory_listing";
    TBAG_CONSTEXPR static char const * const OPT_GLOBAL_AUTH_FILE                  = "global_auth_file";
    TBAG_CONSTEXPR static char const * const OPT_INDEX_FILES                       = "index_files";
    TBAG_CONSTEXPR static char const * const OPT_ACCESS_CONTROL_LIST               = "access_control_list";
    TBAG_CONSTEXPR static char const * const OPT_EXTRA_MIME_TYPES                  = "extra_mime_types";
    TBAG_CONSTEXPR static char const * const OPT_SSL_CERTIFICATE                   = "ssl_certificate";
    TBAG_CONSTEXPR static char const * const OPT_SSL_CERTIFICATE_CHAIN             = "ssl_certificate_chain";
    TBAG_CONSTEXPR static char const * const OPT_URL_REWRITE_PATTERNS              = "url_rewrite_patterns";
    TBAG_CONSTEXPR static char const * const OPT_HIDE_FILES_PATTERNS               = "hide_files_patterns";
    TBAG_CONSTEXPR static char const * const OPT_SSL_VERIFY_PEER                   = "ssl_verify_peer";
    TBAG_CONSTEXPR static char const * const OPT_SSL_CA_PATH                       = "ssl_ca_path";
    TBAG_CONSTEXPR static char const * const OPT_SSL_CA_FILE                       = "ssl_ca_file";
    TBAG_CONSTEXPR static char const * const OPT_SSL_VERIFY_DEPTH                  = "ssl_verify_depth";
    TBAG_CONSTEXPR static char const * const OPT_SSL_DEFAULT_VERIFY_PATHS          = "ssl_default_verify_paths";
    TBAG_CONSTEXPR static char const * const OPT_SSL_CIPHER_LIST                   = "ssl_cipher_list";
    TBAG_CONSTEXPR static char const * const OPT_SSL_PROTOCOL_VERSION              = "ssl_protocol_version";
    TBAG_CONSTEXPR static char const * const OPT_SSL_SHORT_TRUST                   = "ssl_short_trust";
    TBAG_CONSTEXPR static char const * const OPT_LUA_PRELOAD_FILE                  = "lua_preload_file";
    TBAG_CONSTEXPR static char const * const OPT_LUA_SCRIPT_PATTERN                = "lua_script_pattern";
    TBAG_CONSTEXPR static char const * const OPT_LUA_SERVER_PAGE_PATTERN           = "lua_server_page_pattern";
    TBAG_CONSTEXPR static char const * const OPT_LUA_DEBUG                         = "lua_debug";
    TBAG_CONSTEXPR static char const * const OPT_DUKTAPE_SCRIPT_PATTERN            = "duktape_script_pattern";
    TBAG_CONSTEXPR static char const * const OPT_WEBSOCKET_ROOT                    = "websocket_root";
    TBAG_CONSTEXPR static char const * const OPT_LUA_WEBSOCKET_PATTERN             = "lua_websocket_pattern";
    TBAG_CONSTEXPR static char const * const OPT_ACCESS_CONTROL_ALLOW_ORIGIN       = "access_control_allow_origin";
    TBAG_CONSTEXPR static char const * const OPT_ACCESS_CONTROL_ALLOW_METHODS      = "access_control_allow_methods";
    TBAG_CONSTEXPR static char const * const OPT_ACCESS_CONTROL_ALLOW_HEADERS      = "access_control_allow_headers";
    TBAG_CONSTEXPR static char const * const OPT_ERROR_PAGES                       = "error_pages";
    TBAG_CONSTEXPR static char const * const OPT_STATIC_FILE_MAX_AGE               = "static_file_max_age";
    TBAG_CONSTEXPR static char const * const OPT_STRICT_TRANSPORT_SECURITY_MAX_AGE = "strict_transport_security_max_age";
    TBAG_CONSTEXPR static char const * const OPT_ADDITIONAL_HEADER                 = "additional_header";
    TBAG_CONSTEXPR static char const * const OPT_ALLOW_INDEX_SCRIPT_RESOURCE       = "allow_index_script_resource";
    TBAG_CONSTEXPR static char const * const OPT_DAEMONIZE                         = "daemonize";

public:
    CivetCallbacks CALLBACKS;

private:
    UniqueCivetServer _server;
    RequestMap _requests;

public:
    HttpServer();
    virtual ~HttpServer();

public:
    static void opt(Options & o, char const * key, bool arg);
    static void opt(Options & o, char const * key, int arg);
    static void opt(Options & o, char const * key, std::string const & arg);
    static void opt(Options & o, char const * key, std::vector<std::string> const & args);

public:
    TBAG_CONSTEXPR static unsigned const METHOD_FLAG_GET     = 0x01;
    TBAG_CONSTEXPR static unsigned const METHOD_FLAG_POST    = 0x02;
    TBAG_CONSTEXPR static unsigned const METHOD_FLAG_HEAD    = 0x04;
    TBAG_CONSTEXPR static unsigned const METHOD_FLAG_PUT     = 0x08;
    TBAG_CONSTEXPR static unsigned const METHOD_FLAG_DELETE  = 0x10;
    TBAG_CONSTEXPR static unsigned const METHOD_FLAG_OPTIONS = 0x20;
    TBAG_CONSTEXPR static unsigned const METHOD_FLAG_PATCH   = 0x40;

    template <typename BaseT,
              typename HandlerT = CivetWebEventHandler<BaseT> >
    static bool req(RequestMap & r, std::string const & path, BaseT * base,
                    typename CivetWebEventHandler<BaseT>::Callback f,
                    unsigned method_flags = METHOD_FLAG_GET|METHOD_FLAG_POST)
    {
        std::shared_ptr<HandlerT> handler;
        try {
            handler = std::make_shared<HandlerT>(base);
        } catch (...) {
            return false;
        }

        // clang-format off
        if (TBAG_CHECK_BIT_FLAG(method_flags, METHOD_FLAG_GET    )) { handler->get_cb     = f; }
        if (TBAG_CHECK_BIT_FLAG(method_flags, METHOD_FLAG_POST   )) { handler->post_cb    = f; }
        if (TBAG_CHECK_BIT_FLAG(method_flags, METHOD_FLAG_HEAD   )) { handler->head_cb    = f; }
        if (TBAG_CHECK_BIT_FLAG(method_flags, METHOD_FLAG_PUT    )) { handler->put_cb     = f; }
        if (TBAG_CHECK_BIT_FLAG(method_flags, METHOD_FLAG_DELETE )) { handler->delete_cb  = f; }
        if (TBAG_CHECK_BIT_FLAG(method_flags, METHOD_FLAG_OPTIONS)) { handler->options_cb = f; }
        if (TBAG_CHECK_BIT_FLAG(method_flags, METHOD_FLAG_PATCH  )) { handler->patch_cb   = f; }
        // clang-format on

        return r.emplace(path, std::static_pointer_cast<CivetHandler>(handler)).second;
    }

public:
    static bool req(RequestMap & r, std::string const & path, CivetWebEventFunctional const & f);

public:
    ErrMsg open(Options const & options, RequestMap const & requests, void * user = nullptr);
    void close();

public:
    bool isOpen() const;

public:
    static Err write(mg_connection * conn, std::string const & body);
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__

