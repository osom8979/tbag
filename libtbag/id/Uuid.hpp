/**
 * @file   Uuid.hpp
 * @brief  Uuid class prototype.
 * @author zer0
 * @date   2017-07-01
 *
 * @see <https://tools.ietf.org/html/rfc4122>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ID_UUID_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ID_UUID_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <cstdint>
#include <string>
#include <iterator>
#include <type_traits>
#include <exception>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace id {

/**
 * Uuid class prototype.
 *
 * @author zer0
 * @date   2017-07-01
 *
 * @see <https://en.wikipedia.org/wiki/Universally_unique_identifier>
 *
 * @remarks
 *  0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                          time_low                             |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |       time_mid                |         time_hi_and_version   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |clk_seq_hi_res |  clk_seq_low  |         node (0-1)            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         node (2-5)                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class TBAG_API Uuid
{
public:
    TBAG_CONSTEXPR static std::size_t const       BIT_SIZE = 128;
    TBAG_CONSTEXPR static std::size_t const      BYTE_SIZE = BIT_SIZE / 8;
    TBAG_CONSTEXPR static std::size_t const NODE_BYTE_SIZE = 6;

    TBAG_CONSTEXPR static uint8_t const HI_VERSION_01 = 0x10;
    TBAG_CONSTEXPR static uint8_t const HI_VERSION_02 = 0x20;
    TBAG_CONSTEXPR static uint8_t const HI_VERSION_03 = 0x30;
    TBAG_CONSTEXPR static uint8_t const HI_VERSION_04 = 0x40;
    TBAG_CONSTEXPR static uint8_t const HI_VERSION_05 = 0x50;

    TBAG_CONSTEXPR static uint8_t const LOW_FULL = 0x0F;

    // The following table lists the contents of the variant field, where
    // the letter "x" indicates a "don't-care" value.
    // In this class, x is 0.
    TBAG_CONSTEXPR static uint8_t const VARIANT_0XX = 0x00; ///< Reserved, NCS backward compatibility.
    TBAG_CONSTEXPR static uint8_t const VARIANT_10X = 0x80; ///< The variant specified in this document.
    TBAG_CONSTEXPR static uint8_t const VARIANT_110 = 0xC0; ///< Reserved, Microsoft Corporation backward compatibility
    TBAG_CONSTEXPR static uint8_t const VARIANT_111 = 0xE0; ///< Reserved for future definition.

    TBAG_CONSTEXPR static char const UUID_STR_DASH = '-';
    TBAG_CONSTEXPR static char const * const NIL_UUID_STR = "00000000-0000-0000-0000-000000000000";
    TBAG_CONSTEXPR static std::size_t NIL_UUID_STR_LENGTH = 36;

public:
    enum class Version
    {
        UUID_VER_1, ///< Version 1 (date-time and MAC address)
        UUID_VER_2, ///< Version 2 (date-time and MAC Address, DCE security version)
        UUID_VER_3, ///< Version 3 (namespace name-based; MD5 hash)
        UUID_VER_4, ///< Version 4 (random)
        UUID_VER_5, ///< Version 5 (namespace name-based; SHA-1 hash)
    };

public:
    using value_type             = uint8_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    union Identifier
    {
        struct {
            uint32_t time_low;              ///< The low field of the timestamp.
            uint16_t time_mid;              ///< The middle field of the timestamp.
            uint16_t time_hi_and_version;   ///< The high field of the timestamp multiplexed with the version number.
            uint8_t  clock_seq_hi_and_reserved; ///<The high field of the clock sequence multiplexed with the variant.
            uint8_t  clock_seq_low;         ///< The low field of the clock sequence.
            uint8_t  node[NODE_BYTE_SIZE];  ///< The spatially unique node identifier.
        };
        value_type data[BYTE_SIZE];
    };

public:
    Identifier id;

public:
    static_assert(sizeof(Identifier) == BYTE_SIZE,
                  "A universally unique identifier (UUID) is a 128-bit number "
                  "used to identify information in computer systems.");

public:
    Uuid() TBAG_NOEXCEPT;
    Uuid(Version version);
    Uuid(std::string const & str);
    Uuid(Uuid const & obj) TBAG_NOEXCEPT;
    Uuid(Uuid && obj) TBAG_NOEXCEPT;
    virtual ~Uuid();

public:
    Uuid & operator =(Uuid const & obj) TBAG_NOEXCEPT;
    Uuid & operator =(Uuid && obj) TBAG_NOEXCEPT;

public:
    bool operator ==(Uuid const & obj) const TBAG_NOEXCEPT;
    bool operator !=(Uuid const & obj) const TBAG_NOEXCEPT;

    bool operator ==(std::string const & str) const;
    bool operator !=(std::string const & str) const;

public:
    void fill(value_type const & data) TBAG_NOEXCEPT;
    void swap(Uuid & obj) TBAG_NOEXCEPT;

// Iterators methods.
public:
    inline       iterator begin()       TBAG_NOEXCEPT { return       iterator(id.data); }
    inline const_iterator begin() const TBAG_NOEXCEPT { return const_iterator(id.data + BYTE_SIZE); }
    inline       iterator   end()       TBAG_NOEXCEPT { return       iterator(id.data + BYTE_SIZE); }
    inline const_iterator   end() const TBAG_NOEXCEPT { return const_iterator(id.data + BYTE_SIZE); }

    inline       reverse_iterator rbegin()       TBAG_NOEXCEPT { return       reverse_iterator(end()); }
    inline const_reverse_iterator rbegin() const TBAG_NOEXCEPT { return const_reverse_iterator(end()); }
    inline       reverse_iterator   rend()       TBAG_NOEXCEPT { return       reverse_iterator(begin()); }
    inline const_reverse_iterator   rend() const TBAG_NOEXCEPT { return const_reverse_iterator(begin()); }

    inline         const_iterator  cbegin() const TBAG_NOEXCEPT { return  begin(); }
    inline         const_iterator    cend() const TBAG_NOEXCEPT { return    end(); }
    inline const_reverse_iterator crbegin() const TBAG_NOEXCEPT { return rbegin(); }
    inline const_reverse_iterator   crend() const TBAG_NOEXCEPT { return   rend(); }

// Capacity methods.
public:
    TBAG_CONSTEXPR static size_type  size() TBAG_NOEXCEPT { return BYTE_SIZE; }
    TBAG_CONSTEXPR static      bool empty() TBAG_NOEXCEPT { return false; }

// Element access methods.
public:
    inline       reference operator[](size_type i)       { return id.data[i]; }
    inline const_reference operator[](size_type i) const { return id.data[i]; }

          reference at(size_type i);
    const_reference at(size_type i) const;

    inline       reference front()       { return id.data[0]; }
    inline const_reference front() const { return id.data[0]; }
    inline       reference  back()       { return id.data[BYTE_SIZE-1]; }
    inline const_reference  back() const { return id.data[BYTE_SIZE-1]; }

    inline       value_type * data()       TBAG_NOEXCEPT { return id.data; }
    inline const value_type * data() const TBAG_NOEXCEPT { return id.data; }

public:
    bool initVersion1();
    bool initVersion2();

    /** Algorithm for Creating a Name-Based UUID. */
    bool initVersion3();

    /**
     * Algorithms for Creating a UUID from Truly Random or Pseudo-Random Numbers.
     *
     * @see <https://tools.ietf.org/html/rfc4122#section-4.4>
     */
    bool initVersion4();

    /** Algorithm for Creating a Name-Based UUID. */
    bool initVersion5();

public:
    bool init(Version version = Version::UUID_VER_4);

public:
    std::string toString();
    Err fromString(std::string const & str);

public:
    /**
     * The "nil" UUID, a special case, is the UUID, 00000000-0000-0000-0000-000000000000;
     * that is, all bits set to zero.
     */
    static Uuid nil() TBAG_NOEXCEPT;
    static Uuid ver4() TBAG_NOEXCEPT;
};

} // namespace id

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ID_UUID_HPP__

