/**
 * @file   CivetWebBypass.cpp
 * @brief  CivetWebBypass class implementation.
 * @author zer0
 * @date   2019-06-15
 */

#include <civetweb.h>

#include <libtbag/http/CivetWebBypass.hpp>
#include <libtbag/log/Log.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

unsigned mg_init_library(unsigned features)
{
    return ::mg_init_library(features);
}

unsigned mg_exit_library()
{
    return ::mg_exit_library();
}

mg_context * mg_start(mg_callbacks const * callbacks,
                      void * user_data,
                      const char ** configuration_options)
{
    return ::mg_start(callbacks, user_data, configuration_options);
}

void mg_stop(mg_context * ctx)
{
    ::mg_stop(ctx);
}

int mg_start_domain(struct mg_context * ctx, const char ** configuration_options)
{
#if defined(MG_EXPERIMENTAL_INTERFACES)
    return ::mg_start_domain(ctx, configuration_options);
#else
    return NOT_IMPLEMENTED;
#endif
}

void mg_set_request_handler(mg_context * ctx,
                            char const * uri,
                            mg_request_handler handler,
                            void * cbdata)
{
    ::mg_set_request_handler(ctx, uri, handler, cbdata);
}

void mg_set_websocket_handler(mg_context * ctx,
                              char const * uri,
                              mg_websocket_connect_handler connect_handler,
                              mg_websocket_ready_handler ready_handler,
                              mg_websocket_data_handler data_handler,
                              mg_websocket_close_handler close_handler,
                              void * cbdata)
{
    ::mg_set_websocket_handler(ctx, uri, connect_handler, ready_handler, data_handler, close_handler, cbdata);
}

void mg_set_websocket_handler_with_subprotocols(mg_context * ctx,
                                                char const * uri,
                                                mg_websocket_subprotocols * subprotocols,
                                                mg_websocket_connect_handler connect_handler,
                                                mg_websocket_ready_handler ready_handler,
                                                mg_websocket_data_handler data_handler,
                                                mg_websocket_close_handler close_handler,
                                                void * cbdata)
{
    ::mg_set_websocket_handler_with_subprotocols(ctx, uri, subprotocols,
                                                 connect_handler, ready_handler, data_handler, close_handler, cbdata);
}

void mg_set_auth_handler(mg_context * ctx,
                         char const * uri,
                         mg_authorization_handler handler,
                         void * cbdata)
{
    ::mg_set_auth_handler(ctx, uri, handler, cbdata);
}

char const * mg_get_option(mg_context const * ctx, char const * name)
{
    return ::mg_get_option(ctx, name);
}

mg_context * mg_get_context(mg_connection const * conn)
{
    return ::mg_get_context(conn);
}

void * mg_get_user_data(mg_context const * ctx)
{
    return ::mg_get_user_data(ctx);
}

void * mg_get_thread_pointer(mg_connection const * conn)
{
    return ::mg_get_thread_pointer(conn);
}

void mg_set_user_connection_data(mg_connection * conn, void * data)
{
    ::mg_set_user_connection_data(conn, data);
}

void * mg_get_user_connection_data(mg_connection const * conn)
{
    return ::mg_get_user_connection_data(conn);
}

int mg_get_request_link(mg_connection const * conn, char * buf, size_t buflen)
{
    return ::mg_get_request_link(conn, buf, buflen);
}

mg_option const * mg_get_valid_options()
{
    return ::mg_get_valid_options();
}

int mg_get_server_ports(mg_context const * ctx, int size, mg_server_port * ports)
{
    return ::mg_get_server_ports(ctx, size, ports);
}

int mg_modify_passwords_file(char const * passwords_file_name,
                             char const * realm,
                             char const * user,
                             char const * password)
{
    return ::mg_modify_passwords_file(passwords_file_name, realm, user, password);
}

mg_request_info const * mg_get_request_info(mg_connection const * conn)
{
    return ::mg_get_request_info(conn);
}

mg_response_info const * mg_get_response_info(mg_connection const * conn)
{
    return ::mg_get_response_info(conn);
}

int mg_write(mg_connection * conn, void const * buf, size_t len)
{
    return ::mg_write(conn, buf, len);
}

int mg_websocket_write(mg_connection * conn, int opcode, char const * data, size_t data_len)
{
    return ::mg_websocket_write(conn, opcode, data, data_len);
}

int mg_websocket_client_write(mg_connection * conn, int opcode, char const * data, size_t data_len)
{
    return ::mg_websocket_client_write(conn, opcode, data, data_len);
}

void mg_lock_connection(mg_connection * conn)
{
    ::mg_lock_connection(conn);
}

void mg_unlock_connection(mg_connection * conn)
{
    ::mg_unlock_connection(conn);
}

void mg_lock_context(mg_context * ctx)
{
    ::mg_lock_context(ctx);
}

void mg_unlock_context(mg_context * ctx)
{
    ::mg_unlock_context(ctx);
}

int mg_send_chunk(mg_connection * conn, char const * chunk, unsigned int chunk_len)
{
    return ::mg_send_chunk(conn, chunk, chunk_len);
}

void mg_send_file(mg_connection * conn, char const * path)
{
    return ::mg_send_file(conn, path);
}

int mg_send_file_body(mg_connection * conn, char const * path)
{
    return ::mg_send_file_body(conn, path);
}

int mg_send_http_error(mg_connection * conn, int status_code, char const * msg, int msglen)
{
    return ::mg_send_http_error(conn, status_code, "%s", std::string(msg, msg + msglen).c_str());
}

int mg_send_http_ok(mg_connection * conn, char const * mime_type, long long content_length)
{
    return ::mg_send_http_ok(conn, mime_type, content_length);
}

