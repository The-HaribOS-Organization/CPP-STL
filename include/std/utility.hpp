#ifndef STL_UTILITY_HPP
#define STL_UTILITY_HPP


#include "type_traits.hpp"

namespace std
{
    template<class T>
    remove_reference_t<T>&& move(T&& var) noexcept;

    template<class T>
    add_rvalue_reference_t<T> declval() noexcept;

    template<class T>
    constexpr T&& forward(remove_reference_t<T>& t) noexcept;

    template<class T>
    constexpr T&& forward(remove_reference_t<T>&& t) noexcept;

    template<class T>
    constexpr void swap(T& a, T& b) noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_move_assignable_v<T>);
}


#endif //STL_UTILITY_HPP
