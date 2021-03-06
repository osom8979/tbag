/**
 * @file   SslUtils.cpp
 * @brief  SslUtils class implementation.
 * @author zer0
 * @date   2017-12-07
 *
 * @see <https://wiki.openssl.org/index.php/Library_Initialization>
 * @see <https://en.wikibooks.org/wiki/OpenSSL/Initialization>
 */

#include <libtbag/crypto/SslUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

void initializeSsl()
{
    // first, set up threading callbacks if your program is multithreaded
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(nullptr);

    ERR_load_crypto_strings();
    ERR_load_BIO_strings();
}

void releaseSsl()
{
    ERR_free_strings();
    RAND_cleanup();
    EVP_cleanup();
    // this seems to cause double-frees for me: ENGINE_cleanup();
    CONF_modules_free();
    ERR_remove_state(0); // If zero we look it up.
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

