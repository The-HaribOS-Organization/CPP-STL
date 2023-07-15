#include "memory.hpp"
#include "functional.hpp"


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
    // Functions
    template<class Deleter>
    constexpr void filter()
    {
        static_assert(is_nothrow_default_constructible_v<Deleter>, "Deleter type default constructor should be marked as noexcept");
        static_assert(!is_pointer_v<Deleter>, "Deleter type cannot be a pointer");
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

    // Constructors implementations
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
        static_assert(!is_array_v<T>, "Cannot transfer the provided value to an array.");
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

    // Destructor
    template<class T, class Deleter>
    unique_ptr<T, Deleter>::~unique_ptr()
    {
        if (get())
        {
            static_assert(noexcept(get_deleter()(get())), "Deleter must not throw an exception.");
        }
    }

    // Operators
    template<class T, class Deleter>
    template<class U, class E>
    void unique_ptr<T, Deleter>::transfer_ownership_from(unique_ptr<U, E>&& r)
    {
        reset(r.release());

        if (is_reference<Deleter>)
            static_assert(is_nothrow_copy_assignable_v<remove_reference_t<Deleter>>, "Deleter must be copy assignable and marked as noexcept.");
        else
            static_assert(is_nothrow_move_assignable_v<Deleter>, "Deleter must be move assignable.");

        get_deleter() = forward<Deleter>(r.get_deleter());
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter>& unique_ptr<T, Deleter>::operator=(unique_ptr&& r) noexcept
    {
        transfer_ownership_from<T, Deleter>(r);
        return *this;
    }

    template<class T, class Deleter>
    template<class U, class E>
    unique_ptr<T, Deleter>& unique_ptr<T, Deleter>::operator=(unique_ptr<U,E>&& r) noexcept
    {
        static_assert(!is_array_v<U>, "The provided element cannot be an array.");
        static_assert(is_convertible_v<typename unique_ptr<U,E>::pointer, pointer>, "The provided class pointer cannot be converted into this class pointer.");
        static_assert(is_assignable_v<Deleter&, E&&>, "The provided Deleter cannot be move assigned to this class Deleter.");

        transfer_ownership_from(r);
        return *this;
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter> &unique_ptr<T, Deleter>::operator=(nullptr_t) noexcept
    {
        reset();
        return *this;
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter>::operator bool() const noexcept
    {
        return get();
    }

    template<class T, class Deleter>
    add_lvalue_reference_t<T> unique_ptr<T, Deleter>::operator*() const noexcept(noexcept(*declval<pointer>()))
    {
        return *get();
    }

    template<class T, class Deleter>
    unique_ptr<T, Deleter>::pointer unique_ptr<T, Deleter>::operator->() const noexcept
    {
        return get();
    }

    // Member functions
    template<class T, class Deleter>
    unique_ptr<T, Deleter>::pointer unique_ptr<T, Deleter>::release() noexcept
    {
        if (m_element)
        {
            auto old_ptr = m_element;
            m_element = nullptr;
            return old_ptr;
        }

        return nullptr;
    }

    template<class T, class Deleter>
    void unique_ptr<T, Deleter>::reset(pointer ptr) noexcept
    {
        auto old_ptr = m_element;
        m_element = ptr;
        if (old_ptr) get_deleter()(old_ptr);
    }

    template<class T, class Deleter>
    void unique_ptr<T, Deleter>::swap(unique_ptr &other) noexcept
    {
        auto old_ptr = *this;
        *this = other;
        other = old_ptr;
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

    /// Extern functions
    template<class T, class... Args>
    unique_ptr<T> make_unique(Args&&... args)
    {
        static_assert(!is_array_v<T>);
        return unique_ptr<T>(new T(forward<Args>(args)...));
    }

    template<class T>
    unique_ptr<T> make_unique_for_overwrite()
    {
        return unique_ptr<T>(new T);
    }

    template<class T, class D>
    void swap(unique_ptr<T, D>& lhs, unique_ptr<T, D>& rhs) noexcept
    {
        static_assert(is_swappable_v<D>, "The Deleter must be swappable.");
        lhs.swap(rhs);
    }


    /// Extern operators
    //  Comparison between unique_ptrs
    template<class T1, class D1, class T2, class D2>
    bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
    {
        return x.get() == y.get();
    }

    template<class T1, class D1, class T2, class D2>
    bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
    {
        return !(x == y);
    }

    template<class T1, class D1, class T2, class D2>
    bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
    {
        // TODO: Implement that operator.
        // less<common_type<unique_ptr<T1, D1>::pointer, unique_ptr<T2, D2>::pointer>::type>()(x.get(), y.get())
    }

    template<class T1, class D1, class T2, class D2>
    bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
    {
        return !(y < x);
    }

    template<class T1, class D1, class T2, class D2>
    bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
    {
        return y < x;
    }

    template<class T1, class D1, class T2, class D2>
    bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
    {
        return !(x < y);
    }

    //  Comparison between unique_ptr and nullptr
    template<class T, class D>
    bool operator==(const unique_ptr<T, D>& x, nullptr_t) noexcept
    {
        return !x;
    }

    template<class T, class D>
    bool operator==(nullptr_t, const unique_ptr<T, D>& x) noexcept
    {
        return !x;
    }

    template<class T, class D>
    bool operator!=(const unique_ptr<T, D>& x, nullptr_t) noexcept
    {
        return (bool)x;
    }

    template<class T, class D>
    bool operator!=(nullptr_t, const unique_ptr<T, D>& x) noexcept
    {
        return (bool)x;
    }

    template<class T, class D>
    bool operator<(const unique_ptr<T, D>& x, nullptr_t)
    {
        return less<typename unique_ptr<T, D>::pointer>()(x.get(), nullptr);
    }

    template<class T, class D>
    bool operator<(nullptr_t, const unique_ptr<T, D>& y)
    {
        return less<typename unique_ptr<T, D>::pointer>()(nullptr, y.get());
    }

    template<class T, class D>
    bool operator<=(const unique_ptr<T, D>& x, nullptr_t)
    {
        return !(nullptr < x);
    }

    template<class T, class D>
    bool operator<=(nullptr_t, const unique_ptr<T, D>& y)
    {
        return !(y < nullptr);
    }

    template<class T, class D>
    bool operator>(const unique_ptr<T, D>& x, nullptr_t)
    {
        return nullptr < x;
    }

    template<class T, class D>
    bool operator>(nullptr_t, const unique_ptr<T, D>& y)
    {
        return y < nullptr;
    }

    template<class T, class D>
    bool operator>=(const unique_ptr<T, D>& x, nullptr_t)
    {
        return !(x < nullptr);
    }

    template<class T, class D>
    bool operator>=(nullptr_t, const unique_ptr<T, D>& y)
    {
        return !(nullptr < y);
    }

}