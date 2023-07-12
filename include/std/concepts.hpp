//
// Created by Arthur on 12/07/2023.
//

#ifndef CPP_CONCEPTS_HPP
#define CPP_CONCEPTS_HPP


#include "type_traits.hpp"

namespace std
{
    template<class T, class U>
    concept same_as = is_same_v<T, U>;
}


#endif //CPP_CONCEPTS_HPP
