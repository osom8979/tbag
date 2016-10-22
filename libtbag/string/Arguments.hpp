/**
 * @file   Arguments.hpp
 * @brief  Arguments class prototype.
 * @author zer0
 * @date   2016-09-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Exception.hpp>
#include <libtbag/string/Strings.hpp>
#include <libtbag/geometry/Point.hpp>
#include <libtbag/geometry/Rect.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * Arguments class prototype.
 *
 * @author zer0
 * @date   2016-09-23
 */
template <typename CharType = char>
class BaseArguments
{
public:
    using Value  = CharType;
    using String = std::basic_string<Value>;

    using StringVector = std::vector<String>;

public:
    static constexpr Value const * const DEFAULT_DELIMITER       = CHAR_OR_WIDECHAR(Value, ",");
    static constexpr Value const * const DEFAULT_POINT_DELIMITER = CHAR_OR_WIDECHAR(Value, "x");

private:
    String       _name;
    StringVector _args;

private:
    String _delimiter;
    String _point_delimiter;

public:
    BaseArguments() : _name(), _args(), _delimiter(DEFAULT_DELIMITER), _point_delimiter(DEFAULT_POINT_DELIMITER)
    {
        // EMPTY.
    }

    BaseArguments(String name
                , String arguments
                , String delimiter       = DEFAULT_DELIMITER
                , String point_delimiter = DEFAULT_POINT_DELIMITER) throw(InitializeException)
            : _name(name), _args(), _delimiter(delimiter), _point_delimiter(point_delimiter)
    {
        if (parse(arguments) == false) {
            throw InitializeException();
        }
    }

public:
    ~BaseArguments() = default;

public:
    BaseArguments(BaseArguments const & obj) = default;
    BaseArguments(BaseArguments && obj) = default;

public:
    BaseArguments & operator =(BaseArguments const & obj) = default;
    BaseArguments & operator =(BaseArguments && obj) = default;

public:
    inline String getName() const
    { return _name; }
    inline String getDelimiter() const
    { return _delimiter; }
    inline String getPointDelimiter() const
    { return _point_delimiter; }

    inline void setName(String name)
    { _name = name; }
    inline void setDelimiter(String delimiter)
    { _delimiter = delimiter; }
    inline void setPointDelimiter(String point_delimiter)
    { _point_delimiter = point_delimiter; }

