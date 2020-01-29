/**
 * @file   Flags.cpp
 * @brief  Flags class implementation.
 * @author zer0
 * @date   2016-10-29
 */

#include <libtbag/string/Flags.hpp>
#include <cassert>
#include <algorithm>

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
    if (!parse(argc, argv)) {
        throw std::bad_alloc();
    }
}

Flags::Flags(std::string const & args, std::string const & prefix, std::string const & delimiter)
{
    if (!parse(args, prefix, delimiter)) {
        throw std::bad_alloc();
    }
}

Flags::Flags(std::string const & args)
{
    if (!parse(args)) {
        throw std::bad_alloc();
    }
}

Flags::Flags(std::vector<std::string> const & args, std::string const & prefix, std::string const & delimiter)
{
    if (!parse(args, prefix, delimiter)) {
        throw std::bad_alloc();
    }
}

Flags::Flags(std::vector<std::string> const & args)
{
    if (!parse(args)) {
        throw std::bad_alloc();
    }
}

Flags::Flags(Flags const & obj) : _flags(obj._flags)
{
    // EMPTY.
}

Flags::Flags(Flags && obj) TBAG_NOEXCEPT : _flags(std::move(obj._flags))
{
    // EMPTY.
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

Flags & Flags::operator =(Flags && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _flags.swap(obj._flags);
    }
    return *this;
}

void Flags::swap(Flags & obj) TBAG_NOEXCEPT
{
    _flags.swap(obj._flags);
}

Flags::Flag Flags::findByKey(std::string const & key) const
{
    auto const itr = std::find_if(_flags.cbegin(), _flags.cend(), [&key](Flag const & flag) -> bool {
        return flag.key == key;
    });
    if (itr == _flags.cend()) {
        return Flag();
    }
    return *itr;
}

Flags::Flag Flags::findByValue(std::string const & value) const
{
    auto const itr = std::find_if(_flags.cbegin(), _flags.cend(), [&value](Flag const & flag) -> bool {
        return flag.value == value;
    });
    if (itr == _flags.cend()) {
        return Flag();
    }
    return *itr;
}

bool Flags::existsByKey(std::string const & key) const
{
    auto const itr = std::find_if(_flags.cbegin(), _flags.cend(), [&](Flag const & f) -> bool {
        return f.key == key;
    });
    return itr != _flags.cend();
}

bool Flags::existsByValue(std::string const & value) const
{
    auto const itr = std::find_if(_flags.cbegin(), _flags.cend(), [&](Flag const & f) -> bool {
        return f.value == value;
    });
    return itr != _flags.cend();
}

void Flags::set(std::string const & key, std::string const & val)
{
    auto itr = std::find_if(_flags.begin(), _flags.end(), [&key](Flag const & flag) -> bool {
        return flag.key == key;
    });
    if (itr == _flags.end()) {
        push(key, val);
    } else {
        itr->value = val;
    }
}

bool Flags::get(std::string const & key, std::string & val) const
{
    auto const itr = std::find_if(_flags.cbegin(), _flags.cend(), [&key](Flag const & flag) -> bool {
        return flag.key == key;
    });
    if (itr != _flags.cend()) {
        val = itr->value;
        return true;
    }
    return false;
}

std::string Flags::get(std::string const & key) const
{
    std::string val;
    if (get(key, val)) {
        return val;
    }
    return {};
}

std::string Flags::opt(std::string const & key, std::string const & default_val) const
{
    std::string value;
    if (get(key, value)) {
        return value;
    }
    return default_val;
}

std::string Flags::opt(std::string const & key) const
{
    return opt(key, {});
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
    auto const itr = std::find_if(_flags.cbegin(), _flags.cend(), [&](Flag const & f) -> bool {
        return f.key == key;
    });
    if (itr != _flags.cend()) {
        _flags.erase(itr);
        return true;
    }
    return false;
}

std::vector<std::string> Flags::keys() const
{
    std::vector<std::string> result;
    for (auto const & flag : _flags) {
        if (!flag.key.empty()) {
            result.emplace_back(flag.key);
        }
    }
    return result;
}

std::vector<std::string> Flags::values() const
{
    std::vector<std::string> result;
    for (auto const & flag : _flags) {
        if (!flag.key.empty()) {
            result.emplace_back(flag.value);
        }
    }
    return result;
}

std::vector<std::string> Flags::getUnnamedValues() const
{
    std::vector<std::string> result;
    for (auto const & cursor : _flags) {
        if (cursor.key.empty() && !cursor.value.empty()) {
            result.emplace_back(cursor.value);
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
        push(convertStringToFlag(std::string(argv[index]), prefix, delimiter));
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

bool Flags::parse(std::vector<std::string> const & args)
{
    return parse(args, DEFAULT_PREFIX, DEFAULT_DELIMITER);
}

bool Flags::parse(std::vector<std::string> const & args, std::string const & prefix, std::string const & delimiter)
{
    for (auto const & cursor : args) {
        push(convertStringToFlag(cursor, prefix, delimiter));
    }
    return true;
}

Argv Flags::argv(std::string const & prefix, std::string const & delimiter) const
{
    std::vector<std::string> strings(_flags.size());
    for (auto i = 0u; i < _flags.size(); ++i) {
        strings[i] = convertFlagToString(_flags[i], prefix, delimiter);
    }
    return Argv(strings);
}

Argv Flags::argv() const
{
    return argv(DEFAULT_PREFIX, DEFAULT_DELIMITER);
}

Flags::Flag Flags::convertStringToFlag(std::string const & str, std::string const & prefix, std::string const & delimiter)
{
    if (str != prefix && str.substr(0, prefix.size()) == prefix) {
        auto const delimiter_pos = str.find(delimiter);
        auto const key = str.substr(prefix.size(), delimiter_pos - prefix.size());
        if (delimiter_pos == std::string::npos) {
            return Flag(key); // Key only.
        } else {
            return Flag(key, str.substr(delimiter_pos+1)); // Key and Value.
        }
    }
    return Flag(std::string(), std::string(str)); // Value only.
}

Flags::Flag Flags::convertStringToFlag(std::string const & str)
{
    return convertStringToFlag(str, DEFAULT_PREFIX, DEFAULT_DELIMITER);
}

std::string Flags::convertFlagToString(Flag const & flag, std::string const & prefix, std::string const & delimiter)
{
    if (flag.key.empty()) {
        return flag.value;
    }
    if (flag.value.empty()) {
        return prefix + flag.key;
    }
    return prefix + flag.key + delimiter + flag.value;
}

std::string Flags::convertFlagToString(Flag const & flag)
{
    return convertFlagToString(flag, DEFAULT_PREFIX, DEFAULT_DELIMITER);
}

std::vector<std::string> Flags::splitTokens(std::string const & args)
{
    auto trim_right_args = trimRight(args);
    auto const args_size = trim_right_args.size();
    std::size_t all_process_count = 0u;
    std::size_t current_process_count = 0u;

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

    auto itr = trim_left_args.begin();
    char quotation_mark;
    std::string result;

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
        *process_count = (args.size()-trim_left_args.size()) // Trim size.
                       + std::distance(trim_left_args.begin(), itr) // Size of string processing.
                       + 1; // Last, quotation_mark size.
    }

    return result;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

