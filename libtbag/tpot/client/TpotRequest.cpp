/**
 * @file   TpotRequest.cpp
 * @brief  TpotRequest class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/client/TpotRequest.hpp>
#include <libtbag/tpot/structure/TpotProtocol.hpp>
#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <iostream>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot   {
namespace client {

int runTpotRequest(std::string const & uri, structure::Exec const & exec)
{
    using namespace libtbag::network::http;

    std::string body;
    if (exec.toRequestJsonString(body) != Err::E_SUCCESS) {
        std::cout << "Json convert error.\n";
        return EXIT_FAILURE;
    }

    HttpRequest request;
    request.insertHeader(exec.getAcceptKey(), exec.getAcceptValue());
    request.method = exec.getMethod();
    request.body = body;

    uint64_t const TIMEOUT = 10000;
    std::string const UPDATE_URI = network::Uri(uri).getUrl() + structure::Exec::getPath();
    std::cout << "Request URI: " << UPDATE_URI << std::endl;
    std::cout << "Timeout: " << TIMEOUT << "ms\n";

    HttpResponse response;
    if (requestWithSync(UPDATE_URI, request, TIMEOUT, response) != Err::E_SUCCESS) {
        std::cout << "Request error.\n";
        return EXIT_FAILURE;
    }

    std::cout << "> Status code: " << response.status << std::endl;
    std::cout << "> Body: " << response.body << std::endl;
    return EXIT_SUCCESS;
}

int runTpotRequest(std::string const & uri,
                   std::string const & file,
                   std::string const & cwd,
                   std::vector<std::string> const & args,
                   std::vector<std::string> const & envs,
                   uint32_t uid, uint32_t gid)
{
    structure::Exec exec;
    exec.request.file = file;
    exec.request.cwd  = cwd;
    exec.request.args = args;
    exec.request.envs = envs;
    exec.request.uid  = uid;
    exec.request.gid  = gid;
    return runTpotRequest(uri, exec);
}

int runTpotRequest()
{
    std::string uri;
    std::string file;
    std::string cwd;
    std::string args;
    std::string envs;
    std::string yea_or_no;

    while (true) {
        uri.clear();
        file.clear();
        cwd.clear();
        args.clear();
        envs.clear();
        yea_or_no.clear();

        std::cout << "TpoT Server URI: ";
        std::cin.clear();
        std::getline(std::cin, uri);

        std::cout << "File path: ";
        std::cin.clear();
        std::getline(std::cin, file);

        std::cout << "Working directory: ";
        std::cin.clear();
        std::getline(std::cin, cwd);

        std::cout << "Arguments (VAL1 VAL2 ...): ";
        std::cin.clear();
        std::getline(std::cin, args);

        std::cout << "Environment (KEY1=VAL1 KEY2=VAL2 ...): ";
        std::cin.clear();
        std::getline(std::cin, envs);

        std::cout << "Result:\n"
                << "> URI: "  << uri  << std::endl
                << "> File: " << file << std::endl
                << "> CWD: "  << cwd  << std::endl
                << "> ARGS: " << args << std::endl
                << "> ENVS: " << envs << std::endl;

        std::cout << "Continue? [y/n/q]: ";
        std::cin.clear();
        std::getline(std::cin, yea_or_no);

        if (yea_or_no.size() == 1) {
            if (yea_or_no[0] == 'y' || yea_or_no[0] == 'Y') {
                break;
            } else if (yea_or_no[0] == 'q' || yea_or_no[0] == 'Q') {
                std::cout << "Cancel request.\n";
                return EXIT_FAILURE;
            }
        }
    }

    std::string const DELIMITER = " ";
    return runTpotRequest(uri, file, cwd,
                          string::splitTokens(args, DELIMITER),
                          string::splitTokens(envs, DELIMITER),
                          0, 0);
}

int runTpotRequestWithTest()
{
    std::string const TEST = "TPOT_REQUEST_TEST";

    std::string const URI  = "http://localhost:2100";
    std::string const FILE = "${EXE_DIR}/tbproc";
    std::string const CWD  = "${EXE_DIR}";
    std::vector<std::string> const ARGS = {"file", TEST};
    std::vector<std::string> const ENVS;
    uint32_t uid = 0;
    uint32_t gid = 0;

    int const EXIT_CODE = runTpotRequest(URI, FILE, CWD, ARGS, ENVS);
    if (EXIT_CODE != EXIT_SUCCESS) {
        std::cerr << "Exit error (" << EXIT_CODE << ")\n";
        return EXIT_FAILURE;
    }

    std::cout << "Success! check the tbproc.txt file.\n";
    return EXIT_SUCCESS;
}

} // namespace client
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

