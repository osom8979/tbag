/**
 * @file   Flags.cpp
 * @brief  Flags class implementation.
 * @author zer0
 * @date   2016-10-29
 */

#include <libtbag/string/Flags.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Flags::Flags()
{
    // EMPTY.
}

Flags::Flags(int argc, char ** argv)
{
    parse(argc, argv);
}

Flags::Flags(std::string const & args, std::string const & prefix, std::string const & delimiter)
{
    parse(args, prefix, delimiter);
}

Flags::Flags(std::string const & args)
{
    parse(args);
}

Flags::Flags(Flags const & obj)
{
    (*this) = obj;
}

Flags::Flags(Flags && obj)
{
    (*this) = std::move(obj);
}

Flags::~Flags()
{
    // EMPTY.
}

Flags & Flags::operator =(Flags const & obj)
{
    if (this != &obj) {
        _flags = obj._flags;
    }
    return *this;
}

Flags & Flags::operator =(Flags && obj)
{
    if (this != &obj) {
        _flags.swap(obj._flags);
    }
    return *this;
}

void Flags::swap(Flags & obj)
{
    _flags.swap(obj._flags);
}

Flags::Flag Flags::find(FlagVector::const_iterator itr) const
{
    if (itr == _flags.end()) {
        return Flag();
    }
    return *itr;
}

Flags::Flag Flags::findWithKey(std::string const & key) const
{
    return find(std::find_if(_flags.begin(), _flags.end(), [&key](Flag const & flag) -> bool {
        return (flag.key == key);
    }));
}

Flags::Flag Flags::findWithValue(std::string const & value) const
{
    return find(std::find_if(_flags.begin(), _flags.end(), [&value](Flag const & flag) -> bool {
        return (flag.value == value);
    }));
}

bool Flags::remove(std::size_t index)
{
    if (index < _flags.size()) {
        _flags.erase(_flags.cbegin() + index);
        return true;
    }
    return false;
}

bool Flags::remove(std::string const & key)
{
    auto itr = std::find_if(_flags.cbegin(), _flags.cend(), [key](Flag const & f) -> bool { return f.key == key; });
    if (itr != _flags.end()) {
        _flags.erase(itr);
        return true;
    }
    return false;
}

bool Flags::existsWithKey(std::string const & key) const
{
    return !findWithKey(key).key.empty();
}

bool Flags::existsWithValue(std::string const & value) const
{
    return !findWithValue(value).value.empty();
}

std::vector<std::string> Flags::getUnnamedValues() const
{
    std::vector<std::string> result;
    for (auto & cursor : _flags) {
        if (cursor.key == "" && cursor.value.empty() == false) {
            result.push_back(cursor.value);
        }
    }
    return result;
}

bool Flags::parse(int argc, char ** argv)
{
    return parse(argc, argv, DEFAULT_PREFIX, DEFAULT_DELIMITER);
}

bool Flags::parse(int argc, char ** argv, std::string const & prefix, std::string const & delimiter)
{
    for (int index = 0; index < argc; ++index) {
        push(convertFlag(std::string(argv[index]), prefix, delimiter));
    }
    return true;
}

bool Flags::parse(std::string const & args)
{
    return parse(args, DEFAULT_PREFIX, DEFAULT_DELIMITER);
}

bool Flags::parse(std::string const & args, std::string const & prefix, std::string const & delimiter)
{
    return parse(splitTokens(args), prefix, delimiter);
}

bool Flags::parse(std::vector<std::string> const & args, std::string const & prefix, std::string const & delimiter)
{
    for (auto & cursor : args) {
        push(convertFlag(cursor, prefix, delimiter));
    }
    return true;
}

Flags::Argv Flags::getArgv(std::string const & prefix, std::string const & delimiter, bool last_null) const
{
    std::size_t size = _flags.size();
    if (last_null) {
        ++size;
    }

    Argv argv;
    argv._strings.resize(size);
    argv._arguments.resize(size);

    for (std::size_t i = 0; i < size; ++i) {
        argv._strings  [i] = convertString(_flags[i], prefix, delimiter);
        argv._arguments[i] = &argv._strings[i][0];
    }

    if (last_null) {
        assert(size >= 1);
        argv._arguments[size - 1] = nullptr;
    }
    return argv;
}

Flags::Argv Flags::getArgv(bool last_null) const
{
    return getArgv(DEFAULT_PREFIX, DEFAULT_DELIMITER, last_null);
}

Flags::Flag Flags::convertFlag(std::string const & str, std::string const & prefix, std::string const & delimiter)
{
    if (str.substr(0, prefix.size()) == prefix) {
        // ENABLE KEY.
        std::size_t delimiter_pos = str.find(delimiter);
        std::string key = str.substr(prefix.size(), delimiter_pos - prefix.size());

        if (delimiter_pos == std::string::npos) {
            // ONLY KEY.
            return Flag(key);
        } else {
            // KEY & VALUE.
            std::string value = str.substr(delimiter_pos + 1);
            return Flag(key, value);
        }
    }

    // ONLY VALUE.
    return Flag(std::string(), std::string(str));
}

Flags::Flag Flags::convertFlag(std::string const & str)
{
    return convertFlag(str, DEFAULT_PREFIX, DEFAULT_DELIMITER);
}

std::string Flags::convertString(Flag const & flag, std::string const & prefix, std::string const & delimiter)
{
    if (flag.key.empty()) {
        return flag.value;
    }
    if (flag.value.empty()) {
        return prefix + flag.key;
    }
    return prefix + flag.key + delimiter + flag.value;
}

std::string Flags::convertString(Flag const & flag)
{
    return convertString(flag, DEFAULT_PREFIX, DEFAULT_DELIMITER);
}

std::vector<std::string> Flags::splitTokens(std::string const & args)
{
    std::string trim_right_args = trimRight(args);
    std::size_t args_size = trim_right_args.size();
    std::size_t all_process_count     = 0U;
    std::size_t current_process_count = 0U;

    std::vector<std::string> result;

    while (all_process_count < args_size) {
        result.push_back(splitFirst(trim_right_args.substr(all_process_count), &current_process_count));
        all_process_count += current_process_count;
    }

    return result;
}

std::string Flags::splitFirst(std::string const & args, std::size_t * process_count)
{
    std::string trim_left_args = trimLeft(args);
    if (trim_left_args.empty()) {
        return std::string();
    }

    std::string result;

    auto itr = trim_left_args.begin();

    char quotation_mark;
    if (*itr == DOUBLE_QUOTES) {
        quotation_mark = DOUBLE_QUOTES;
    } else if (*itr == SINGLE_QUOTES) {
        quotation_mark = SINGLE_QUOTES;
    } else {
        quotation_mark = SPACE_QUOTES;
        result.push_back(*itr);
    }
    ++itr;

    bool force_push_back = false;
    for (; itr != trim_left_args.end(); ++itr) {
        if (force_push_back) {
            result.push_back(*itr);
            force_push_back = false;
        } else {
            if (*itr == ESCAPE) {
                // Skip, escape character (\).
                force_push_back = true;
            } else if (*itr == quotation_mark) {
                break;
            } else {
                result.push_back(*itr);
            }
        }
    }

    if (process_count != nullptr) {
        *process_count = (args.size() - trim_left_args.size())      // Trim size.
                       + std::distance(trim_left_args.begin(), itr) // Size of string processing.
                       + 1; // Last, quotation_mark size.
    }

    return result;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

