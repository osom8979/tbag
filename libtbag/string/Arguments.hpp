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

/**
 * Arguments class prototype.
 *
 * @author zer0
 * @date   2016-09-23
 */
class TBAG_API Arguments
{
public:
    TBAG_CONSTEXPR static char const * const DEFAULT_ARGUMENTS_DELIMITER       = ",";
    TBAG_CONSTEXPR static char const * const DEFAULT_ARGUMENTS_POINT_DELIMITER = "x";

    TBAG_CONSTEXPR static char const * const DEFAULT_BOOLEAN_TRUE_CASE1 = "ON";
    TBAG_CONSTEXPR static char const * const DEFAULT_BOOLEAN_TRUE_CASE2 = "YES";
    TBAG_CONSTEXPR static char const * const DEFAULT_BOOLEAN_TRUE_CASE3 = "TRUE";

    TBAG_CONSTEXPR static char const * const DEFAULT_BOOLEAN_FALSE_CASE1 = "OFF";
    TBAG_CONSTEXPR static char const * const DEFAULT_BOOLEAN_FALSE_CASE2 = "NO";
    TBAG_CONSTEXPR static char const * const DEFAULT_BOOLEAN_FALSE_CASE3 = "FALSE";

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

    Arguments(Arguments const & obj);
    Arguments(Arguments && obj);
    ~Arguments();

public:
    Arguments & operator =(Arguments const & obj);
    Arguments & operator =(Arguments && obj);

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
    bool optBoolean         (std::size_t index, bool               * output, bool check_grammar = true) const;
    bool optChar            (std::size_t index, char               * output, bool check_grammar = true) const;
    bool optUnsignedChar    (std::size_t index, unsigned char      * output, bool check_grammar = true) const;
    bool optShort           (std::size_t index, short              * output, bool check_grammar = true) const;
    bool optUnsignedShort   (std::size_t index, unsigned short     * output, bool check_grammar = true) const;
    bool optInteger         (std::size_t index, int                * output, bool check_grammar = true) const;
    bool optLong            (std::size_t index, long               * output, bool check_grammar = true) const;
    bool optUnsignedLong    (std::size_t index, unsigned long      * output, bool check_grammar = true) const;
    bool optLongLong        (std::size_t index, long long          * output, bool check_grammar = true) const;
    bool optUnsignedLongLong(std::size_t index, unsigned long long * output, bool check_grammar = true) const;
    bool optFloat           (std::size_t index, float              * output, bool check_grammar = true) const;
    bool optDouble          (std::size_t index, double             * output, bool check_grammar = true) const;
    bool optLongDouble      (std::size_t index, long double        * output, bool check_grammar = true) const;
    bool optString          (std::size_t index, std::string        * output) const;

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

public:
    // @formatter:off
    bool opt(std::size_t i, bool               * o, bool c = true) const { return optBoolean         (i, o, c); }
    bool opt(std::size_t i, char               * o, bool c = true) const { return optChar            (i, o, c); }
    bool opt(std::size_t i, unsigned char      * o, bool c = true) const { return optUnsignedChar    (i, o, c); }
    bool opt(std::size_t i, short              * o, bool c = true) const { return optShort           (i, o, c); }
    bool opt(std::size_t i, unsigned short     * o, bool c = true) const { return optUnsignedShort   (i, o, c); }
    bool opt(std::size_t i, int                * o, bool c = true) const { return optInteger         (i, o, c); }
    bool opt(std::size_t i, long               * o, bool c = true) const { return optLong            (i, o, c); }
    bool opt(std::size_t i, unsigned long      * o, bool c = true) const { return optUnsignedLong    (i, o, c); }
    bool opt(std::size_t i, long long          * o, bool c = true) const { return optLongLong        (i, o, c); }
    bool opt(std::size_t i, unsigned long long * o, bool c = true) const { return optUnsignedLongLong(i, o, c); }
    bool opt(std::size_t i, float              * o, bool c = true) const { return optFloat           (i, o, c); }
    bool opt(std::size_t i, double             * o, bool c = true) const { return optDouble          (i, o, c); }
    bool opt(std::size_t i, long double        * o, bool c = true) const { return optLongDouble      (i, o, c); }
    bool opt(std::size_t i, std::string        * o, bool c = true) const { return optString          (i, o);    }
    bool opt(std::size_t i, Pointi             * o, bool c = true) const { return optIntegerPoint    (i, o, c); }
    bool opt(std::size_t i, Pointd             * o, bool c = true) const { return optDoublePoint     (i, o, c); }
    bool opt(std::size_t i, Recti              * o, bool c = true) const { return optIntegerRect     (i, o, c); }
    bool opt(std::size_t i, Rectd              * o, bool c = true) const { return optDoubleRect      (i, o, c); }
    // @formatter:on
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_ARGUMENTS_HPP__

