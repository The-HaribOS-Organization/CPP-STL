#include "memory.hpp"


namespace std
{
    template<class T>
    template<class U>
    default_delete<T>::default_delete(const default_delete<U> &d) noexcept
    {
        static_assert(is_same_v<T*, U*>, "The right class cannot be converted to the left class (cast feature not implemented yet).");
    }

    template<class T>
    template<class U>
    default_delete<T>::default_delete(const default_delete<U[]> &d) noexcept
    {
        static_assert(is_same_v<T(*)[], U(*)[]>, "The right class cannot be converted to the left class (cast feature not implemented yet).");
    }

    template<class T>
    void default_delete<T>::operator()(T *ptr) const
    {
        delete ptr;
    }

    template<class T>
    template<class U>
    void default_delete<T>::operator()(U* ptr) const
    {
        delete[] ptr;
    }


    template<class Deleter>
    constexpr void filter()
    {
        static_assert(is_nothrow_default_constructible_v<Deleter>, "Deleter type default constructor should be marked as noexcept");
        static_assert(!is_pointer_v<Deleter>, "Deleter type cannot be a pointer");
    }

    template<class T, class Deleter>
    constexpr unique_ptr<T, Deleter>::unique_ptr()
    {
        filter<Deleter>();

        m_element = nullptr;
        m_deleter = {};
    }

    template<class T, class Deleter>
    constexpr unique_ptr<T, Deleter>::unique_ptr(nullptr_t) noexcept : unique_ptr()
    {
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter>::unique_ptr(pointer p) noexcept
    {
        filter<Deleter>();
        m_element = p;
        m_deleter = {};
    }

    template<class Deleter>
    constexpr void assert_copy_constructible() noexcept
    {
        static_assert(is_nothrow_copy_constructible_v<Deleter>, "The deleter copy constructor must exist and be marked as noexcept.");
    }


    template<class Deleter>
    constexpr void assert_move_constructible() noexcept
    {
        static_assert(is_nothrow_move_constructible_v<Deleter>, "The deleter move constructor must exist and be marked as noexcept.");
    }

    template<class T, class Deleter>
    template<NonReferenceType A>
    unique_ptr<T, Deleter>::unique_ptr(pointer p, const A &d) noexcept
    {
        assert_copy_constructible<Deleter>();
        m_element = p;
        m_deleter = d;
    }

    template<class T, class Deleter>
    template<NonReferenceType A>
    unique_ptr<T, Deleter>::unique_ptr(pointer p, A &&d) noexcept
    {
        assert_move_constructible<Deleter>();
        m_element = p;
        m_deleter = d;
    }

    template<class T, class Deleter>
    template<LValueReferenceType A>
    unique_ptr<T, Deleter>::unique_ptr(pointer p, A &d) noexcept
    {
        assert_copy_constructible<Deleter>();
        m_element = p;
        m_deleter = d;
    }

    template<class T, class Deleter>
    template<RValueReferenceType A>
    unique_ptr<T, Deleter>::unique_ptr(pointer p, const A& d) noexcept
    {
        assert_move_constructible<Deleter>();
        m_element = p;
        m_deleter = d;
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter>::unique_ptr(unique_ptr &&u) noexcept
    {
        static_assert(is_move_constructible_v<Deleter>, "Deleter must be move constructible.");

        if constexpr (!is_reference<Deleter>)
        {
            assert_move_constructible<Deleter>();
        }

        *this = move(u);
        u = nullptr;
    }

    template<class T, class Deleter>
    template<class U, class E>
    unique_ptr<T, Deleter>::unique_ptr(unique_ptr<U, E>&& u) noexcept
    {
        if constexpr (is_reference<E>)
        {
            assert_copy_constructible<E>();
            m_deleter = u.m_deleter;
        }
        else
        {
            assert_move_constructible<Deleter>();
            m_deleter = move(u.m_deleter);
        }
    }


}