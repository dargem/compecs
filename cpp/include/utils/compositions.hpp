#ifndef COMPECS_COMPOSITIONS_HPP
#define COMPECS_COMPOSITIONS_HPP

#include "utils/packs.hpp"

namespace compecs {

namespace detail {
template <typename T>
struct Base {};

// direct multi-inheritance from the same class is ill formed so we use a default lambda to abstract
// away the true base (the type) another layer down, so it will be falsy but not compile error
template <typename T, auto Differentiator = [] {}>
struct BaseClass : Base<T> {};

template <typename T>
struct Layout {};
// type pack has higher specificity so even for arguments of length 1 this won't instantiate

template <typename... Ts>
struct Layout<TypePack<Ts...>> : BaseClass<Ts>... {};

// will only have a standard layout if all its base classes are of distinct types
template <typename Pack>
struct UniqueTypesImpl : std::bool_constant<std::is_standard_layout_v<Layout<Pack>>> {};
}  // namespace detail

template <typename TypePack>
concept IsUniquePackTypes = IsTypePack<TypePack> && detail::UniqueTypesImpl<TypePack>::value;

namespace detail {
template <typename T, typename... Ts>
concept OneOf = (std::is_same_v<T, Ts> || ...);

template <typename T, typename Pack>
struct OneOfPackImpl : std::false_type {};

template <typename T, typename... Ts>
struct OneOfPackImpl<T, TypePack<Ts...>> : std::bool_constant<OneOf<T, Ts...>> {};
}  // namespace detail

// OneOf<T, TypePack<...>>, checks if T is in  the TypePack
template <typename T, typename Pack>
concept OneOfPack = IsTypePack<Pack> && detail::OneOfPackImpl<T, Pack>::value;

namespace detail {
template <typename ElementsPack, typename SetPack>
struct BoundedPacksImpl : std::false_type {};

template <typename... Elements, typename... Set>
struct BoundedPacksImpl<TypePack<Elements...>, TypePack<Set...>>
        : std::bool_constant<(OneOf<Elements, Set...> && ...)> {};
}  // namespace detail

// BoundedPacks<TypePack<...>, TypePack<...>>
// Checks if element of types in first pack are a subset of types in second pack
template <typename ElementsPack, typename SetPack>
concept BoundedPacks = IsTypePack<ElementsPack> && IsTypePack<SetPack> &&
                       detail::BoundedPacksImpl<ElementsPack, SetPack>::value;

namespace detail {
template <typename PackA, typename PackB>
struct SameSizePacksImpl : std::false_type {};

template <typename... A, typename... B>
struct SameSizePacksImpl<TypePack<A...>, TypePack<B...>>
        : std::bool_constant<(sizeof...(A) == sizeof...(B))> {};
}  // namespace detail

// SameSizePacks<TypePack<...>, TypePack<...>>
// Checks if the template packings are the same size
template <typename PackA, typename PackB>
concept SameSizePacks =
    IsTypePack<PackA> && IsTypePack<PackB> && detail::SameSizePacksImpl<PackA, PackB>::value;

namespace detail {
template <typename PackA, typename PackB>
struct IsPermutationPacksImpl : std::false_type {};

template <typename... A, typename... B>
struct IsPermutationPacksImpl<TypePack<A...>, TypePack<B...>>
        : std::bool_constant<
              SameSizePacks<TypePack<A...>, TypePack<B...>> && IsUniquePackTypes<TypePack<A...>> &&
              IsUniquePackTypes<TypePack<B...>> && BoundedPacks<TypePack<A...>, TypePack<B...>>> {};
}  // namespace detail

// IsPermutationPacks<TypePack<...>, TypePack<...>>. Checks if both both template packings are
// made from the same templates, not necessarily in order If A and B are same size, both are unique,
// and all members of A are bounded in B it must be made from exact same components
template <typename PackA, typename PackB>
concept IsPermutationPacks =
    IsTypePack<PackA> && IsTypePack<PackB> && detail::IsPermutationPacksImpl<PackA, PackB>::value;

namespace detail {
template <typename A, typename B>
struct IsIntersectingImpl {};

template <typename... A, typename... B>
struct IsIntersectingImpl<TypePack<A...>, TypePack<B...>>
        : std::bool_constant<(OneOf<A, B...> || ...)> {};
}  // namespace detail

// Is truthy if the parameter packs share at least 1 type, false if no intersecting types
template <typename PackA, typename PackB>
concept IsIntersecting =
    IsTypePack<PackA> && IsTypePack<PackB> && detail::IsIntersectingImpl<PackA, PackB>::value;

}  // namespace compecs
#endif