/**
 * @file   Arguments.cpp
 * @brief  Arguments class implementation.
 * @author zer0
 * @date   2016-10-29
 */

#include <libtbag/string/Arguments.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/Exception.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Arguments::Arguments()
        : _original(), _name(), _args(),
          _delimiter(DEFAULT_ARGUMENTS_DELIMITER),
          _point_delimiter(DEFAULT_ARGUMENTS_POINT_DELIMITER),
          _full()
{
    // EMPTY.
}

Arguments::Arguments(std::string const & name
                   , std::string const & arguments
                   , std::string const & delimiter
                   , std::string const & point_delimiter)
        : _original(), _name(name), _args(),
          _delimiter(delimiter), _point_delimiter(point_delimiter),
          _full()
{
    if (parse(arguments) == false) {
        tDLogE("Arguments::Arguments() parse error.");
    }
}

Arguments::Arguments(Arguments const & obj)
{
    (*this) = obj;
}

Arguments::Arguments(Arguments && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

Arguments::~Arguments()
{
    // EMPTY.
}

Arguments & Arguments::operator =(Arguments const & obj)
{
    if (this != &obj) {
        _original = obj._original;
        _name = obj._name;
        _args = obj._args;
        _delimiter = obj._delimiter;
        _point_delimiter = obj._point_delimiter;
        _full = obj._full;
    }
    return *this;
}

Arguments & Arguments::operator =(Arguments && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _original.swap(obj._original);
        _name.swap(obj._name);
        _args.swap(obj._args);
        _delimiter.swap(obj._delimiter);
        _point_delimiter.swap(obj._point_delimiter);
        _full.swap(obj._full);
    }
    return *this;
}

void Arguments::insert(std::size_t index, std::string const & argument)
{
    _args.insert(_args.begin() + index, argument);
}

bool Arguments::parse(std::string const & arguments)
{
    for (auto & cursor : libtbag::string::splitTokens(arguments, _delimiter)) {
        push(cursor);
    }
    return true;
}

std::string Arguments::toString()
{
    if (_args.empty()) {
        return std::string();
    } else if (_args.size() == 1) {
        return _args.at(0);
    }

    std::size_t const SIZE = _args.size();
    std::string result = _args[0];
    for (std::size_t index = 1; index < SIZE; ++index) {
        result += _delimiter + _args.at(index);
    }
    return result;
}

bool Arguments::optBoolean(std::size_t index, bool * output, bool check_grammar) const
{
    return tryObtainArgument(index, output, [this, check_grammar](std::string const & value){
        if (check_grammar && value.find(this->_point_delimiter) != std::string::npos) {
            throw ParseException();
        }
        std::string const UPPER = string::upper(string::trim(value));
        if (UPPER == DEFAULT_BOOLEAN_TRUE_CASE1 ) { return  true; }
        if (UPPER == DEFAULT_BOOLEAN_TRUE_CASE2 ) { return  true; }
        if (UPPER == DEFAULT_BOOLEAN_TRUE_CASE3 ) { return  true; }
        if (UPPER == DEFAULT_BOOLEAN_FALSE_CASE1) { return false; }
        if (UPPER == DEFAULT_BOOLEAN_FALSE_CASE2) { return false; }
        if (UPPER == DEFAULT_BOOLEAN_FALSE_CASE3) { return false; }
        return std::stoi(value) != 0 ? true : false;
    });
}

#ifndef __TBAG_ARGUMENTS_OBTAIN_TRY
#define __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, param_name, retval) \
    return tryObtainArgument(index, output, [this, check_grammar](std::string const & param_name){ \
        if (check_grammar && param_name.find(this->_point_delimiter) != std::string::npos) { \
            throw ParseException(); \
        } \
        return retval; \
    });
#endif

bool Arguments::optChar(std::size_t index, char * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, static_cast<char>(std::stoi(value))); }

bool Arguments::optUnsignedChar(std::size_t index, unsigned char * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, static_cast<unsigned char>(std::stoi(value))); }

bool Arguments::optShort(std::size_t index, short * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, static_cast<short>(std::stoi(value))); }

