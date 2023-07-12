#include "utility.hpp"

namespace std
{
    template<class T>
    remove_reference_t<T>&& move(T&& var) noexcept
    {
        return static_cast<typename remove_reference_t<T>&&>(var);
    }

    template<class T>
    add_rvalue_reference_t<T> declval() noexcept
    {
        static_assert(stdimpl::always_false<T>, "declval not allowed in an evaluated context");
    }
}