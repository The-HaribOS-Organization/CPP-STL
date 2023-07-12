#include "utility.hpp"

template<class T>
std::remove_reference_t<T>&& move(T&& var) noexcept
{
    return static_cast<typename std::remove_reference_t<T>&&>(var);
}