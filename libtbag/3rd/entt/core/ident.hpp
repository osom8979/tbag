#ifndef ENTT_CORE_IDENT_HPP
#define ENTT_CORE_IDENT_HPP


#include<type_traits>
#include<cstddef>
#include<utility>


namespace entt {


namespace {


template<typename... Types>
struct Identifier final: Identifier<Types>... {
    using identifier_type = std::size_t;

    template<std::size_t... Indexes>
    constexpr Identifier(std::index_sequence<Indexes...>)
        : Identifier<Types>{std::index_sequence<Indexes>{}}...
    {}

    template<typename Type>
    constexpr std::size_t get() const {
        return Identifier<std::decay_t<Type>>::get();
    }
};


template<typename Type>
struct Identifier<Type> {
    using identifier_type = std::size_t;

    template<std::size_t Index>
    constexpr Identifier(std::index_sequence<Index>)
        : index{Index}
    {}

    constexpr std::size_t get() const {
        return index;
    }

private:
    const std::size_t index;
};


}


/**
 * @brief Types identifers.
 *
 * Variable template used to generate identifiers at compile-time for the given
 * types. Use the `constexpr` `get` member function to know what's the
 * identifier associated to the specific type.
 *
 * @note
 * Identifiers are constant expression and can be used in any context where such
 * an expression is required. As an example:
 * @code{.cpp}
 * constexpr auto identifiers = entt::ident<AType, AnotherType>;
 *
 * switch(aTypeIdentifier) {
 * case identifers.get<AType>():
 *     // ...
 *     break;
 * case identifers.get<AnotherType>():
 *     // ...
 *     break;
 * default:
 *     // ...
 * }
 * @endcode
 *
 * @note
 * In case of single type list, `get` isn't a member function template:
 * @code{.cpp}
 * func(std::integral_constant<
 *     entt::ident<AType>::identifier_type,
 *     entt::ident<AType>::get()
 * >{});
 * @endcode
 *
 * @tparam Types List of types for which to generate identifiers.
 */
template<typename... Types>
constexpr auto ident = Identifier<std::decay_t<Types>...>{std::make_index_sequence<sizeof...(Types)>{}};


}


#endif // ENTT_CORE_IDENT_HPP
