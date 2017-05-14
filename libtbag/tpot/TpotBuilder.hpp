/**
 * @file   TpotBuilder.hpp
 * @brief  TpotBuilder class prototype.
 * @author zer0
 * @date   2017-05-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTBUILDER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTBUILDER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdint>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotBuilder class prototype.
 *
 * @author zer0
 * @date   2017-05-14
 */
class TBAG_API TpotBuilder : public Noncopyable
{
public:
    using Buffer = std::vector<char>;

public:
    struct Header
    {
        uint64_t request;
    };

    struct Version
    {
        uint16_t major;
        uint16_t minor;
        uint16_t patch;
    };

    struct Env
    {
        std::string key;
        std::string value;
    };

    struct Command
    {
        std::string exec;
        std::string cwd;
        std::vector<std::string> args;
        std::vector<Env> envs;
    };

    struct Script
    {
        std::string mime;
        std::string body;
    };

    struct Error
    {
        uint32_t id;
        std::string msg;
    };

    enum class Type : int
    {
        UNEMPLOYED,
        VERSION,
        COMMAND,
        SCRIPT,
        ERROR,
    };

    enum class Code : int
    {
        SUCCESS,
        UNKNOWN,
        FAILURE,
    };

public:
    TpotBuilder();
    virtual ~TpotBuilder();

public:
    Code build(Header const & header, Buffer & buffer);
    Code build(Header const & header, Version const & body, Buffer & buffer);
    Code build(Header const & header, Command const & body, Buffer & buffer);
    Code build(Header const & header, Script  const & body, Buffer & buffer);
    Code build(Header const & header, Error   const & body, Buffer & buffer);

public:
    Code parse(Buffer const & buffer, Header & header, Type & type);

public:
    Code parse(Buffer const & buffer, Version & body);
    Code parse(Buffer const & buffer, Command & body);
    Code parse(Buffer const & buffer, Script  & body);
    Code parse(Buffer const & buffer, Error   & body);
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTBUILDER_HPP__

