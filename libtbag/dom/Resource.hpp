/**
 * @file   Resource.hpp
 * @brief  Resource class prototype.
 * @author zer0
 * @date   2016-04-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_RESOURCE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_RESOURCE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <unordered_map>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {

constexpr bool isCompactXmlFile() noexcept
{
    return false;
}

/**
 * Resource class prototype.
 *
 * @author zer0
 * @date   2016-04-12
 */
class TBAG_EXPORTS Resource
{
public:
    static constexpr char const * const ROOT_TAG = "resource";
    static constexpr char const * const NAME_ATTRIBUTE = "name";

public:
    using String = std::string;
    using Map = std::unordered_map<String, String>;

private:
    String _tag;
    Map _map;

public:
    Resource();
    Resource(Resource const & obj);
    Resource(Resource && obj);
    virtual ~Resource();

public:
    Resource & operator =(Resource const & obj);
    Resource & operator =(Resource && obj);

public:
    Resource & copy(Resource const & obj);
    void swap(Resource & obj);

public:
    void clear() noexcept;
    std::size_t size() const noexcept;

public:
    void set_tag(std::string const & tag) noexcept;
    std::string get_tag() const noexcept;

// XML.
public:
    bool readFile(std::string const & path, std::string const & tag);
    bool readString(std::string const & xml, std::string const & tag);
    bool save(std::string const & path);

// Accessor.
public:
    template <typename Type, typename Converter>
    bool getValue(std::string const & key, Type * result, Converter func) const
    {
        auto find_value = this->_map.find(key);
        if (find_value == this->_map.end()) {
            return false;
        }

        Type convert = 0;
        try {
            convert = func(find_value->second);
        } catch (std::invalid_argument & e) {
            // e.what();
            return false;
        }

        if (result != nullptr) {
            *result = convert;
        }
        return true;
    }

public:
    bool getString(std::string const & key, std::string * result) const;
    std::string getString(std::string const & key, std::string default_value) const;
    std::string getString(std::string const & key) const;

public:
    auto get(std::string const & key
           , std::string default_value) const -> decltype(default_value);

#ifndef __RESOURCE_ACCESSOR_IMPLEMENT
#define __RESOURCE_ACCESSOR_IMPLEMENT(name, type, func, value)      \
public:                                                             \
    bool get##name(std::string const & key, type * result) const {  \
        return getValue(key, result                                 \
                , [](std::string const & str) -> type {             \
                    return func(str);                               \
                });                                                 \
    }                                                               \
    type get##name(std::string const & key                          \
                 , type default_value = value) const {              \
        type result = 0;                                            \
        if (get##name(key, &result)) {                              \
            return result;                                          \
        }                                                           \
        return default_value;                                       \
    }                                                               \
    auto get(std::string const & key                                \
           , type default_value = value) const                      \
           -> decltype(default_value) {                             \
        return this->get##name(key, default_value);                 \
    }
#endif

public:
    __RESOURCE_ACCESSOR_IMPLEMENT(Integer,    int,                   std::stoi,   0);
    __RESOURCE_ACCESSOR_IMPLEMENT(UnInteger,  unsigned int,          std::stoul,  0);
    __RESOURCE_ACCESSOR_IMPLEMENT(LongLong,   long long,             std::stoll,  0);
    __RESOURCE_ACCESSOR_IMPLEMENT(UnLongLong, unsigned long long,    std::stoull, 0);

    __RESOURCE_ACCESSOR_IMPLEMENT(Float,      float,       std::stof,  0.0);
    __RESOURCE_ACCESSOR_IMPLEMENT(Double,     double,      std::stod,  0.0);
    __RESOURCE_ACCESSOR_IMPLEMENT(LongDouble, long double, std::stold, 0.0);

// Mutator.
public:
    void set(std::string const & key, std::string const & value);

#ifndef __RESOURCE_MUTATOR_IMPLEMENT
#define __RESOURCE_MUTATOR_IMPLEMENT(type)                  \
public:                                                     \
    void set(std::string const & key, type const & value) { \
        this->set(key, std::to_string(value));              \
    }
#endif

public:
    __RESOURCE_MUTATOR_IMPLEMENT(int);
    __RESOURCE_MUTATOR_IMPLEMENT(unsigned int);
    __RESOURCE_MUTATOR_IMPLEMENT(long long);
    __RESOURCE_MUTATOR_IMPLEMENT(unsigned long long);

    __RESOURCE_MUTATOR_IMPLEMENT(float);
    __RESOURCE_MUTATOR_IMPLEMENT(double);
    __RESOURCE_MUTATOR_IMPLEMENT(long double);

public:
    std::string & at(std::string const & key);
    std::string const & at(std::string const & key) const;

public:
    static Map readFromXmlString(std::string const & xml, std::string const & tag);
    static Map readFromXmlFile(std::string const & path, std::string const & tag);
    static bool save(std::string const & path, std::string const & tag, Map const & map);
};

} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_RESOURCE_HPP__

