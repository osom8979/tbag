/**
 * @file   ByteStringParser.hpp
 * @brief  ByteStringParser class prototype.
 * @author zer0
 * @date   2020-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRINGPARSER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRINGPARSER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/Unit.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * ByteStringParser class prototype.
 *
 * @author zer0
 * @date   2020-05-17
 */
struct TBAG_API ByteStringParser : private Noncopyable
{
    using ErrSize = libtbag::ErrPair<std::size_t>;

    static ErrSize  byte(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v }; }
    static ErrSize  kilo(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v *  KILO_BYTE_TO_BYTE }; }
    static ErrSize  mega(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v *  MEGA_BYTE_TO_BYTE }; }
    static ErrSize  giga(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v *  GIGA_BYTE_TO_BYTE }; }
    static ErrSize  tera(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v *  TERA_BYTE_TO_BYTE }; }
    static ErrSize  peta(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v *  PETA_BYTE_TO_BYTE }; }
    static ErrSize   exa(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v *   EXA_BYTE_TO_BYTE }; }
    static ErrSize zetta(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v * ZETTA_BYTE_TO_BYTE }; }
    static ErrSize yotta(std::size_t v) TBAG_NOEXCEPT { return { E_SUCCESS, v * YOTTA_BYTE_TO_BYTE }; }

    static ErrSize parse1(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 1);
        // clang-format off
        switch (unit[0]) {
        case 'b': return  byte(value);
        case 'k': return  kilo(value);
        case 'm': return  mega(value);
        case 'g': return  giga(value);
        case 't': return  tera(value);
        case 'p': return  peta(value);
        case 'e': return   exa(value);
        case 'z': return zetta(value);
        case 'y': return yotta(value);
        default:  return E_PARSING;
        }
        // clang-format on
    }

    static ErrSize parse2(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 2);
        // All three letters end with 'b'.
        if (unit[1] == 'b') {
            // clang-format off
            switch (unit[0]) {
            case 'k': return  kilo(value);
            case 'm': return  mega(value);
            case 'g': return  giga(value);
            case 't': return  tera(value);
            case 'p': return  peta(value);
            case 'e': return   exa(value);
            case 'z': return zetta(value);
            case 'y': return yotta(value);
            }
            // clang-format on
        }
        return E_PARSING;
    }

    static ErrSize parse3(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 3);
        if (TBAG_STRCMP3(unit, "exa")) {
            return exa(value);
        }
        return E_PARSING;
    }

    static ErrSize parse4(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 4);
        auto const * suffix_text = &(unit[1]);
        // clang-format off
        switch (unit[0]) {
        case 'b': return TBAG_STRCMP3(suffix_text, "yte") ? byte(value) : ErrSize(E_PARSING,0);
        case 'k': return TBAG_STRCMP3(suffix_text, "ilo") ? kilo(value) : ErrSize(E_PARSING,0);
        case 'm': return TBAG_STRCMP3(suffix_text, "ega") ? mega(value) : ErrSize(E_PARSING,0);
        case 'g': return TBAG_STRCMP3(suffix_text, "iga") ? giga(value) : ErrSize(E_PARSING,0);
        case 't': return TBAG_STRCMP3(suffix_text, "eta") ? tera(value) : ErrSize(E_PARSING,0);
        case 'p': return TBAG_STRCMP3(suffix_text, "eta") ? peta(value) : ErrSize(E_PARSING,0);
        }
        // clang-format on
        return E_PARSING;
    }

    static ErrSize parse5(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 5);
        auto const * suffix_text = &(unit[1]);
        // clang-format off
        switch (unit[0]) {
        case 'k': return TBAG_STRCMP4(suffix_text, "byte") ? kilo(value) : ErrSize(E_PARSING,0);
        case 'm': return TBAG_STRCMP4(suffix_text, "byte") ? mega(value) : ErrSize(E_PARSING,0);
        case 'g': return TBAG_STRCMP4(suffix_text, "byte") ? giga(value) : ErrSize(E_PARSING,0);
        case 't': return TBAG_STRCMP4(suffix_text, "byte") ? tera(value) : ErrSize(E_PARSING,0);
        case 'p': return TBAG_STRCMP4(suffix_text, "byte") ? peta(value) : ErrSize(E_PARSING,0);
        case 'e': return TBAG_STRCMP4(suffix_text, "byte") ?  exa(value) : ErrSize(E_PARSING,0);
        case 'z':
            if (TBAG_STRCMP4(suffix_text, "byte")) {
                return zetta(value);
            } else if (TBAG_STRCMP4(suffix_text, "etta")) {
                return zetta(value);
            } else {
                return E_PARSING;
            }
        case 'y':
            if (TBAG_STRCMP4(suffix_text, "byte")) {
                return yotta(value);
            } else if (TBAG_STRCMP4(suffix_text, "otta")) {
                return yotta(value);
            } else {
                return E_PARSING;
            }
        default:
            return E_PARSING;
        }
        // clang-format on
    }

    static ErrSize parse6(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 6);
        return E_PARSING;
    }

    static ErrSize parse7(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 7);
        if (TBAG_STRCMP7(unit, "exabyte")) {
            return exa(value);
        }
        return E_PARSING;
    }

    static ErrSize parse8(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 8);
        auto const * suffix_text = &(unit[1]);
        // clang-format off
        switch (unit[0]) {
        case 'k': return TBAG_STRCMP7(suffix_text, "ilobyte") ? kilo(value) : ErrSize(E_PARSING,0);
        case 'm': return TBAG_STRCMP7(suffix_text, "egabyte") ? mega(value) : ErrSize(E_PARSING,0);
        case 'g': return TBAG_STRCMP7(suffix_text, "igabyte") ? giga(value) : ErrSize(E_PARSING,0);
        case 't': return TBAG_STRCMP7(suffix_text, "erabyte") ? tera(value) : ErrSize(E_PARSING,0);
        case 'p': return TBAG_STRCMP7(suffix_text, "etabyte") ? peta(value) : ErrSize(E_PARSING,0);
        }
        // clang-format on
        return E_PARSING;
    }

    static ErrSize parse9(std::size_t value, std::string const & unit)
    {
        assert(unit.size() == 9);
        auto const * suffix_text = &(unit[1]);
        // clang-format off
        switch (unit[0]) {
        case 'z': return TBAG_STRCMP8(suffix_text, "zettabyte") ? zetta(value) : ErrSize(E_PARSING,0);
        case 'y': return TBAG_STRCMP8(suffix_text, "yottabyte") ? yotta(value) : ErrSize(E_PARSING,0);
        }
        // clang-format on
        return E_PARSING;
    }

    static ErrSize parse(std::string const & str)
    {
        std::string number;
        std::string unit;
        auto const split_code = libtbag::string::splitNumberAndUnit(str, number, unit);
        if (isFailure(split_code)) {
            return split_code;
        }

        auto const value = libtbag::string::toValue<std::size_t>(number);
        auto const unit_size = unit.size();

        // clang-format off
        switch (unit_size) {
        case  0: return { E_SUCCESS, value };
        case  1: return parse1(value, unit);
        case  2: return parse2(value, unit);
        case  3: return parse3(value, unit);
        case  4: return parse4(value, unit);
        case  5: return parse5(value, unit);
        case  6: return parse6(value, unit);
        case  7: return parse7(value, unit);
        case  8: return parse8(value, unit);
        case  9: return parse9(value, unit);
        default: return E_PARSING;
        }
        // clang-format on
    }
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRINGPARSER_HPP__