bool Arguments::optUnsignedShort(std::size_t index, unsigned short * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, static_cast<unsigned short>(std::stoi(value))); }

bool Arguments::optInteger(std::size_t index, int * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, std::stoi(value)); }

bool Arguments::optLong(std::size_t index, long * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, std::stol(value)); }

bool Arguments::optUnsignedLong(std::size_t index, unsigned long * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, std::stoul(value)); }

bool Arguments::optLongLong(std::size_t index, long long * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, std::stoll(value)); }

bool Arguments::optUnsignedLongLong(std::size_t index, unsigned long long * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, std::stoull(value)); }

bool Arguments::optFloat(std::size_t index, float * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, std::stof(value)); }

bool Arguments::optDouble(std::size_t index, double * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, std::stod(value)); }

bool Arguments::optLongDouble(std::size_t index, long double * output, bool check_grammar) const
{ __TBAG_ARGUMENTS_OBTAIN_TRY(index, output, check_grammar, value, std::stold(value)); }

#undef __TBAG_ARGUMENTS_OBTAIN_TRY

bool Arguments::optString(std::size_t index, std::string * output) const
{
    return tryObtainArgument(index, output, [](std::string const & value){
        return value;
    });
}

bool Arguments::optIntegerPoint(std::size_t index, Pointi * output, bool check_grammar) const
{
    return tryObtainTokens(index, output, [check_grammar](std::vector<std::string> const & tokens) -> Pointi {
        if (check_grammar && tokens.size() != 2) {
            throw ParseException();
        }
        return Pointi(std::stoi(tokens.at(0)), std::stoi(tokens.at(1)));
    });
}

bool Arguments::optDoublePoint(std::size_t index, Pointd * output, bool check_grammar) const
{
    return tryObtainTokens(index, output, [check_grammar](std::vector<std::string> const & tokens) -> Pointd {
        if (check_grammar && tokens.size() != 2) {
            throw ParseException();
        }
        return Pointd(std::stod(tokens.at(0)), std::stod(tokens.at(1)));
    });
}

bool Arguments::optIntegerRect(std::size_t index, Recti * output, bool check_grammar) const
{
    return tryObtainTokens(index, output, [check_grammar](std::vector<std::string> const & tokens) -> Recti {
        if (check_grammar && tokens.size() != 4) {
            throw ParseException();
        }
        return Recti(std::stoi(tokens.at(0)), std::stoi(tokens.at(1)), std::stoi(tokens.at(2)), std::stoi(tokens.at(3)));
    });
}

bool Arguments::optDoubleRect(std::size_t index, Rectd * output, bool check_grammar) const
{
    return tryObtainTokens(index, output, [check_grammar](std::vector<std::string> const & tokens) -> Rectd {
        if (check_grammar && tokens.size() != 4) {
            throw ParseException();
        }
        return Rectd(std::stod(tokens.at(0)), std::stod(tokens.at(1)), std::stod(tokens.at(2)), std::stod(tokens.at(3)));
    });
}

std::vector<std::string> Arguments::getStrings() const
{
    return _args;
}

std::vector<int> Arguments::getIntegers() const
{
    return getVector<int>([this](std::size_t index, int * output){
        return this->optInteger(index, output);
    });
}

std::vector<double> Arguments::getDoubles() const
{
    return getVector<double>([this](std::size_t index, double * output){
        return this->optDouble(index, output);
    });
}

std::vector<Arguments::Pointi> Arguments::getIntegerPoints() const
{
    return getVector<Pointi>([this](std::size_t index, Pointi * output){
        return this->optIntegerPoint(index, output);
    });
}

std::vector<Arguments::Pointd> Arguments::getDoublePoints() const
{
    return getVector<Pointd>([this](std::size_t index, Pointd * output){
        return this->optDoublePoint(index, output);
    });
}

std::vector<Arguments::Recti> Arguments::getIntegerRects() const
{
    return getVector<Recti>([this](std::size_t index, Recti * output){
        return this->optIntegerRect(index, output);
    });
}

std::vector<Arguments::Rectd> Arguments::getDoubleRects() const
{
    return getVector<Rectd>([this](std::size_t index, Rectd * output){
        return this->optDoubleRect(index, output);
    });
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

