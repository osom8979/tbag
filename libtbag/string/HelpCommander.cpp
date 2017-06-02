/**
 * @file   HelpCommander.cpp
 * @brief  HelpCommander class implementation.
 * @author zer0
 * @date   2017-05-26
 */

#include <libtbag/string/HelpCommander.hpp>
#include <libtbag/log/Log.hpp>

#include <utility>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

HelpCommander::HelpCommander() : Commander()
{
    // EMPTY.
}

HelpCommander::HelpCommander(std::string const & prefix, std::string const & delimiter)
        : Commander(prefix, delimiter)
{
    // EMPTY.
}

HelpCommander::HelpCommander(Commander::Callback const & default_callback)
        : Commander(default_callback)
{
    // EMPTY.
}

HelpCommander::HelpCommander(HelpCommander const & obj) : Commander()
{
    (*this) = obj;
}

HelpCommander::HelpCommander(HelpCommander && obj) : Commander()
{
    (*this) = std::move(obj);
}

HelpCommander::~HelpCommander()
{
    // EMPTY.
}

HelpCommander & HelpCommander::operator =(HelpCommander const & obj)
{
    if (this != &obj) {
        Commander::operator=(obj);
        _synopsis = obj._synopsis;
        _remarks  = obj._remarks;
        _helps    = obj._helps;
        _params   = obj._params;
    }
    return *this;
}

HelpCommander & HelpCommander::operator =(HelpCommander && obj)
{
    if (this != &obj) {
        Commander::operator=(std::move(obj));
        _synopsis.swap(obj._synopsis);
        _remarks.swap(obj._remarks);
        _helps.swap(obj._helps);
        _params.swap(obj._params);
    }
    return *this;
}

void HelpCommander::clear()
{
    Commander::clear();
    _synopsis.clear();
    _remarks.clear();
    _helps.clear();
    _params.clear();
}

bool HelpCommander::insertHelpCommand(Callback const & callback)
{
    return insert(DEFAULT_HELP_COMMAND, callback, "Show this help message.");
}

bool HelpCommander::insert(std::string const & command, Callback const & callback, std::string const & help_msg)
{
    _helps.insert(HelpPair(command, help_msg));
    return Commander::insert(command, callback);
}

bool HelpCommander::insert(std::string const & command, Callback const & callback, std::string const & help_msg, std::string const & help_param)
{
    _helps.insert(HelpPair(command, help_msg));
    _params.insert(HelpPair(command, help_param));
    return Commander::insert(command, callback);
}

std::string HelpCommander::help(std::string const & head, bool auto_padding) const
{
    std::size_t const PADDING_SIZE = 2;

    std::stringstream ss;
    if (_synopsis.empty() == false) {
        ss << _synopsis << std::endl;
    }

    std::size_t max_size = 0;
    if (auto_padding) {
        for (auto & help : _helps) {
            std::size_t const CURSOR_SIZE = help.first.size();
            if (CURSOR_SIZE > max_size) {
                max_size = CURSOR_SIZE;
            }
        }
    }

    ss << head << std::endl;
    for (auto & cursor : _helps) {
        ss << getPrefix() << cursor.first;

        if (max_size > 0) {
            ss << std::string(max_size + PADDING_SIZE - cursor.first.size(), ' ');
        } else {
            auto param = _params.find(cursor.first);
            if (param != _params.end()) {
                ss << '=' << param->second;
            }
            ss << "\n\t";
        }
        ss << cursor.second << std::endl;
    }

    if (_remarks.empty() == false) {
        ss << _remarks << std::endl;
    }

    return ss.str();
}

std::string HelpCommander::help(bool auto_padding) const
{
    return help("Options:", auto_padding);
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

