/**
 * @file   Arguments.cpp
 * @brief  Arguments class implementation.
 * @author zer0
 * @date   2016-10-29
 */

#include <libtbag/string/Arguments.hpp>
#include <libtbag/Exception.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Arguments::Arguments()
        : _name()
        , _args()
        , _delimiter(DEFAULT_ARGUMENTS_DELIMITER)
        , _point_delimiter(DEFAULT_ARGUMENTS_POINT_DELIMITER)
{
    // EMPTY.
}

Arguments::Arguments(std::string const & name
                   , std::string const & arguments
                   , std::string const & delimiter
                   , std::string const & point_delimiter)
        : _name(name)
        , _args()
        , _delimiter(delimiter)
        , _point_delimiter(point_delimiter)
{
    if (parse(arguments) == false) {
        tDLogE("Arguments::Arguments() parse error.");
    }
}

Arguments::Arguments(Arguments const & obj)
{
    (*this) = obj;
}

Arguments::Arguments(Arguments && obj)
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
        _name = obj._name;
        _args = obj._args;
        _delimiter = obj._delimiter;
        _point_delimiter = obj._point_delimiter;
    }
    return *this;
}

Arguments & Arguments::operator =(Arguments && obj)
{
    if (this != &obj) {
        _name.swap(obj._name);
        _args.swap(obj._args);
        _delimiter.swap(obj._delimiter);
        _point_delimiter.swap(obj._point_delimiter);
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
    std::string result = _args.at(0);
    for (std::size_t index = 1; index < SIZE; ++index) {
        result += _delimiter + _args.at(index);
    }
    return result;
}

bool Arguments::optInteger(std::size_t index, int * output, bool check_grammar) const
{
    return tryObtainArgument(index, output, [this, check_grammar](std::string const & value){
        if (check_grammar && value.find(this->_point_delimiter) != std::string::npos) {
            throw ParseException();
        }
        return std::stoi(value);
    });
}

bool Arguments::optDouble(std::size_t index, double * output, bool check_grammar) const
{
    return tryObtainArgument(index, output, [this, check_grammar](std::string const & value){
        if (check_grammar && value.find(this->_point_delimiter) != std::string::npos) {
            throw ParseException();
        }
        return std::stod(value);
    });
}

bool Arguments::optString(std::size_t index, std::string * output) const
{
    return tryObtainArgument(index, output, [](std::string const & value){
        return value;
    });
}

bool Arguments::optIntegerPoint(std::size_t index, Pointi * output, bool check_grammar) const
{
    return tryObtainTokens(index, output, [check_grammar](std::vector<std::string> const & tokens){
        if (check_grammar && tokens.size() != 2) {
            throw ParseException();
        }
        return libtbag::geometry::makePoint(std::stoi(tokens.at(0)), std::stoi(tokens.at(1)));
    });
}

bool Arguments::optDoublePoint(std::size_t index, Pointd * output, bool check_grammar) const
{
    return tryObtainTokens(index, output, [check_grammar](std::vector<std::string> const & tokens){
        if (check_grammar && tokens.size() != 2) {
            throw ParseException();
        }
        return libtbag::geometry::makePoint(std::stod(tokens.at(0)), std::stod(tokens.at(1)));
    });
}

bool Arguments::optIntegerRect(std::size_t index, Recti * output, bool check_grammar) const
{
    return tryObtainTokens(index, output, [check_grammar](std::vector<std::string> const & tokens){
        if (check_grammar && tokens.size() != 4) {
            throw ParseException();
        }
        return libtbag::geometry::makeRect(std::stoi(tokens.at(0))
                                         , std::stoi(tokens.at(1))
                                         , std::stoi(tokens.at(2))
                                         , std::stoi(tokens.at(3)));
    });
}

bool Arguments::optDoubleRect(std::size_t index, Rectd * output, bool check_grammar) const
{
    return tryObtainTokens(index, output, [check_grammar](std::vector<std::string> const & tokens){
        if (check_grammar && tokens.size() != 4) {
            throw ParseException();
        }
        return libtbag::geometry::makeRect(std::stod(tokens.at(0))
                                         , std::stod(tokens.at(1))
                                         , std::stod(tokens.at(2))
                                         , std::stod(tokens.at(3)));
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

