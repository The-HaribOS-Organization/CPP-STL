#include "functional.hpp"


namespace std
{
    template<class T>
    constexpr bool less<T>::operator()(const T &lhs, const T &rhs) const {
        return lhs < rhs;
    }
}