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
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/geometry/Point.hpp>
#include <libtbag/geometry/Rect.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

char const * const DEFAULT_ARGUMENTS_DELIMITER       = ",";
char const * const DEFAULT_ARGUMENTS_POINT_DELIMITER = "x";

/**
 * Arguments class prototype.
 *
 * @author zer0
 * @date   2016-09-23
 */
class TBAG_API Arguments
{
private:
    std::string _name;
    std::vector<std::string> _args;

    std::string _delimiter;
    std::string _point_delimiter;

public:
    Arguments();
    Arguments(std::string const & name
            , std::string const & arguments
            , std::string const & delimiter       = DEFAULT_ARGUMENTS_DELIMITER
            , std::string const & point_delimiter = DEFAULT_ARGUMENTS_POINT_DELIMITER);
    ~Arguments();

public:
    Arguments(Arguments const & obj) = default;
    Arguments & operator =(Arguments const & obj) = default;

#if defined(TBAG_HAS_DEFAULTED_FUNCTIONS) && !defined(TBAG_HAS_DEFAULTED_FUNCTIONS_BUT_NOT_MOVE_FUNCTION)
    Arguments(Arguments && obj) = default;
    Arguments & operator =(Arguments && obj) = default;
#endif

public:
    inline std::string getName() const
    { return _name; }
    inline std::string getDelimiter() const
    { return _delimiter; }
    inline std::string getPointDelimiter() const
    { return _point_delimiter; }

    inline void setName(std::string const & name)
    { _name = name; }
    inline void setDelimiter(std::string const & delimiter)
    { _delimiter = delimiter; }
    inline void setPointDelimiter(std::string const & point_delimiter)
    { _point_delimiter = point_delimiter; }

    inline void clear() TBAG_NOEXCEPT
    { _args.clear(); }
    inline bool empty() const TBAG_NOEXCEPT
    { return _args.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT
    { return _args.size(); }

    inline std::string get(std::size_t index) const
    { return _args.at(index); }

public:
    inline void erase(std::size_t index)
    { _args.erase(_args.begin() + index); }
    inline void erase(std::string const & argument)
    { _args.erase(std::find(_args.begin(), _args.end(), argument)); }

    inline void push(std::string const & argument)
    { _args.push_back(argument); }
    inline void pop()
    { erase(0); }

public:
    void insert(std::size_t index, std::string const & argument);

public:
    bool parse(std::string const & arguments);
    std::string toString();

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
    bool optInteger(std::size_t index, int         * output, bool check_grammar = true) const;
    bool optDouble (std::size_t index, double      * output, bool check_grammar = true) const;
    bool optString (std::size_t index, std::string * output) const;

// ----------
// EXTENSION.
// ----------

public:
    using Pointi = libtbag::geometry::BasePoint<int>;
    using Pointd = libtbag::geometry::BasePoint<double>;

    using Recti = libtbag::geometry::BaseRect<int>;
    using Rectd = libtbag::geometry::BaseRect<double>;

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
    bool optIntegerPoint(std::size_t index, Pointi * output, bool check_grammar = true) const;
    bool optDoublePoint (std::size_t index, Pointd * output, bool check_grammar = true) const;
    bool optIntegerRect (std::size_t index, Recti  * output, bool check_grammar = true) const;
    bool optDoubleRect  (std::size_t index, Rectd  * output, bool check_grammar = true) const;

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
    std::vector<std::string> getStrings      () const;
    std::vector<int        > getIntegers     () const;
    std::vector<double     > getDoubles      () const;
    std::vector<Pointi     > getIntegerPoints() const;
    std::vector<Pointd     > getDoublePoints () const;
    std::vector<Recti      > getIntegerRects () const;
    std::vector<Rectd      > getDoubleRects  () const;
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__

