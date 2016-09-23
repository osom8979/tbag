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
            : _name(), _args(), _delimiter(delimiter), _point_delimiter(point_delimiter)
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

    inline void clear() noexcept
    { _args.clear(); }
    inline bool empty() const noexcept
    { return _args.empty(); }
    inline std::size_t size() const noexcept
    { return _args.size(); }

public:
    bool parse(String arguments)
    {
        for (auto & cursor : libtbag::string::splitTokens(arguments, _delimiter)) {
            _args.push_back(cursor);
        }
        return true;
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
};

using Arguments = BaseArguments<char>;
using WideArguments = BaseArguments<wchar_t>;

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__

