#ifndef CPLIPARSER_UTILITY_HPP
#define CPLIPARSER_UTILITY_HPP


#include "type_traits.hpp"

namespace std
{
    template<class T>
    remove_reference_t<T>&& move(T&& var) noexcept;

    template<class T>
    add_rvalue_reference_t<T> declval() noexcept;
}


#endif //CPLIPARSER_UTILITY_HPP