int mg_send_http_redirect(mg_connection * conn, char const * target_url, int redirect_code)
{
    return ::mg_send_http_redirect(conn, target_url, redirect_code);
}

int mg_send_digest_access_authentication_request(mg_connection * conn, char const * realm)
{
    return ::mg_send_digest_access_authentication_request(conn, realm);
}

int mg_check_digest_access_authentication(mg_connection * conn, char const * realm, char const * filename)
{
    return ::mg_check_digest_access_authentication(conn, realm, filename);
}

void mg_send_mime_file(mg_connection * conn, char const * path, char const * mime_type)
{
    ::mg_send_mime_file(conn, path, mime_type);
}

void mg_send_mime_file2(mg_connection * conn, char const * path, char const * mime_type, char const * additional_headers)
{
    ::mg_send_mime_file2(conn, path, mime_type, additional_headers);
}

long long mg_store_body(mg_connection * conn, char const * path)
{
    return ::mg_store_body(conn, path);
}

int mg_read(mg_connection * conn, void * buf, size_t len)
{
    return ::mg_read(conn, buf, len);
}

char const * mg_get_header(mg_connection const * conn, char const * name)
{
    return ::mg_get_header(conn, name);
}

int mg_get_var(char const * data, size_t data_len, char const * var_name,
               char * dst, size_t dst_len)
{
    return ::mg_get_var(data, data_len, var_name, dst, dst_len);
}

int mg_get_var2(char const * data, size_t data_len, char const * var_name,
                char * dst, size_t dst_len, size_t occurrence)
{
    return ::mg_get_var2(data, data_len, var_name, dst, dst_len, occurrence);
}

int mg_get_cookie(char const * cookie, char const * var_name,
                  char * buf, size_t buf_len)
{
    return ::mg_get_cookie(cookie, var_name, buf, buf_len);
}

mg_connection * mg_download(char const * host,
                            int port,
                            int use_ssl,
                            char * error_buffer,
                            size_t error_buffer_size,
                            char const * request, int reqlen)
{
    return ::mg_download(host, port, use_ssl, error_buffer, error_buffer_size,
                         "%s", std::string(request, request + reqlen).c_str());
}

void mg_close_connection(mg_connection * conn)
{
    ::mg_close_connection(conn);
}

int mg_handle_form_request(mg_connection * conn, mg_form_data_handler * fdh)
{
    return ::mg_handle_form_request(conn, fdh);
}

int mg_start_thread(mg_thread_func_t f, void * p)
{
    return ::mg_start_thread(f, p);
}

char const * mg_get_builtin_mime_type(char const * file_name)
{
    return ::mg_get_builtin_mime_type(file_name);
}

const char * mg_get_response_code_text(mg_connection const * conn, int response_code)
{
    return ::mg_get_response_code_text(conn, response_code);
}

char const * mg_version()
{
    return ::mg_version();
}

int mg_url_decode(char const * src, int src_len, char * dst, int dst_len, int is_form_url_encoded)
{
    return ::mg_url_decode(src, src_len, dst, dst_len, is_form_url_encoded);
}

int mg_url_encode(char const * src, char * dst, size_t dst_len)
{
    return ::mg_url_encode(src, dst, dst_len);
}

char * mg_md5(char buf[33], char const * msg, int msglen)
{
    return ::mg_md5(buf, std::string(msg, msg + msglen).c_str());
}

void mg_cry(mg_connection const * conn, char const * msg, int msglen)
{
    ::mg_cry(conn, "%s", std::string(msg, msg + msglen).c_str());
}

int mg_strcasecmp(char const * s1, char const * s2)
{
    return ::mg_strcasecmp(s1, s2);
}

int mg_strncasecmp(char const * s1, char const * s2, size_t len)
{
    return ::mg_strncasecmp(s1, s2, len);
}

mg_connection * mg_connect_websocket_client(char const * host,
                                            int port,
                                            int use_ssl,
                                            char * error_buffer,
                                            size_t error_buffer_size,
                                            char const * path,
                                            char const * origin,
                                            mg_websocket_data_handler data_func,
                                            mg_websocket_close_handler close_func,
                                            void * user_data)
{
    return ::mg_connect_websocket_client(host, port, use_ssl, error_buffer, error_buffer_size,
                                         path, origin, data_func, close_func, user_data);
}

mg_connection * mg_connect_client(char const * host,
                                  int port,
                                  int use_ssl,
                                  char * error_buffer,
                                  size_t error_buffer_size)
{
    return ::mg_connect_client(host, port, use_ssl, error_buffer, error_buffer_size);
}

mg_connection * mg_connect_client_secure(mg_client_options const * client_options,
                                         char * error_buffer,
                                         size_t error_buffer_size)
{
    return ::mg_connect_client_secure(client_options, error_buffer, error_buffer_size);
}

int mg_get_response(mg_connection * conn, char * ebuf, size_t ebuf_len, int timeout)
{
    return ::mg_get_response(conn, ebuf, ebuf_len, timeout);
}

unsigned mg_check_feature(unsigned feature)
{
    return ::mg_check_feature(feature);
}

int mg_get_system_info(char * buffer, int buflen)
{
    return ::mg_get_system_info(buffer, buflen);
}

int mg_get_context_info(mg_context const * ctx, char * buffer, int buflen)
{
    return ::mg_get_context_info(ctx, buffer, buflen);
}

int mg_get_connection_info(mg_context const * ctx, int idx, char * buffer, int buflen)
{
#if defined(MG_EXPERIMENTAL_INTERFACES)
    return ::mg_get_connection_info(ctx, idx, buffer, buflen);
#else
    return NOT_IMPLEMENTED;
#endif
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

