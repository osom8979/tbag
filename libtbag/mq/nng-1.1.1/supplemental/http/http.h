#ifndef NNG_SUPPLEMENTAL_HTTP_HTTP_H
#define NNG_SUPPLEMENTAL_HTTP_HTTP_H

// HTTP API.  Only present if HTTP support compiled into the library.
// Functions will return NNG_ENOTSUP (or NULL or 0 as appropriate)
// if the library lacks support for HTTP.

#include <stdint.h>

struct nng_tls_config;

// HTTP status codes.
// This list is not exhaustive.
enum nng_http_status
{
    NNG_HTTP_STATUS_CONTINUE                 = 100,
    NNG_HTTP_STATUS_SWITCHING                = 101,
    NNG_HTTP_STATUS_PROCESSING               = 102,
    NNG_HTTP_STATUS_OK                       = 200,
    NNG_HTTP_STATUS_CREATED                  = 201,
    NNG_HTTP_STATUS_ACCEPTED                 = 202,
    NNG_HTTP_STATUS_NOT_AUTHORITATIVE        = 203,
    NNG_HTTP_STATUS_NO_CONTENT               = 204,
    NNG_HTTP_STATUS_RESET_CONTENT            = 205,
    NNG_HTTP_STATUS_PARTIAL_CONTENT          = 206,
    NNG_HTTP_STATUS_MULTI_STATUS             = 207,
    NNG_HTTP_STATUS_ALREADY_REPORTED         = 208,
    NNG_HTTP_STATUS_IM_USED                  = 226,
    NNG_HTTP_STATUS_MULTIPLE_CHOICES         = 300,
    NNG_HTTP_STATUS_STATUS_MOVED_PERMANENTLY = 301,
    NNG_HTTP_STATUS_FOUND                    = 302,
    NNG_HTTP_STATUS_SEE_OTHER                = 303,
    NNG_HTTP_STATUS_NOT_MODIFIED             = 304,
    NNG_HTTP_STATUS_USE_PROXY                = 305,
    NNG_HTTP_STATUS_TEMPORARY_REDIRECT       = 307,
    NNG_HTTP_STATUS_PERMANENT_REDIRECT       = 308,
    NNG_HTTP_STATUS_BAD_REQUEST              = 400,
    NNG_HTTP_STATUS_UNAUTHORIZED             = 401,
    NNG_HTTP_STATUS_PAYMENT_REQUIRED         = 402,
    NNG_HTTP_STATUS_FORBIDDEN                = 403,
    NNG_HTTP_STATUS_NOT_FOUND                = 404,
    NNG_HTTP_STATUS_METHOD_NOT_ALLOWED       = 405,
    NNG_HTTP_STATUS_NOT_ACCEPTABLE           = 406,
    NNG_HTTP_STATUS_PROXY_AUTH_REQUIRED      = 407,
    NNG_HTTP_STATUS_REQUEST_TIMEOUT          = 408,
    NNG_HTTP_STATUS_CONFLICT                 = 409,
    NNG_HTTP_STATUS_GONE                     = 410,
    NNG_HTTP_STATUS_LENGTH_REQUIRED          = 411,
    NNG_HTTP_STATUS_PRECONDITION_FAILED      = 412,
    NNG_HTTP_STATUS_PAYLOAD_TOO_LARGE        = 413,
    NNG_HTTP_STATUS_ENTITY_TOO_LONG          = 414,
    NNG_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE   = 415,
    NNG_HTTP_STATUS_RANGE_NOT_SATISFIABLE    = 416,
    NNG_HTTP_STATUS_EXPECTATION_FAILED       = 417,
    NNG_HTTP_STATUS_TEAPOT                   = 418,
    NNG_HTTP_STATUS_UNPROCESSABLE_ENTITY     = 422,
    NNG_HTTP_STATUS_LOCKED                   = 423,
    NNG_HTTP_STATUS_FAILED_DEPENDENCY        = 424,
    NNG_HTTP_STATUS_UPGRADE_REQUIRED         = 426,
    NNG_HTTP_STATUS_PRECONDITION_REQUIRED    = 428,
    NNG_HTTP_STATUS_TOO_MANY_REQUESTS        = 429,
    NNG_HTTP_STATUS_HEADERS_TOO_LARGE        = 431,
    NNG_HTTP_STATUS_UNAVAIL_LEGAL_REASONS    = 451,
    NNG_HTTP_STATUS_INTERNAL_SERVER_ERROR    = 500,
    NNG_HTTP_STATUS_NOT_IMPLEMENTED          = 501,
    NNG_HTTP_STATUS_BAD_GATEWAY              = 502,
    NNG_HTTP_STATUS_SERVICE_UNAVAILABLE      = 503,
    NNG_HTTP_STATUS_GATEWAY_TIMEOUT          = 504,
    NNG_HTTP_STATUS_HTTP_VERSION_NOT_SUPP    = 505,
    NNG_HTTP_STATUS_VARIANT_ALSO_NEGOTIATES  = 506,
    NNG_HTTP_STATUS_INSUFFICIENT_STORAGE     = 507,
    NNG_HTTP_STATUS_LOOP_DETECTED            = 508,
    NNG_HTTP_STATUS_NOT_EXTENDED             = 510,
    NNG_HTTP_STATUS_NETWORK_AUTH_REQUIRED    = 511,
};

typedef struct nng_http_req nng_http_req;
typedef struct nng_http_res nng_http_res;
typedef struct nng_http_conn nng_http_conn;
typedef struct nng_http_handler nng_http_handler;
typedef struct nng_http_server nng_http_server;
typedef struct nng_http_client nng_http_client;

#endif // NNG_SUPPLEMENTAL_HTTP_HTTP_H
