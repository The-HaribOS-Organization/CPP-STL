#include "memory.hpp"


namespace std
{
    // default_delete
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


    // unique_ptr
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
    constexpr void assert_nothrow_copy_constructible() noexcept
    {
        static_assert(is_nothrow_copy_constructible_v<Deleter>, "The deleter copy constructor must exist and be marked as noexcept.");
    }


    template<class Deleter>
    constexpr void assert_nothrow_move_constructible() noexcept
    {
        static_assert(is_nothrow_move_constructible_v<Deleter>, "The deleter move constructor must exist and be marked as noexcept.");
    }

    template<class T, class Deleter>
    template<NonReferenceType A>
    unique_ptr<T, Deleter>::unique_ptr(pointer p, const A &d) noexcept
    {
        assert_nothrow_copy_constructible<Deleter>();
        m_element = p;
        m_deleter = forward<Deleter>(d);
    }

    template<class T, class Deleter>
    template<NonReferenceType A>
    unique_ptr<T, Deleter>::unique_ptr(pointer p, A &&d) noexcept
    {
        assert_nothrow_move_constructible<Deleter>();
        m_element = p;
        m_deleter = forward<Deleter>(d);
    }

    template<class T, class Deleter>
    template<LValueReferenceType A>
    unique_ptr<T, Deleter>::unique_ptr(pointer p, A &d) noexcept
    {
        assert_nothrow_copy_constructible<Deleter>();
        m_element = p;
        m_deleter = forward<Deleter>(d);
    }

    template<class T, class Deleter>
    template<RValueReferenceType A>
    unique_ptr<T, Deleter>::unique_ptr(pointer p, const A& d) noexcept
    {
        assert_nothrow_move_constructible<Deleter>();
        m_element = p;
        m_deleter = forward<Deleter>(d);
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter>::unique_ptr(unique_ptr &&u) noexcept
    {
        static_assert(is_move_constructible_v<Deleter>, "Deleter must be move constructible.");

        if constexpr (!is_reference<Deleter>)
        {
            assert_nothrow_move_constructible<Deleter>();
        }

        m_element = u.m_element;
        m_deleter = move(u.m_deleter);
        u = nullptr;
    }

    template<class T, class Deleter>
    template<class U, class E>
    unique_ptr<T, Deleter>::unique_ptr(unique_ptr<U, E>&& u) noexcept
    {
        static_assert(is_convertible_v<typename unique_ptr<U, E>::pointer, pointer>, "daz");
        static_assert(!is_same_v<T, T[]>, "Cannot transfer the provided value to an array.");
        static_assert
        (
                is_reference<Deleter> && is_same_v<E, Deleter> ||
                !is_reference<Deleter> && is_convertible_v<E, Deleter>,
                "Either Deleter must be a reference type and E the same type as Deleter or "
                "Deleter must not be a reference type and E convertible to Deleter"
        );

        m_element = move(u.m_element);

        if constexpr (is_reference<E>)
        {
            assert_nothrow_copy_constructible<E>();
            m_deleter = u.m_deleter;
        }
        else
        {
            assert_nothrow_move_constructible<Deleter>();
            m_deleter = move(u.m_deleter);
        }
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter> &unique_ptr<T, Deleter>::operator=(unique_ptr &&r) noexcept {

        return *this;
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter>::~unique_ptr()
    {
        if (get() != nullptr)
        {
            static_assert(noexcept(get_deleter()(get())), "Deleter must not throw an exception.");
        }
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter>::pointer unique_ptr<T, Deleter>::get() const noexcept
    {
        return m_element;
    }

    template<class T, class Deleter>
    Deleter &unique_ptr<T, Deleter>::get_deleter() noexcept
    {
        return &m_deleter;
    }

    template<class T, class Deleter>
    const Deleter &unique_ptr<T, Deleter>::get_deleter() const noexcept
    {
        return &m_deleter;
    }

}