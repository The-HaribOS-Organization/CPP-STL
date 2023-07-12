#include "type_traits.hpp"

namespace std
{
    template<class T, T v>
    constexpr integral_constant<T, v>::operator value_type&() const noexcept {
        return value;
    }

    template<class T, T v>
    constexpr integral_constant<T, v>::value_type integral_constant<T, v>::operator()() const noexcept {
        return value;
    }
}