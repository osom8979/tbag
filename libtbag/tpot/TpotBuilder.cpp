/**
 * @file   TpotBuilder.cpp
 * @brief  TpotBuilder class implementation.
 * @author zer0
 * @date   2017-05-14
 */

// Cap'n-proto result.
#include <libtbag/3rd/kj/string.h>
#include <libtbag/3rd/capnp/message.h>
#include <libtbag/3rd/capnp/serialize-packed.h>
#include <libtbag/tpot/struct/Protocol.capnp.h>

#include <libtbag/tpot/TpotBuilder.hpp>
#include <libtbag/log/Log.hpp>

#include <iostream>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

// ---------------
namespace __impl {
// ---------------

void setHeader(::Header::Builder & builder, TpotBuilder::Header const & header)
{
    builder.setRequest(header.request);
}

// ------------------
} // namespace __impl
// ------------------

TpotBuilder::TpotBuilder()
{
    // EMPTY.
}

TpotBuilder::~TpotBuilder()
{
    // EMPTY.
}

TpotBuilder::Code TpotBuilder::build(Header const & header, Buffer & buffer)
{
    capnp::MallocMessageBuilder builder;
    Packet::Builder packet = builder.initRoot<Packet>();

    auto h = packet.getHeader();
    __impl::setHeader(h, header);
    packet.getBody().setUnemployed();

    kj::VectorOutputStream output;
    capnp::writePackedMessage(output, builder);
    buffer.assign(output.getArray().begin(), output.getArray().end());
    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::build(Header const & header, Version const & body, Buffer & buffer)
{
    capnp::MallocMessageBuilder builder;
    Packet::Builder packet = builder.initRoot<Packet>();

    auto h = packet.getHeader();
    __impl::setHeader(h, header);

    auto v = packet.getBody().initVersion();
    v.setMajor(body.major);
    v.setMinor(body.minor);
    v.setPatch(body.patch);

    kj::VectorOutputStream output;
    capnp::writePackedMessage(output, builder);
    buffer.assign(output.getArray().begin(), output.getArray().end());
    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::build(Header const & header, Command const & body, Buffer & buffer)
{
    capnp::MallocMessageBuilder builder;
    Packet::Builder packet = builder.initRoot<Packet>();

    auto h = packet.getHeader();
    __impl::setHeader(h, header);

    auto c = packet.getBody().initCommand();
    unsigned int const ARGS_SIZE = static_cast<unsigned int>(body.args.size());
    unsigned int const ENVS_SIZE = static_cast<unsigned int>(body.envs.size());
    c.setExec(body.exec.c_str());
    c.setCwd(body.cwd.c_str());

    auto args = c.initArgs(ARGS_SIZE);
    auto envs = c.initEnvs(ENVS_SIZE);

    unsigned int index = 0;
    for (index = 0; index < ARGS_SIZE; ++index) {
        args.set(index, capnp::Text::Reader(body.args[index]));
    }
    for (index = 0; index < ENVS_SIZE; ++index) {
        envs[index].setKey(body.envs[index].key);
        envs[index].setValue(body.envs[index].value);
    }

    kj::VectorOutputStream output;
    capnp::writePackedMessage(output, builder);
    buffer.assign(output.getArray().begin(), output.getArray().end());
    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::build(Header const & header, Script const & body, Buffer & buffer)
{
    capnp::MallocMessageBuilder builder;
    Packet::Builder packet = builder.initRoot<Packet>();

    auto h = packet.getHeader();
    __impl::setHeader(h, header);

    auto s = packet.getBody().initScript();
    s.setMime(body.mime.c_str());
    s.setBody(body.body.c_str());

    kj::VectorOutputStream output;
    capnp::writePackedMessage(output, builder);
    buffer.assign(output.getArray().begin(), output.getArray().end());
    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::build(Header const & header, Message const & body, Buffer & buffer)
{
    capnp::MallocMessageBuilder builder;
    Packet::Builder packet = builder.initRoot<Packet>();

    auto h = packet.getHeader();
    __impl::setHeader(h, header);

    auto err = packet.getBody().initMessage();
    err.setMsg(body.msg.c_str());

    kj::VectorOutputStream output;
    capnp::writePackedMessage(output, builder);
    buffer.assign(output.getArray().begin(), output.getArray().end());
    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::parse(Buffer const & buffer, Header & header, Type & type)
{
    kj::ArrayPtr<char const> ptr(buffer.data(), buffer.size());
    kj::ArrayInputStream input(ptr.asBytes());

    // std::size_t const SCRATCH_SIZE = 2048;
    // capnp::word scratch[SCRATCH_SIZE];
    // capnp::PackedMessageReader reader(
    //         input, capnp::ReaderOptions(),
    //         kj::ArrayPtr<capnp::word>(scratch, SCRATCH_SIZE));
    capnp::PackedMessageReader reader(input);

    Packet::Reader packet = reader.getRoot<Packet>();
    header.request = packet.getHeader().getRequest();
    switch (packet.getBody().which()) {
    case ::Packet::Body::Which::UNEMPLOYED: type = Type::UNEMPLOYED; break;
    case ::Packet::Body::Which::VERSION:    type = Type::VERSION;    break;
    case ::Packet::Body::Which::COMMAND:    type = Type::COMMAND;    break;
    case ::Packet::Body::Which::SCRIPT:     type = Type::SCRIPT;     break;
    case ::Packet::Body::Which::MESSAGE:    type = Type::MESSAGE;    break;
    }
    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::parse(Buffer const & buffer, Version & body)
{
    kj::ArrayPtr<char const> ptr(buffer.data(), buffer.size());
    kj::ArrayInputStream input(ptr.asBytes());
    capnp::PackedMessageReader reader(input);

    Packet::Reader packet = reader.getRoot<Packet>();
    auto v = packet.getBody().getVersion();
    body.major = v.getMajor();
    body.minor = v.getMinor();
    body.patch = v.getPatch();
    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::parse(Buffer const & buffer, Command & body)
{
    kj::ArrayPtr<char const> ptr(buffer.data(), buffer.size());
    kj::ArrayInputStream input(ptr.asBytes());
    capnp::PackedMessageReader reader(input);

    Packet::Reader packet = reader.getRoot<Packet>();
    auto c = packet.getBody().getCommand();
    body.exec.assign(c.getExec().begin(), c.getExec().end());
    body.cwd.assign(c.getCwd().begin(), c.getCwd().end());

    //std::size_t ARGS_SIZE = body.args.size();
    //std::size_t ENVS_SIZE = body.envs.size();

    for (auto arg : c.getArgs()) {
        body.args.push_back(arg);
    }
    for (auto env : c.getEnvs()) {
        body.envs.push_back(TpotBuilder::Env{env.getKey(), env.getValue()});
    }

    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::parse(Buffer const & buffer, Script & body)
{
    kj::ArrayPtr<char const> ptr(buffer.data(), buffer.size());
    kj::ArrayInputStream input(ptr.asBytes());
    capnp::PackedMessageReader reader(input);

    Packet::Reader packet = reader.getRoot<Packet>();
    auto s = packet.getBody().getScript();
    body.mime.assign(s.getMime().begin(), s.getMime().end());
    body.body.assign(s.getBody().begin(), s.getBody().end());
    return Code::SUCCESS;
}

TpotBuilder::Code TpotBuilder::parse(Buffer const & buffer, Message & body)
{
    kj::ArrayPtr<char const> ptr(buffer.data(), buffer.size());
    kj::ArrayInputStream input(ptr.asBytes());
    capnp::PackedMessageReader reader(input);

    Packet::Reader packet = reader.getRoot<Packet>();
    auto err = packet.getBody().getMessage();
    body.msg = err.getMsg();
    return Code::SUCCESS;
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

