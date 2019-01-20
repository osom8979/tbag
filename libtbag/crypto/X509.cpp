/**
 * @file   X509.cpp
 * @brief  X509 class implementation.
 * @author zer0
 * @date   2018-01-31
 */

#include <libtbag/crypto/X509.hpp>
#include <libtbag/log/Log.hpp>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/objects.h>
#include <openssl/ocsp.h>

#include <cassert>
#include <cstring>
#include <vector>
#include <memory>

#ifndef X509_FLAG_CA
#define X509_FLAG_CA (X509_FLAG_NO_ISSUER | X509_FLAG_NO_PUBKEY | X509_FLAG_NO_HEADER | X509_FLAG_NO_VERSION)
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

struct NAME_EX_TBL
{
    char const * name;
    unsigned long flag;
    unsigned long mask;
};

static int _set_table_opts(unsigned long * flags, char const * arg, NAME_EX_TBL const * in_tbl)
{
    char c;
    NAME_EX_TBL const * ptbl;

    c = arg[0];
    if (c == '-') {
        c = 0;
        arg++;
    } else if (c == '+') {
        c = 1;
        arg++;
    } else {
        c = 1;
    }

    for (ptbl = in_tbl; ptbl->name; ptbl++) {
        if (!strcasecmp(arg, ptbl->name)) {
            *flags &= ~ptbl->mask;
            if (c) {
                *flags |= ptbl->flag;
            } else {
                *flags &= ~ptbl->flag;
            }
            return 1;
        }
    }
    return 0;
}

static int _set_multi_opts(unsigned long * flags, char const * arg, NAME_EX_TBL const * in_tbl)
{
    STACK_OF(CONF_VALUE) * vals;
    CONF_VALUE * val;
    int i, ret = 1;

    if (!arg) {
        return 0;
    }
    vals = X509V3_parse_list(arg);
    for (i = 0; i < sk_CONF_VALUE_num(vals); i++) {
        val = sk_CONF_VALUE_value(vals, i);
        if (!_set_table_opts(flags, val->name, in_tbl)) {
            ret = 0;
        }
    }
    sk_CONF_VALUE_pop_free(vals, X509V3_conf_free);
    return ret;
}

static int _set_cert_ex(unsigned long * flags, char const * arg)
{
    static NAME_EX_TBL const cert_tbl[] = {
            {"compatible", X509_FLAG_COMPAT, 0xffffffffl},
            {"ca_default", X509_FLAG_CA, 0xffffffffl},
            {"no_header", X509_FLAG_NO_HEADER, 0},
            {"no_version", X509_FLAG_NO_VERSION, 0},
            {"no_serial", X509_FLAG_NO_SERIAL, 0},
            {"no_signame", X509_FLAG_NO_SIGNAME, 0},
            {"no_validity", X509_FLAG_NO_VALIDITY, 0},
            {"no_subject", X509_FLAG_NO_SUBJECT, 0},
            {"no_issuer", X509_FLAG_NO_ISSUER, 0},
            {"no_pubkey", X509_FLAG_NO_PUBKEY, 0},
            {"no_extensions", X509_FLAG_NO_EXTENSIONS, 0},
            {"no_sigdump", X509_FLAG_NO_SIGDUMP, 0},
            {"no_aux", X509_FLAG_NO_AUX, 0},
            {"no_attributes", X509_FLAG_NO_ATTRIBUTES, 0},
            {"ext_default", X509V3_EXT_DEFAULT, X509V3_EXT_UNKNOWN_MASK},
            {"ext_error", X509V3_EXT_ERROR_UNKNOWN, X509V3_EXT_UNKNOWN_MASK},
            {"ext_parse", X509V3_EXT_PARSE_UNKNOWN, X509V3_EXT_UNKNOWN_MASK},
            {"ext_dump", X509V3_EXT_DUMP_UNKNOWN, X509V3_EXT_UNKNOWN_MASK},
            {nullptr, 0, 0}
    };
    return _set_multi_opts(flags, arg, cert_tbl);
}

