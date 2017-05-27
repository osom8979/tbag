/**
 * @file   TpotRunner.cpp
 * @brief  TpotRunner class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/TpotRunner.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR char const * const DEFAULT_HTML_BODY = R"HTML(
<html>
<head>
    <title>${title}</title>
</head>
<body>
    <h2>${title}</h2>
    <p>Now running...</p>
</body>
</html>
)HTML";

TpotRunner::TpotRunner(TpotParams const & params) : _params(params)
{
    _envs.push(EnvFlag("title", std::string(LIBTBAG_MAIN_TITLE)));
}

TpotRunner::~TpotRunner()
{
    // EMPTY.
}

int TpotRunner::run()
{
    _server.reset(new (std::nothrow) HttpServer(_loop, HttpServer::StreamType::TCP));
    assert(static_cast<bool>(_server));
    if (_server->init(_params.server_bind.c_str(), _params.server_port) == false) {
        return EXIT_FAILURE;
    }

    using namespace std::placeholders;
    _server->setOnOpen   (std::bind(&TpotRunner::onNodeOpen   , this, _1));
    _server->setOnRequest(std::bind(&TpotRunner::onNodeRequest, this, _1, _2, _3, _4));
    _server->setOnClose  (std::bind(&TpotRunner::onNodeClose  , this, _1));

    if (_params.enable_tty) {
        _stdin  = _loop.newHandle<Tty>(_loop, Tty::GeneralFile::FILE_STDIN);
        _stdout = _loop.newHandle<Tty>(_loop, Tty::GeneralFile::FILE_STDOUT);
        _stderr = _loop.newHandle<Tty>(_loop, Tty::GeneralFile::FILE_STDERR);
    }

    Err const RESULT = _loop.run();
    if (RESULT != Err::E_SUCCESS) {
        return EXIT_FAILURE;
    }

    {   // Check the last state.
        // XXXX
    }
    return EXIT_SUCCESS;
}

void TpotRunner::onNodeOpen(Node node)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeOpen() Expired client.");
        return;
    }
    tDLogN("TpotRunner::onNodeOpen(ID:{}) {}:{}", shared->getId(), shared->getDestination(), shared->getPort());
}

void TpotRunner::onNodeRequest(Err code, HttpParser const & request,
                               HttpBuilder & response, uint64_t & timeout)
{
    if (code != Err::E_SUCCESS) {
        return;
    }

    response.setStatus(200);
    response.setReason("OK");
    response.setBody(_envs.convert(std::string(DEFAULT_HTML_BODY)));
}

void TpotRunner::onNodeClose(Node node)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeOpen() Expired client.");
        return;
    }
    tDLogN("TpotRunner::onNodeClose(ID:{})", shared->getId());
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

