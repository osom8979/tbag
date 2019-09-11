/**
 * @file   CivetWebServer.cpp
 * @brief  CivetWebServer class implementation.
 * @author zer0
 * @date   2019-09-11
 */

#include <libtbag/http/CivetWebServer.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>

#ifndef MAX_PARAM_BODY_LENGTH
// Set a default limit for parameters in a form body: 2 MB
#define MAX_PARAM_BODY_LENGTH (1024 * 1024 * 2)
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

CivetServer::CivetServer(char const ** options, CivetCallbacks const * cb, void const * user)
        : context(nullptr), _user(user)
{
    CivetCallbacks callbacks;
    if (cb) {
        callbacks = *cb;
        userCloseHandler = cb->connection_close;
    } else {
        userCloseHandler = nullptr;
    }
    callbacks.connection_close = closeHandler;

    context = mg_start(&callbacks, this, options);
    if (context == nullptr) {
        throw CivetException("Null context when constructing CivetServer. "
                             "Possible problem binding to port.");
    }
}

CivetServer::CivetServer(Options const & options, CivetCallbacks const * cb, void const * user)
        : context(nullptr), _user(user)
{
    CivetCallbacks callbacks;
    if (cb) {
        callbacks = *cb;
        userCloseHandler = cb->connection_close;
    } else {
        userCloseHandler = nullptr;
    }
    callbacks.connection_close = closeHandler;

    std::vector<char const *> pointers(options.size());
    for (size_t i = 0; i < options.size(); i++) {
        pointers[i] = (options[i].c_str());
    }
    pointers.push_back(nullptr);

    context = mg_start(&callbacks, this, &pointers[0]);
    if (context == nullptr) {
        throw CivetException("null context when constructing CivetServer. "
                             "Possible problem binding to port.");
    }
}

CivetServer::~CivetServer()
{
    close();
}

int
CivetServer::requestHandler(struct mg_connection *conn, void *cbdata)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    assert(request_info != nullptr);
    CivetServer *me = (CivetServer *)(request_info->user_data);
    assert(me != nullptr);

    // Happens when a request hits the server before the context is saved
    if (me->context == nullptr)
        return 0;

    mg_lock_context(me->context);
    me->connections[conn] = CivetConnection();
    mg_unlock_context(me->context);

    CivetHandler *handler = (CivetHandler *)cbdata;

    if (handler) {
        if (strcmp(request_info->request_method, "GET") == 0) {
            return handler->handleGet(me, conn) ? 1 : 0;
        } else if (strcmp(request_info->request_method, "POST") == 0) {
            return handler->handlePost(me, conn) ? 1 : 0;
        } else if (strcmp(request_info->request_method, "HEAD") == 0) {
            return handler->handleHead(me, conn) ? 1 : 0;
        } else if (strcmp(request_info->request_method, "PUT") == 0) {
            return handler->handlePut(me, conn) ? 1 : 0;
        } else if (strcmp(request_info->request_method, "DELETE") == 0) {
            return handler->handleDelete(me, conn) ? 1 : 0;
        } else if (strcmp(request_info->request_method, "OPTIONS") == 0) {
            return handler->handleOptions(me, conn) ? 1 : 0;
        } else if (strcmp(request_info->request_method, "PATCH") == 0) {
            return handler->handlePatch(me, conn) ? 1 : 0;
        }
    }

    return 0; // No handler found
}

int
CivetServer::authHandler(struct mg_connection *conn, void *cbdata)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    assert(request_info != nullptr);
    CivetServer *me = (CivetServer *)(request_info->user_data);
    assert(me != nullptr);

    // Happens when a request hits the server before the context is saved
    if (me->context == nullptr)
        return 0;

    mg_lock_context(me->context);
    me->connections[conn] = CivetConnection();
    mg_unlock_context(me->context);

    CivetAuthHandler *handler = (CivetAuthHandler *)cbdata;

    if (handler) {
        return handler->authorize(me, conn) ? 1 : 0;
    }

    return 0; // No handler found
}

int
CivetServer::webSocketConnectionHandler(const struct mg_connection *conn,
                                        void *cbdata)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    assert(request_info != nullptr);
    CivetServer *me = (CivetServer *)(request_info->user_data);
    assert(me != nullptr);

    // Happens when a request hits the server before the context is saved
    if (me->context == nullptr)
        return 0;

    CivetWebSocketHandler *handler = (CivetWebSocketHandler *)cbdata;

    if (handler) {
        return handler->handleConnection(me, conn) ? 0 : 1;
    }

    return 1; // No handler found, close connection
}

void
CivetServer::webSocketReadyHandler(struct mg_connection *conn, void *cbdata)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    assert(request_info != nullptr);
    CivetServer *me = (CivetServer *)(request_info->user_data);
    assert(me != nullptr);

    // Happens when a request hits the server before the context is saved
    if (me->context == nullptr)
        return;

    CivetWebSocketHandler *handler = (CivetWebSocketHandler *)cbdata;

    if (handler) {
        handler->handleReadyState(me, conn);
    }
}

