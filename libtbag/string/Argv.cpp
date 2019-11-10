/**
 * @file   Argv.cpp
 * @brief  Argv class implementation.
 * @author zer0
 * @date   2019-11-10
 */

#include <libtbag/string/Argv.hpp>
#include <libtbag/container/Global.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Argv::Argv()
{
    // EMPTY.
}

Argv::Argv(Argv const & obj)
        : _strings(obj._strings),
          _arguments(obj._arguments)
{
    // EMPTY.
}

Argv::Argv(Argv && obj) TBAG_NOEXCEPT
        : _strings(std::move(obj._strings)),
          _arguments(std::move(obj._arguments))
{
    // EMPTY.
}

Argv::~Argv()
{
    // EMPTY.
}

Argv & Argv::operator =(Argv const & obj)
{
    if (this != &obj) {
        _strings = obj._strings;
        _arguments = obj._arguments;
    }
    return *this;
}

Argv & Argv::operator =(Argv && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _strings = std::move(obj._strings);
        _arguments = std::move(obj._arguments);
    }
    return *this;
}

void Argv::copy(Argv const & obj)
{
    if (this != &obj) {
        _strings = obj._strings;
        _arguments = obj._arguments;
    }
}

void Argv::swap(Argv & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _strings.swap(obj._strings);
        _arguments.swap(obj._arguments);
    }
}

void Argv::clear()
{
    _strings.clear();
    _arguments.clear();
}

std::size_t Argv::getArgvSize(char const ** argv)
{
    if ((*argv) == nullptr) {
        return 0u;
    }
    std::size_t count = 0u;
    do {
        ++count;
    } while (*(argv + count) != nullptr);
    return count;
}

void Argv::parse(int argc, char const ** argv)
{
    assert(argc >= 1u);
    assert(argv != nullptr);

    _strings.resize(argc);
    _arguments.resize(argc+1);

    for (int i = 0; i < argc; ++i) {
        _strings[i] = argv[i];
        _arguments[i] = &(_strings[i][0]);
    }
    _arguments[argc] = nullptr;
}

void Argv::parse(char const ** argv)
{
    parse(getArgvSize(argv), argv);
}

void Argv::parse(std::vector<std::string> const & argv)
{
    _strings = argv;
    _arguments.resize(argv.size()+1);
    for (auto i = 0u; i < argv.size(); ++i) {
        _arguments[i] = &(_strings[i][0]);
    }
    _arguments[argv.size()] = nullptr;
}

std::weak_ptr<Argv> getMainArgv()
{
    auto weak = libtbag::container::findGlobalObject<Argv>(TBAG_MAIN_ARGV_KEY);
    if (!weak.expired()) {
        return weak;
    }

    auto shared = libtbag::container::newGlobalObject<Argv>(TBAG_MAIN_ARGV_KEY);
    if (shared) {
        return shared;
    } else {
        // Perhaps generated in another thread.
        return libtbag::container::findGlobalObject<Argv>(TBAG_MAIN_ARGV_KEY);
    }
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

