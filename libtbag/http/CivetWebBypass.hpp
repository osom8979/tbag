/**
 * @file   CivetWebBypass.hpp
 * @brief  CivetWebBypass class prototype.
 * @author zer0
 * @date   2019-06-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBBYPASS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBBYPASS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/http/civet-1.12/civetweb.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

TBAG_CONSTEXPR int const NOT_IMPLEMENTED = -9999;

TBAG_API unsigned mg_init_library(unsigned features);
TBAG_API unsigned mg_exit_library();

TBAG_API mg_context * mg_start(mg_callbacks const * callbacks,
                               void * user_data,
                               const char ** configuration_options);
TBAG_API void mg_stop(mg_context * ctx);
TBAG_API int mg_start_domain(mg_context * ctx,
                             const char ** configuration_options);

TBAG_API void mg_set_request_handler(mg_context * ctx,
                                     char const * uri,
                                     mg_request_handler handler,
                                     void * cbdata);
TBAG_API void mg_set_websocket_handler(mg_context * ctx,
                                       char const * uri,
                                       mg_websocket_connect_handler connect_handler,
                                       mg_websocket_ready_handler ready_handler,
                                       mg_websocket_data_handler data_handler,
                                       mg_websocket_close_handler close_handler,
                                       void * cbdata);
TBAG_API void mg_set_websocket_handler_with_subprotocols(mg_context * ctx,
                                                         char const * uri,
                                                         mg_websocket_subprotocols * subprotocols,
                                                         mg_websocket_connect_handler connect_handler,
                                                         mg_websocket_ready_handler ready_handler,
                                                         mg_websocket_data_handler data_handler,
                                                         mg_websocket_close_handler close_handler,
                                                         void * cbdata);
TBAG_API void mg_set_auth_handler(mg_context * ctx,
                                  char const * uri,
                                  mg_authorization_handler handler,
                                  void * cbdata);

TBAG_API char const * mg_get_option(mg_context const * ctx, char const * name);
TBAG_API mg_context * mg_get_context(mg_connection const * conn);
TBAG_API void * mg_get_user_data(mg_context const * ctx);
TBAG_API void * mg_get_thread_pointer(mg_connection const * conn);
TBAG_API void mg_set_user_connection_data(mg_connection * conn, void * data);
TBAG_API void * mg_get_user_connection_data(mg_connection const * conn);
TBAG_API int mg_get_request_link(mg_connection const * conn, char * buf, size_t buflen);
TBAG_API mg_option const * mg_get_valid_options();
TBAG_API int mg_get_server_ports(mg_context const * ctx, int size, mg_server_port * ports);

TBAG_API int mg_modify_passwords_file(char const * passwords_file_name,
                                      char const * realm,
                                      char const * user,
                                      char const * password);

TBAG_API mg_request_info const * mg_get_request_info(mg_connection const * conn);
TBAG_API mg_response_info const * mg_get_response_info(mg_connection const * conn);

TBAG_API int mg_write(mg_connection * conn, void const * buf, size_t len);
TBAG_API int mg_websocket_write(mg_connection * conn, int opcode, char const * data, size_t data_len);
TBAG_API int mg_websocket_client_write(mg_connection * conn, int opcode, char const * data, size_t data_len);

TBAG_API void mg_lock_connection(mg_connection * conn);
TBAG_API void mg_unlock_connection(mg_connection * conn);

TBAG_API void mg_lock_context(mg_context * ctx);
TBAG_API void mg_unlock_context(mg_context * ctx);

TBAG_API int mg_send_chunk(mg_connection * conn, char const * chunk, unsigned int chunk_len);
TBAG_API void mg_send_file(mg_connection * conn, char const * path);
TBAG_API int mg_send_file_body(mg_connection * conn, char const * path);
TBAG_API int mg_send_http_error(mg_connection * conn, int status_code, char const * msg, int msglen);
TBAG_API int mg_send_http_ok(mg_connection * conn, char const * mime_type, long long content_length);
TBAG_API int mg_send_http_redirect(mg_connection * conn, char const * target_url, int redirect_code);

TBAG_API int mg_send_digest_access_authentication_request(mg_connection * conn, char const * realm);
TBAG_API int mg_check_digest_access_authentication(mg_connection * conn, char const * realm, char const * filename);

TBAG_API void mg_send_mime_file(mg_connection * conn, char const * path, char const * mime_type);
TBAG_API void mg_send_mime_file2(mg_connection * conn, char const * path, char const * mime_type,
                                 char const * additional_headers);

TBAG_API long long mg_store_body(mg_connection * conn, char const * path);
TBAG_API int mg_read(mg_connection * conn, void * buf, size_t len);
TBAG_API char const * mg_get_header(mg_connection const * conn, char const * name);

TBAG_API int mg_get_var(char const * data, size_t data_len, char const * var_name,
                        char * dst, size_t dst_len);
TBAG_API int mg_get_var2(char const * data, size_t data_len, char const * var_name,
                         char * dst, size_t dst_len, size_t occurrence);
TBAG_API int mg_get_cookie(char const * cookie, char const * var_name, char * buf, size_t buf_len);

TBAG_API mg_connection * mg_download(char const * host, int port, int use_ssl,
                                     char * error_buffer, size_t error_buffer_size,
                                     char const * request, int reqlen);

TBAG_API void mg_close_connection(mg_connection * conn);

TBAG_API int mg_handle_form_request(mg_connection * conn, mg_form_data_handler * fdh);

TBAG_API int mg_start_thread(mg_thread_func_t f, void * p);

TBAG_API char const * mg_get_builtin_mime_type(char const * file_name);
TBAG_API const char * mg_get_response_code_text(mg_connection const * conn, int response_code);
TBAG_API char const * mg_version();
TBAG_API int mg_url_decode(char const * src, int src_len, char * dst, int dst_len, int is_form_url_encoded);
TBAG_API int mg_url_encode(char const * src, char * dst, size_t dst_len);
TBAG_API char * mg_md5(char buf[33], char const * msg, int msglen);
TBAG_API void mg_cry(mg_connection const * conn, char const * msg, int msglen);
TBAG_API int mg_strcasecmp(char const * s1, char const * s2);
TBAG_API int mg_strncasecmp(char const * s1, char const * s2, size_t len);

TBAG_API mg_connection * mg_connect_websocket_client(char const * host,
                                                     int port,
                                                     int use_ssl,
                                                     char * error_buffer,
                                                     size_t error_buffer_size,
                                                     char const * path,
                                                     char const * origin,
                                                     mg_websocket_data_handler data_func,
                                                     mg_websocket_close_handler close_func,
                                                     void * user_data);
TBAG_API mg_connection * mg_connect_client(char const * host,
                                           int port,
                                           int use_ssl,
                                           char * error_buffer,
                                           size_t error_buffer_size);
TBAG_API mg_connection * mg_connect_client_secure(mg_client_options const * client_options,
                                                  char * error_buffer,
                                                  size_t error_buffer_size);

TBAG_API int mg_get_response(mg_connection * conn, char * ebuf, size_t ebuf_len, int timeout);
TBAG_API unsigned mg_check_feature(unsigned feature);
TBAG_API int mg_get_system_info(char * buffer, int buflen);
TBAG_API int mg_get_context_info(mg_context const * ctx, char * buffer, int buflen);
TBAG_API int mg_get_connection_info(mg_context const * ctx, int idx, char * buffer, int buflen);

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBBYPASS_HPP__