int
CivetServer::webSocketDataHandler(struct mg_connection *conn,
                                  int bits,
                                  char *data,
                                  size_t data_len,
                                  void *cbdata)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    assert(request_info != nullptr);
    CivetServer *me = (CivetServer *)(request_info->user_data);
    assert(me != nullptr);

    // Happens when a request hits the server before the context is saved
    if (me->context == nullptr)
        return 0;

    CivetWebSocketHandler *handler = (CivetWebSocketHandler *)cbdata;

    if (handler) {
        return handler->handleData(me, conn, bits, data, data_len) ? 1 : 0;
    }

    return 1; // No handler found
}

void
CivetServer::webSocketCloseHandler(const struct mg_connection *conn,
                                   void *cbdata)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    assert(request_info != nullptr);
    CivetServer *me = (CivetServer *)(request_info->user_data);
    assert(me != nullptr);

    // Happens when a request hits the server before the context is saved
    if (me->context == nullptr)
        return;

    CivetWebSocketHandler *handler = (CivetWebSocketHandler *)cbdata;

    if (handler) {
        handler->handleClose(me, conn);
    }
}

void
CivetServer::closeHandler(const struct mg_connection *conn)
{
    CivetServer *me = (CivetServer *)mg_get_user_data(mg_get_context(conn));
    assert(me != nullptr);

    // Happens when a request hits the server before the context is saved
    if (me->context == nullptr)
        return;

    if (me->userCloseHandler) {
        me->userCloseHandler(conn);
    }
    mg_lock_context(me->context);
    me->connections.erase(const_cast<struct mg_connection *>(conn));
    mg_unlock_context(me->context);
}

void
CivetServer::addHandler(const std::string &uri, CivetHandler *handler)
{
    mg_set_request_handler(context, uri.c_str(), requestHandler, handler);
}

void
CivetServer::addWebSocketHandler(const std::string &uri,
                                 CivetWebSocketHandler *handler)
{
    mg_set_websocket_handler(context,
                             uri.c_str(),
                             webSocketConnectionHandler,
                             webSocketReadyHandler,
                             webSocketDataHandler,
                             webSocketCloseHandler,
                             handler);
}

void
CivetServer::addAuthHandler(const std::string &uri, CivetAuthHandler *handler)
{
    mg_set_auth_handler(context, uri.c_str(), authHandler, handler);
}

void
CivetServer::removeHandler(const std::string &uri)
{
    mg_set_request_handler(context, uri.c_str(), nullptr, nullptr);
}

void
CivetServer::removeWebSocketHandler(const std::string &uri)
{
    mg_set_websocket_handler(
            context, uri.c_str(), nullptr, nullptr, nullptr, nullptr, nullptr);
}

void
CivetServer::removeAuthHandler(const std::string &uri)
{
    mg_set_auth_handler(context, uri.c_str(), nullptr, nullptr);
}

void
CivetServer::close()
{
    if (context) {
        mg_stop(context);
        context = 0;
    }
}

int
CivetServer::getCookie(struct mg_connection *conn,
                       const std::string &cookieName,
                       std::string &cookieValue)
{
    // Maximum cookie length as per microsoft is 4096.
    // http://msdn.microsoft.com/en-us/library/ms178194.aspx
    char _cookieValue[4096];
    const char *cookie = mg_get_header(conn, "Cookie");
    int lRead = mg_get_cookie(cookie,
                              cookieName.c_str(),
                              _cookieValue,
                              sizeof(_cookieValue));
    cookieValue.clear();
    cookieValue.append(_cookieValue);
    return lRead;
}

const char *
CivetServer::getHeader(struct mg_connection *conn,
                       const std::string &headerName)
{
    return mg_get_header(conn, headerName.c_str());
}

void
CivetServer::urlDecode(const char *src,
                       std::string &dst,
                       bool is_form_url_encoded)
{
    urlDecode(src, strlen(src), dst, is_form_url_encoded);
}

void
CivetServer::urlDecode(const char *src,
                       size_t src_len,
                       std::string &dst,
                       bool is_form_url_encoded)
{
    int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

    dst.clear();
    for (i = j = 0; i < (int)src_len; i++, j++) {
        if (i < (int)src_len - 2 && src[i] == '%'
            && isxdigit((unsigned char)src[i + 1])
            && isxdigit((unsigned char)src[i + 2])) {
            a = tolower((unsigned char)src[i + 1]);
            b = tolower((unsigned char)src[i + 2]);
            dst.push_back((char)((HEXTOI(a) << 4) | HEXTOI(b)));
            i += 2;
        } else if (is_form_url_encoded && src[i] == '+') {
            dst.push_back(' ');
        } else {
            dst.push_back(src[i]);
        }
    }
}