    inline void clear() TBAG_NOEXCEPT
    { _args.clear(); }
    inline bool empty() const TBAG_NOEXCEPT
    { return _args.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT
    { return _args.size(); }

    inline String get(std::size_t index) const
    { return _args.at(index); }

public:
    inline void erase(std::size_t index)
    { _args.erase(_args.begin() + index); }
    inline void erase(String argument)
    { _args.erase(std::find(_args.begin(), _args.end(), argument)); }

    inline void push(String argument)
    { _args.push_back(argument); }
    inline void pop()
    { erase(0); }

public:
    void insert(std::size_t index, String argument)
    {
        _args.insert(_args.begin() + index, argument);
    }

public:
    bool parse(String arguments)
    {
        for (auto & cursor : libtbag::string::splitTokens(arguments, _delimiter)) {
            push(cursor);
        }
        return true;
    }

    std::string toString()
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

private:
    template <typename OutputType, typename Predicated>
    inline bool tryObtainArgument(std::size_t index, OutputType * output, Predicated predicated) const
    {
        try {
            OutputType result = predicated(_args.at(index));
            if (output != nullptr) {
                *output = result;
            }
            return true;
        } catch (...) {
            return false;
        }
    }

public:
    inline bool optInteger(std::size_t index, int * output, bool check_grammar = true) const
    {
        return tryObtainArgument(index, output, [this, check_grammar](String const & value){
            if (check_grammar && value.find(this->_point_delimiter) != String::npos) {
                throw ParseException();
            }
            return std::stoi(value);
        });
    }

    inline bool optDouble(std::size_t index, double * output, bool check_grammar = true) const
    {
        return tryObtainArgument(index, output, [this, check_grammar](String const & value){
            if (check_grammar && value.find(this->_point_delimiter) != String::npos) {
                throw ParseException();
            }
            return std::stod(value);
        });
    }

    inline bool optString(std::size_t index, String * output) const
    {
        return tryObtainArgument(index, output, [](String const & value){
            return value;
        });
    }

// ----------
// EXTENSION.
// ----------

public:
    using IntegerPoint = libtbag::geometry::BasePoint<int>;
    using  DoublePoint = libtbag::geometry::BasePoint<double>;

    using IntegerRect = libtbag::geometry::BaseRect<int>;
    using  DoubleRect = libtbag::geometry::BaseRect<double>;

private:
    template <typename OutputType, typename Predicated>
    inline bool tryObtainTokens(std::size_t index, OutputType * output, Predicated predicated) const
    {
        try {
            auto tokens = libtbag::string::splitTokens(_args.at(index), _point_delimiter);
            if (tokens.empty()) {
                return false;
            }

            OutputType result = predicated(tokens);
            if (output != nullptr) {
                *output = result;
            }
            return true;
        } catch (...) {
            return false;
        }
    }

public:
    inline bool optIntegerPoint(std::size_t index, IntegerPoint * output, bool check_grammar = true) const
    {
        return tryObtainTokens(index, output, [check_grammar](StringVector const & tokens){
            if (check_grammar && tokens.size() != 2) {
                throw ParseException();
            }
            return libtbag::geometry::makePoint(std::stoi(tokens.at(0)), std::stoi(tokens.at(1)));
        });
    }

    inline bool optDoublePoint(std::size_t index, DoublePoint * output, bool check_grammar = true) const
    {
        return tryObtainTokens(index, output, [check_grammar](StringVector const & tokens){
            if (check_grammar && tokens.size() != 2) {
                throw ParseException();
            }
            return libtbag::geometry::makePoint(std::stod(tokens.at(0)), std::stod(tokens.at(1)));
        });
    }

    inline bool optIntegerRect(std::size_t index, IntegerRect * output, bool check_grammar = true) const
    {
        return tryObtainTokens(index, output, [check_grammar](StringVector const & tokens){
            if (check_grammar && tokens.size() != 4) {
                throw ParseException();
            }
            return libtbag::geometry::makeRect(std::stoi(tokens.at(0))
                                             , std::stoi(tokens.at(1))
                                             , std::stoi(tokens.at(2))
                                             , std::stoi(tokens.at(3)));
        });
    }

    inline bool optDoubleRect(std::size_t index, DoubleRect * output, bool check_grammar = true) const
    {
        return tryObtainTokens(index, output, [check_grammar](StringVector const & tokens){
            if (check_grammar && tokens.size() != 4) {
                throw ParseException();
            }
            return libtbag::geometry::makeRect(std::stod(tokens.at(0))
                                             , std::stod(tokens.at(1))
                                             , std::stod(tokens.at(2))
                                             , std::stod(tokens.at(3)));
        });
    }

private:
    template <typename Type, typename Predicated>
    inline std::vector<Type> getVector(Predicated predicated) const
    {
        std::size_t const SIZE = _args.size();
        std::vector<Type> result;
        Type temp;
        for (std::size_t index = 0 ; index < SIZE; ++index) {
            if (predicated(index, &temp)) {
                result.push_back(temp);
            }
        }
        return result;
    }

public:
    inline std::vector<String> getStrings() const
    {
        return _args;
    }

    inline std::vector<int> getIntegers() const
    {
        return getVector<int>([this](std::size_t index, int * output){
            return this->optInteger(index, output);
        });
    }

    inline std::vector<double> getDoubles() const
    {
        return getVector<double>([this](std::size_t index, double * output){
            return this->optDouble(index, output);
        });
    }

    inline std::vector<IntegerPoint> getIntegerPoints() const
    {
        return getVector<IntegerPoint>([this](std::size_t index, IntegerPoint * output){
            return this->optIntegerPoint(index, output);
        });
    }

    inline std::vector<DoublePoint> getDoublePoints() const
    {
        return getVector<DoublePoint>([this](std::size_t index, DoublePoint * output){
            return this->optDoublePoint(index, output);
        });
    }

    inline std::vector<IntegerRect> getIntegerRects() const
    {
        return getVector<IntegerRect>([this](std::size_t index, IntegerRect * output){
            return this->optIntegerRect(index, output);
        });
    }

    inline std::vector<DoubleRect> getDoubleRects() const
    {
        return getVector<DoubleRect>([this](std::size_t index, DoubleRect * output){
            return this->optDoubleRect(index, output);
        });
    }
};

using Arguments = BaseArguments<char>;
using WideArguments = BaseArguments<wchar_t>;

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__

