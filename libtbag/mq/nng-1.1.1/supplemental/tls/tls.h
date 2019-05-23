#ifndef NNG_SUPPLEMENTAL_TLS_TLS_H
#define NNG_SUPPLEMENTAL_TLS_TLS_H

#include <stddef.h>
#include <stdint.h>

typedef struct nng_tls_config nng_tls_config;

typedef enum nng_tls_mode
{
    NNG_TLS_MODE_CLIENT = 0,
    NNG_TLS_MODE_SERVER = 1,
} nng_tls_mode;

typedef enum nng_tls_auth_mode
{
    NNG_TLS_AUTH_MODE_NONE     = 0,
    NNG_TLS_AUTH_MODE_OPTIONAL = 1,
    NNG_TLS_AUTH_MODE_REQUIRED = 2,
} nng_tls_auth_mode;

#endif // NNG_SUPPLEMENTAL_TLS_TLS_H