static int _set_name_ex(unsigned long * flags, char const * arg)
{
    static NAME_EX_TBL const ex_tbl[] = {
            {"esc_2253", ASN1_STRFLGS_ESC_2253, 0},
            {"esc_ctrl", ASN1_STRFLGS_ESC_CTRL, 0},
            {"esc_msb", ASN1_STRFLGS_ESC_MSB, 0},
            {"use_quote", ASN1_STRFLGS_ESC_QUOTE, 0},
            {"utf8", ASN1_STRFLGS_UTF8_CONVERT, 0},
            {"ignore_type", ASN1_STRFLGS_IGNORE_TYPE, 0},
            {"show_type", ASN1_STRFLGS_SHOW_TYPE, 0},
            {"dump_all", ASN1_STRFLGS_DUMP_ALL, 0},
            {"dump_nostr", ASN1_STRFLGS_DUMP_UNKNOWN, 0},
            {"dump_der", ASN1_STRFLGS_DUMP_DER, 0},
            {"compat", XN_FLAG_COMPAT, 0xffffffffL},
            {"sep_comma_plus", XN_FLAG_SEP_COMMA_PLUS, XN_FLAG_SEP_MASK},
            {"sep_comma_plus_space", XN_FLAG_SEP_CPLUS_SPC, XN_FLAG_SEP_MASK},
            {"sep_semi_plus_space", XN_FLAG_SEP_SPLUS_SPC, XN_FLAG_SEP_MASK},
            {"sep_multiline", XN_FLAG_SEP_MULTILINE, XN_FLAG_SEP_MASK},
            {"dn_rev", XN_FLAG_DN_REV, 0},
            {"nofname", XN_FLAG_FN_NONE, XN_FLAG_FN_MASK},
            {"sname", XN_FLAG_FN_SN, XN_FLAG_FN_MASK},
            {"lname", XN_FLAG_FN_LN, XN_FLAG_FN_MASK},
            {"align", XN_FLAG_FN_ALIGN, 0},
            {"oid", XN_FLAG_FN_OID, XN_FLAG_FN_MASK},
            {"space_eq", XN_FLAG_SPC_EQ, 0},
            {"dump_unknown", XN_FLAG_DUMP_UNKNOWN_FIELDS, 0},
            {"RFC2253", XN_FLAG_RFC2253, 0xffffffffL},
            {"oneline", XN_FLAG_ONELINE, 0xffffffffL},
            {"multiline", XN_FLAG_MULTILINE, 0xffffffffL},
            {"ca_default", XN_FLAG_MULTILINE, 0xffffffffL},
            {nullptr, 0, 0}
    };
    return _set_multi_opts(flags, arg, ex_tbl);
}

static unsigned long getNameExFlag(std::string const & options)
{
    if (!options.empty()) {
        unsigned long name_flag = 0;
        if (_set_name_ex(&name_flag, options.c_str())) {
            return name_flag;
        }
    }
    return 0;
}

static unsigned long getCertExFlag(std::string const & options)
{
    if (!options.empty()) {
        unsigned long request_flag = 0;
        if (_set_cert_ex(&request_flag, options.c_str())) {
            return request_flag;
        }
    }
    return 0;
}

X509NameEntries getDefaultX509NameEntry(
        std::string const & country,
        std::string const & state,
        std::string const & locality,
        std::string const & organization,
        std::string const & organizational_unit,
        std::string const & common_name,
        std::string const & email_address)
{
    if (country.size() < 2) {
        tDLogW("getDefaultX509NameEntry() Please enter at least 2 letters of the country.");
    }

    X509NameEntries entries;
    // @formatter:on
    entries.push_back(X509NameEntry{COUNTRY            , country            });
    entries.push_back(X509NameEntry{STATE              , state              });
    entries.push_back(X509NameEntry{LOCALITY           , locality           });
    entries.push_back(X509NameEntry{ORGANIZATION       , organization       });
    entries.push_back(X509NameEntry{ORGANIZATIONAL_UNIT, organizational_unit});
    entries.push_back(X509NameEntry{COMMON_NAME        , common_name        });
    entries.push_back(X509NameEntry{EMAIL_ADDRESS      , email_address      });
    // @formatter:off
    return entries;
}

X509NameEntries getDefaultX509NameEntry()
{
    return getDefaultX509NameEntry("ko", "province", "city", "company", "unit", "localhost", "no@localhost");
}

static std::size_t updateX509Subect(X509NameEntries const & entries, X509_REQ * x509_req)
{
    std::size_t count = 0;
    auto * subject = X509_REQ_get_subject_name(x509_req);
    for (auto const & entry : entries) {
        auto const * key = entry.key.c_str();
        auto const * val = (unsigned char const *)entry.val.c_str();
        if (X509_NAME_add_entry_by_txt(subject, key, MBSTRING_ASC, val, -1, -1, 0) == 1){
            ++count;
        }
    }
    return count;
}

