#include "type_traits.hpp"

template<class T, T v>
constexpr std::integral_constant<T, v>::operator value_type&() const noexcept {
    return value;
}

template<class T, T v>
constexpr std::integral_constant<T, v>::value_type std::integral_constant<T, v>::operator()() const noexcept {
    return value;
}