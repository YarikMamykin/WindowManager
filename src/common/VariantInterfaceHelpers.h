#pragma once
#include <cstddef>
#include <optional>
#include <string_view>
#include <type_traits>
#include <variant>

namespace ymwm::common {

  template <typename T>
  struct is_variant : std::false_type {};

  template <typename... Ts>
  struct is_variant<std::variant<Ts...>> : std::true_type {};

  template <typename T>
  inline constexpr bool is_variant_v = is_variant<T>::value;

  template <typename T>
  concept IsVariantInterface = requires(T t) {
    is_variant_v<T>;
    std::variant_alternative_t<0ul, T>{}.type;
    std::is_same_v<std::string_view,
                   decltype(std::variant_alternative_t<0ul, T>{}.type)>;
  };

  template <typename V, std::size_t Index = std::variant_size_v<V> - 1ul>
    requires IsVariantInterface<V>
  static inline std::optional<V>
  try_find_type(std::string_view type_id) noexcept {
    using AlternativeType = std::variant_alternative_t<Index, V>;
    if (type_id == AlternativeType::type) {
      return AlternativeType{};
    }

    if constexpr (0ul < Index) {
      return try_find_type<V, Index - 1ul>(type_id);
    }

    if constexpr (0ul == Index) {
      return std::nullopt;
    }
  }

  template <typename V, std::size_t Index = std::variant_size_v<V> - 1ul>
    requires IsVariantInterface<V>
  static inline std::optional<std::string_view>
  try_validate_type(std::string_view type_id) noexcept {
    using AlternativeType = std::variant_alternative_t<Index, V>;
    if (type_id == AlternativeType::type) {
      return AlternativeType::type;
    }

    if constexpr (0ul < Index) {
      return try_validate_type<V, Index - 1ul>(type_id);
    }

    if constexpr (0ul == Index) {
      return std::nullopt;
    }
  }

  template <typename V>
    requires IsVariantInterface<V>
  using ArrayOfTypeIds = std::array<std::string_view, std::variant_size_v<V>>;

  template <typename V, std::size_t Index = std::variant_size_v<V> - 1ul>
    requires IsVariantInterface<V>
  static inline consteval void
  gather_type_ids(ArrayOfTypeIds<V>& array_of_types) noexcept {
    array_of_types.at(Index) = std::variant_alternative_t<Index, V>::type;

    if constexpr (0ul < Index) {
      gather_type_ids<V, Index - 1ul>(array_of_types);
    }

    if constexpr (0ul == Index) {
      return;
    }
  }

  template <typename V>
    requires IsVariantInterface<V>
  static inline consteval ArrayOfTypeIds<V> known_ids() noexcept {
    ArrayOfTypeIds<V> ids;
    gather_type_ids<V>(ids);
    return ids;
  }
} // namespace ymwm::common
