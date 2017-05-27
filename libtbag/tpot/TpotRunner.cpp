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

TpotRunner::TpotRunner(TpotParams const & params) : _params(params)
{
    // EMPTY.
}

TpotRunner::~TpotRunner()
{
    // EMPTY.
}

int TpotRunner::run()
{
    _server.reset(new (std::nothrow) HttpServer(_loop));
    assert(static_cast<bool>(_server));
    if (_server->init(_params.server_bind.c_str(), _params.server_port) == false) {
        return EXIT_FAILURE;
    }

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

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

