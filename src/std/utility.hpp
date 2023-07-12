#ifndef CPLIPARSER_UTILITY_HPP
#define CPLIPARSER_UTILITY_HPP


#include "type_traits.hpp"

namespace std
{
    template<class T>
    std::remove_reference_t<T>&& move(T&& var) noexcept;
}


#endif //CPLIPARSER_UTILITY_HPP
