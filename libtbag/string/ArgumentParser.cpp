/**
 * @file   ArgumentParser.cpp
 * @brief  ArgumentParser class implementation.
 * @author zer0
 * @date   2019-10-09
 */

#include <libtbag/string/ArgumentParser.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

std::string ArgumentParser::DefaultFormatter::print(Params const & params, Args const & args)
{
    return {};
}

// --------------
// ArgumentParser
// --------------

using ErrFlags = ArgumentParser::ErrFlags;

ArgumentParser::ArgumentParser()
{
    // EMPTY.
}

ArgumentParser::ArgumentParser(Params const & params) : _params(params)
{
    // EMPTY.
}

ArgumentParser::ArgumentParser(Params && params) : _params(std::move(params))
{
    // EMPTY.
}

ArgumentParser::ArgumentParser(ArgumentParser const & obj)
        : _params(obj._params), _args(obj._args)
{
    // EMPTY.
}

ArgumentParser::ArgumentParser(ArgumentParser && obj) TBAG_NOEXCEPT
        : _params(std::move(obj._params)), _args(std::move(obj._args))
{
    // EMPTY.
}

ArgumentParser::~ArgumentParser()
{
    // EMPTY.
}

ArgumentParser & ArgumentParser::operator =(ArgumentParser const & obj)
{
    if (this != &obj) {
        _params = obj._params;
        _args = obj._args;
    }
    return *this;
}

ArgumentParser & ArgumentParser::operator =(ArgumentParser && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _params = std::move(obj._params);
        _args = std::move(obj._args);
    }
    return *this;
}

void ArgumentParser::swap(ArgumentParser & obj) TBAG_NOEXCEPT
{
    std::swap(_params, obj._params);
    _args.swap(obj._args);
}

void ArgumentParser::clearArgs()
{
    _args.clear();
}

void ArgumentParser::addArg(Arg const & arg)
{
    _args.emplace_back(arg);
}

void ArgumentParser::addArg(Arg && arg)
{
    _args.emplace_back(std::move(arg));
}

ErrFlags ArgumentParser::parse(int argc, char ** argv, bool ignore_first)
{
    if (argc <= 0 || argv == nullptr) {
        return E_ILLARGS;
    }
    std::vector<std::string> arguments(argc);
    for (int i = 0; i < argc; ++i) {
        arguments[i] = argv[i];
    }
    return parse(arguments, ignore_first);
}

TBAG_CONSTEXPR static char const * const _ARGUMENT_DELIMITER = " ";

ErrFlags ArgumentParser::parse(std::string const & argv, bool ignore_first)
{
    return parse(libtbag::string::splitTokens(argv, _ARGUMENT_DELIMITER), ignore_first);
}

ErrFlags ArgumentParser::parse(std::vector<std::string> const & argv, bool ignore_first)
{
    if (argv.empty()) {
        return E_ILLARGS;
    }

    Flags flags;

    return { E_SUCCESS, flags };
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

