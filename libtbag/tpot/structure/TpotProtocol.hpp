/**
 * @file   TpotProtocol.hpp
 * @brief  TpotProtocol class prototype.
 * @author zer0
 * @date   2017-05-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_STRUCTURE_TPOTPROTOCOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_STRUCTURE_TPOTPROTOCOL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/network/http/HttpProperty.hpp>

#include <cstdint>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot      {
namespace structure {

// --------------------
// HTTP Path structure.
// --------------------

#ifndef _TPOT_CREATE_PATH_STRUCTURE
#define _TPOT_CREATE_PATH_STRUCTURE(name, path, method) \
    struct name : public libtbag::network::http::HttpMethod##method \
    { TBAG_CONSTEXPR static char const * const getPath() TBAG_NOEXCEPT { return path; } };
#endif

_TPOT_CREATE_PATH_STRUCTURE(    ExecPath, "/exec"    ,    PUT)
_TPOT_CREATE_PATH_STRUCTURE(HeartbitPath, "/heartbit",    GET)
_TPOT_CREATE_PATH_STRUCTURE(    ListPath, "/list"    ,    GET)
_TPOT_CREATE_PATH_STRUCTURE(    KillPath, "/kill"    , DELETE)

struct JsonAcceptHeader
{
    TBAG_CONSTEXPR static char const * const getAcceptKey  () TBAG_NOEXCEPT { return "Accept"; }
    TBAG_CONSTEXPR static char const * const getAcceptValue() TBAG_NOEXCEPT { return "application/json"; }
};

// ------------------------
// Tpot protocol structure.
// ------------------------

struct JsonBodyInterface : public JsonAcceptHeader
{
    JsonBodyInterface() { /* EMPTY. */ }
    virtual ~JsonBodyInterface() { /* EMPTY. */ }

    virtual Err toRequestJsonString(std::string & json) const = 0;
    virtual Err toResponseJsonString(std::string & json) const = 0;

    virtual Err fromRequestJsonString(std::string const & json) = 0;
    virtual Err fromResponseJsonString(std::string const & json) = 0;
};

struct ResponseResult
{
    struct {
        int32_t status;
    } result;

    /** if HTTP Status code is 200(OK). */
    inline bool isSuccess() const TBAG_NOEXCEPT
    { return result.status == 200; }
};

struct TBAG_API Exec : public JsonBodyInterface, public ResponseResult, public ExecPath
{
    TBAG_CONSTEXPR static char const * const getRequestFile() TBAG_NOEXCEPT { return "file"; }
    TBAG_CONSTEXPR static char const * const getRequestCwd () TBAG_NOEXCEPT { return  "cwd"; }
    TBAG_CONSTEXPR static char const * const getRequestArgs() TBAG_NOEXCEPT { return "args"; /* JSON ARRAY. */ }
    TBAG_CONSTEXPR static char const * const getRequestEnvs() TBAG_NOEXCEPT { return "envs"; /* JSON ARRAY. */ }
    TBAG_CONSTEXPR static char const * const getRequestUid () TBAG_NOEXCEPT { return  "uid"; }
    TBAG_CONSTEXPR static char const * const getRequestGid () TBAG_NOEXCEPT { return  "gid"; }

    struct {
        std::string file;
        std::string cwd;

        std::vector<std::string> args;
        std::vector<std::string> envs;

        uint32_t uid;
        uint32_t gid;
    } request;

    TBAG_CONSTEXPR static char const * const getResponseId() TBAG_NOEXCEPT { return "id"; }

    struct {
        int32_t id;
    } response;

    Exec() { /* EMPTY. */ }
    virtual ~Exec() { /* EMPTY. */ }

    virtual Err toRequestJsonString(std::string & json) const override;
    virtual Err toResponseJsonString(std::string & json) const override;
    virtual Err fromRequestJsonString(std::string const & json) override;
    virtual Err fromResponseJsonString(std::string const & json) override;
};

struct TBAG_API Heartbit : public JsonBodyInterface, public ResponseResult, public HeartbitPath
{
    Heartbit() { /* EMPTY. */ }
    virtual ~Heartbit() { /* EMPTY. */ }

    virtual Err toRequestJsonString(std::string & json) const override;
    virtual Err toResponseJsonString(std::string & json) const override;
    virtual Err fromRequestJsonString(std::string const & json) override;
    virtual Err fromResponseJsonString(std::string const & json) override;
};

struct TBAG_API List : public JsonBodyInterface, public ResponseResult, public ListPath
{
    TBAG_CONSTEXPR static char const * const getResponseIds() TBAG_NOEXCEPT { return "ids"; /* JSON ARRAY. */ }

    struct {
        std::vector<int32_t> ids;
    } response;

    List() { /* EMPTY. */ }
    virtual ~List() { /* EMPTY. */ }

    virtual Err toRequestJsonString(std::string & json) const override;
    virtual Err toResponseJsonString(std::string & json) const override;
    virtual Err fromRequestJsonString(std::string const & json) override;
    virtual Err fromResponseJsonString(std::string const & json) override;
};

struct TBAG_API Kill : public JsonBodyInterface, public ResponseResult, public KillPath
{
    TBAG_CONSTEXPR static char const * const getRequestId() TBAG_NOEXCEPT { return "id"; }

    struct {
        int32_t id;
    } request;

    Kill() { /* EMPTY. */ }
    virtual ~Kill() { /* EMPTY. */ }

    virtual Err toRequestJsonString(std::string & json) const override;
    virtual Err toResponseJsonString(std::string & json) const override;
    virtual Err fromRequestJsonString(std::string const & json) override;
    virtual Err fromResponseJsonString(std::string const & json) override;
};

} // namespace structure
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_STRUCTURE_TPOTPROTOCOL_HPP__

