/**
 * @file   TpotProtocol.cpp
 * @brief  TpotProtocol class implementation.
 * @author zer0
 * @date   2017-05-28
 */

#include <libtbag/tpot/structure/TpotProtocol.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/3rd/jsoncpp/json.h>
#include "TpotProtocol.hpp"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot      {
namespace structure {

TBAG_CONSTEXPR static bool const IS_COLLECT_COMMENTS = false;

// --------------------
// Exec implementation.
// --------------------

Err Exec::toRequestString(std::string & json) const
{
    if (request.file.empty()) {
        return Err::E_ILLARGS;
    }

    Json::Value root;
    root[getRequestFile()] = request.file;

    if (request.cwd.empty() == false) {
        root[getRequestCwd()] = request.cwd;
    }

    int const ARGS_SIZE = static_cast<int>(request.args.size());
    int const ENVS_SIZE = static_cast<int>(request.envs.size());
    int i = 0;

    for (i = 0; i < ARGS_SIZE; ++i) {
        root[getRequestArgs()][i] = request.args[i];
    }
    for (i = 0; i < ENVS_SIZE; ++i) {
        root[getRequestEnvs()][i] = request.envs[i];
    }

    if (request.uid != 0) {
        root[getRequestUid()] = request.uid;
    }
    if (request.gid != 0) {
        root[getRequestGid()] = request.gid;
    }

    Json::StreamWriterBuilder writer;
    json.assign(Json::writeString(writer, root));
    return Err::E_SUCCESS;
}

Err Exec::toResponseString(std::string & json) const
{
    Json::Value root;
    root[getResponseId()] = response.id;

    Json::StreamWriterBuilder writer;
    json.assign(Json::writeString(writer, root));
    return Err::E_SUCCESS;
}

Err Exec::fromRequestString(std::string const & json)
{
    request.file.clear();
    request.cwd.clear();
    request.args.clear();
    request.envs.clear();
    request.uid = 0;
    request.gid = 0;

    Json::Value root;
    Json::Reader reader;
    if (reader.parse(json, root, IS_COLLECT_COMMENTS) == false) {
        return Err::E_PARSING;
    }

    if (root[getRequestFile()].isString() == false) {
        return Err::E_PARSING;
    }

    request.file = root[getRequestFile()].asString();

    if (root[getRequestCwd()].isString()) {
        request.cwd = root[getRequestCwd()].asString();
    }

    if (root[getRequestArgs()].isArray()) {
        int const ARGS_SIZE = root[getRequestArgs()].size();
        request.args.resize(static_cast<std::size_t>(ARGS_SIZE));
        for (int i = 0; i < ARGS_SIZE; ++i) {
            if (root[getRequestArgs()][i].isString()) {
                request.args[i] = root[getRequestArgs()][i].asString();
            } else {
                request.args[i].clear();
            }
        }
    }

    if (root[getRequestEnvs()].isArray()) {
        int const ENVS_SIZE = root[getRequestEnvs()].size();
        request.envs.resize(static_cast<std::size_t>(ENVS_SIZE));
        for (int i = 0; i < ENVS_SIZE; ++i) {
            if (root[getRequestEnvs()][i].isString()) {
                request.envs[i] = root[getRequestEnvs()][i].asString();
            } else {
                request.envs[i].clear();
            }
        }
    }

    if (root[getRequestUid()].isUInt()) {
        request.uid = root[getRequestUid()].asUInt();
    } else {
        request.uid = 0;
    }

    if (root[getRequestGid()].isUInt()) {
        request.gid = root[getRequestGid()].asUInt();
    } else {
        request.gid = 0;
    }

    return Err::E_SUCCESS;
}

Err Exec::fromResponseString(std::string const & json)
{
    response.id = 0;

    Json::Value root;
    Json::Reader reader;
    if (reader.parse(json, root, IS_COLLECT_COMMENTS) == false) {
        return Err::E_PARSING;
    }

    if (root[getResponseId()].isInt() == false) {
        return Err::E_PARSING;
    }

    response.id = root[getResponseId()].asInt();
    return Err::E_SUCCESS;
}

// ------------------------
// Heartbit implementation.
// ------------------------

Err Heartbit::toRequestString(std::string & json) const
{
    return Err::E_SUCCESS; // NO REQUEST.
}

Err Heartbit::toResponseString(std::string & json) const
{
    return Err::E_SUCCESS; // NO RESPONSE.
}

Err Heartbit::fromRequestString(std::string const & json)
{
    return Err::E_SUCCESS; // NO REQUEST.
}

Err Heartbit::fromResponseString(std::string const & json)
{
    return Err::E_SUCCESS; // NO RESPONSE.
}

// --------------------
// List implementation.
// --------------------

Err List::toRequestString(std::string & json) const
{
    json.clear();
    return Err::E_SUCCESS; // NO REQUEST.
}

Err List::toResponseString(std::string & json) const
{
    Json::Value root;

    int const SIZE = static_cast<int>(response.ids.size());
    for (int i = 0; i < SIZE; ++i) {
        root[getResponseIds()][i] = response.ids[i];
    }

    Json::StreamWriterBuilder writer;
    json.assign(Json::writeString(writer, root));
    return Err::E_SUCCESS;
}

Err List::fromRequestString(std::string const & json)
{
    return Err::E_SUCCESS; // NO REQUEST.
}

Err List::fromResponseString(std::string const & json)
{
    response.ids.clear();

    Json::Value root;
    Json::Reader reader;
    if (reader.parse(json, root, IS_COLLECT_COMMENTS) == false) {
        return Err::E_PARSING;
    }

    if (root[getResponseIds()].isArray() == false) {
        return Err::E_PARSING;
    }

    int const SIZE = root[getResponseIds()].size();
    response.ids.resize(static_cast<std::size_t>(SIZE));
    for (int i = 0; i < SIZE; ++i) {
        if (root[getResponseIds()][i].isInt()) {
            response.ids[i] = root[getResponseIds()][i].asInt();
        } else {
            response.ids[i] = 0;
        }
    }

    return Err::E_SUCCESS;
}

// --------------------
// Kill implementation.
// --------------------

Err Kill::toRequestString(std::string & json) const
{
    Json::Value root;
    root[getRequestId()] = request.id;

    Json::StreamWriterBuilder writer;
    json.assign(Json::writeString(writer, root));
    return Err::E_SUCCESS;
}

Err Kill::toResponseString(std::string & json) const
{
    json.clear();
    return Err::E_SUCCESS; // NO RESPONSE.
}

Err Kill::fromRequestString(std::string const & json)
{
    request.id = 0;

    Json::Value root;
    Json::Reader reader;
    if (reader.parse(json, root, IS_COLLECT_COMMENTS) == false) {
        return Err::E_PARSING;
    }

    if (root[getRequestId()].isInt() == false) {
        return Err::E_PARSING;
    }

    request.id = root[getRequestId()].asInt();
    return Err::E_SUCCESS;
}

Err Kill::fromResponseString(std::string const & json)
{
    return Err::E_SUCCESS; // NO RESPONSE.
}

} // namespace structure
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