static std::shared_ptr<EVP_PKEY> readPemPrivateKey(std::string const & pem)
{
    std::shared_ptr<BIO> key_mem(BIO_new(BIO_s_mem()), [](BIO * bio){
        BIO_free_all(bio);
    });

    int const KEY_SIZE = static_cast<int>(pem.size());
    int const WRITTEN_SIZE = BIO_write(key_mem.get(), pem.data(), KEY_SIZE);
    assert(WRITTEN_SIZE == KEY_SIZE);

    EVP_PKEY * temp_key = nullptr;
    PEM_read_bio_PrivateKey(key_mem.get(), &temp_key, nullptr, nullptr);

    std::shared_ptr<EVP_PKEY> pkey;
    if (temp_key != nullptr) {
        pkey.reset(temp_key, [](EVP_PKEY * key){
            EVP_PKEY_free(key);
        });
    }
    return pkey;
}

static std::string getX509CsrPem(X509_REQ * x509_req)
{
    std::shared_ptr<BIO> mem(BIO_new(BIO_s_mem()), [](BIO * bio){
        BIO_free_all(bio);
    });

    if (PEM_write_bio_X509_REQ(mem.get(), x509_req) != 1) {
        return std::string();
    }

    int const PUBLIC_LENGTH = BIO_pending(mem.get());
    std::vector<char> buffer;
    buffer.resize(PUBLIC_LENGTH);

    BIO_read(mem.get(), buffer.data(), buffer.size());
    return std::string(buffer.begin(), buffer.end());
}

std::string generateCsrVersion1(X509NameEntries const & entries, std::string const & pem_private_key)
{
    std::shared_ptr<X509_REQ> x509_req(X509_REQ_new(), [](X509_REQ * req){
        X509_REQ_free(req);
    });

    int const VERSION = 1;
    if (X509_REQ_set_version(x509_req.get(), VERSION) != 1){
        return std::string();
    }

    auto const UPDATE_COUNT = updateX509Subect(entries, x509_req.get());
    assert(entries.size() == UPDATE_COUNT);

    auto pkey = readPemPrivateKey(pem_private_key);
    if (X509_REQ_set_pubkey(x509_req.get(), pkey.get()) != 1){
        return std::string();
    }
    if (X509_REQ_sign(x509_req.get(), pkey.get(), EVP_sha1()) <= 0){
        return std::string();
    }
    return getX509CsrPem(x509_req.get());
}

std::string generateCsrVersion1(std::string const & pem_private_key)
{
    return generateCsrVersion1(getDefaultX509NameEntry(), pem_private_key);
}

static std::shared_ptr<X509_REQ> readPemX509Csr(std::string const & pem_csr)
{
    std::shared_ptr<BIO> mem(BIO_new(BIO_s_mem()), [](BIO * bio){
        BIO_free_all(bio);
    });
    int const CSR_SIZE = static_cast<int>(pem_csr.size());
    int const WRITTEN_SIZE = BIO_write(mem.get(), pem_csr.data(), CSR_SIZE);
    assert(WRITTEN_SIZE == CSR_SIZE);

    X509_REQ * temp_req = nullptr;
    PEM_read_bio_X509_REQ(mem.get(), &temp_req, nullptr, nullptr);

    std::shared_ptr<X509_REQ> x509_req;
    if (temp_req != nullptr) {
        x509_req.reset(temp_req, [](X509_REQ * req){
            X509_REQ_free(req);
        });
    }
    return x509_req;
}

static std::string getPrinterbleX509CsrInformation(X509_REQ * x509_req,
                                                   unsigned long name_flag,
                                                   unsigned long request_flag)
{
    std::shared_ptr<BIO> mem(BIO_new(BIO_s_mem()), [](BIO * bio){
        BIO_free_all(bio);
    });

    if (X509_REQ_print_ex(mem.get(), x509_req, name_flag, request_flag) != 1) {
        return std::string();
    }

    int const MEM_LENGTH = BIO_pending(mem.get());
    std::vector<char> buffer;
    buffer.resize(MEM_LENGTH);

    BIO_read(mem.get(), buffer.data(), buffer.size());
    return std::string(buffer.begin(), buffer.end());
}

std::string decoderCsr(std::string const & pem_csr,
                       std::string const & name_options,
                       std::string const & request_options)
{
    auto x509_req = readPemX509Csr(pem_csr);
    auto const NAME_FLAG = getNameExFlag(name_options);
    auto const REQUEST_FLAG = getCertExFlag(request_options);
    return getPrinterbleX509CsrInformation(x509_req.get(), NAME_FLAG, REQUEST_FLAG);
}

std::string decoderCsr(std::string const & pem_csr)
{
    return decoderCsr(pem_csr, std::string(), std::string());
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