bool
CivetServer::getParam(struct mg_connection *conn,
                      const char *name,
                      std::string &dst,
                      size_t occurrence)
{
    const char *formParams = nullptr;
    const char *queryString = nullptr;
    const struct mg_request_info *ri = mg_get_request_info(conn);
    assert(ri != nullptr);
    CivetServer *me = (CivetServer *)(ri->user_data);
    assert(me != nullptr);
    mg_lock_context(me->context);
    CivetConnection &conobj = me->connections[conn];
    mg_lock_connection(conn);
    mg_unlock_context(me->context);

    if (conobj.postData != nullptr) {
        // check if form parameter are already stored
        formParams = conobj.postData;
    } else {
        // otherwise, check if there is a request body
        const char *con_len_str = mg_get_header(conn, "Content-Length");
        if (con_len_str) {
            char *end = 0;
            unsigned long con_len = strtoul(con_len_str, &end, 10);
            if ((end == nullptr) || (*end != 0)) {
                // malformed header
                mg_unlock_connection(conn);
                return false;
            }
            if ((con_len > 0) && (con_len <= MAX_PARAM_BODY_LENGTH)) {
                // Body is within a reasonable range

                // Allocate memory:
                // Add one extra character: in case the post-data is a text, it
                // is required as 0-termination.
                // Do not increment con_len, since the 0 terminating is not part
                // of the content (text or binary).
                conobj.postData = (char *)malloc(con_len + 1);
                if (conobj.postData != nullptr) {
                    // malloc may fail for huge requests
                    mg_read(conn, conobj.postData, con_len);
                    conobj.postData[con_len] = 0;
                    formParams = conobj.postData;
                    conobj.postDataLen = con_len;
                }
            }
            if (conobj.postData == nullptr) {
                // we cannot store the body
                mg_unlock_connection(conn);
                return false;
            }
        }
    }

    if (ri->query_string != nullptr) {
        // get requests do store html <form> field values in the http
        // query_string
        queryString = ri->query_string;
    }

    mg_unlock_connection(conn);

    bool get_param_success = false;
    if (formParams != nullptr) {
        get_param_success =
                getParam(formParams, strlen(formParams), name, dst, occurrence);
    }
    if (!get_param_success && queryString != nullptr) {
        get_param_success =
                getParam(queryString, strlen(queryString), name, dst, occurrence);
    }

    return get_param_success;
}

bool
CivetServer::getParam(const char *data,
                      size_t data_len,
                      const char *name,
                      std::string &dst,
                      size_t occurrence)
{
    const char *p, *e, *s;
    size_t name_len;

    dst.clear();
    if (data == nullptr || name == nullptr || data_len == 0) {
        return false;
    }
    name_len = strlen(name);
    e = data + data_len;

    // data is "var1=val1&var2=val2...". Find variable first
    for (p = data; p + name_len < e; p++) {
        if ((p == data || p[-1] == '&') && p[name_len] == '='
            && !mg_strncasecmp(name, p, name_len) && 0 == occurrence--) {

            // Point p to variable value
            p += name_len + 1;

            // Point s to the end of the value
            s = (const char *)memchr(p, '&', (size_t)(e - p));
            if (s == nullptr) {
                s = e;
            }
            assert(s >= p);

            // Decode variable into destination buffer
            urlDecode(p, (s - p), dst, true);
            return true;
        }
    }
    return false;
}

std::string
CivetServer::getPostData(struct mg_connection *conn)
{
    mg_lock_connection(conn);
    std::string postdata;
    char buf[2048];
    int r = mg_read(conn, buf, sizeof(buf));
    while (r > 0) {
        postdata += std::string(buf, r);
        r = mg_read(conn, buf, sizeof(buf));
    }
    mg_unlock_connection(conn);
    return postdata;
}

void
CivetServer::urlEncode(const char *src, std::string &dst, bool append)
{
    urlEncode(src, strlen(src), dst, append);
}

void
CivetServer::urlEncode(const char *src,
                       size_t src_len,
                       std::string &dst,
                       bool append)
{
    static const char *dont_escape = "._-$,;~()";
    static const char *hex = "0123456789abcdef";

    if (!append)
        dst.clear();

    for (; src_len > 0; src++, src_len--) {
        if (isalnum((unsigned char)*src) || strchr(dont_escape, *src) != nullptr) {
            dst.push_back(*src);
        } else {
            dst.push_back('%');
            dst.push_back(hex[(unsigned char)*src >> 4]);
            dst.push_back(hex[(unsigned char)*src & 0xf]);
        }
    }
}

std::vector<int>
CivetServer::getListeningPorts()
{
    std::vector<struct mg_server_port> server_ports = getListeningPortsFull();

    std::vector<int> ports(server_ports.size());
    for (size_t i = 0; i < server_ports.size(); i++) {
        ports[i] = server_ports[i].port;
    }

    return ports;
}

std::vector<struct mg_server_port>
CivetServer::getListeningPortsFull()
{
    std::vector<struct mg_server_port> server_ports(50);
    int size = mg_get_server_ports(context,
                                   (int)server_ports.size(),
                                   &server_ports[0]);
    if (size <= 0) {
        server_ports.resize(0);
        return server_ports;
    }
    server_ports.resize(size);
    return server_ports;
}

CivetServer::CivetConnection::CivetConnection()
{
    postData = nullptr;
    postDataLen = 0;
}

CivetServer::CivetConnection::~CivetConnection()
{
    free(postData);
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

