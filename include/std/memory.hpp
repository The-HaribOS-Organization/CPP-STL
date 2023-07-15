#ifndef STL_MEMORY_HPP
#define STL_MEMORY_HPP


#include "cstddef.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

namespace std
{
    template<class T>
    struct default_delete
    {
        constexpr default_delete() noexcept = default;
        template<class U>
        explicit default_delete(const default_delete<U>& d) noexcept;
        template<class U>
        explicit default_delete(const default_delete<U[]>& d) noexcept;

        void operator()(T* ptr) const;

        template<class U>
        void operator()(U* ptr) const;
    };

    template<class A>
    concept NonReferenceType = !is_reference<A>;

    template<class A>
    concept LValueReferenceType = is_lvalue_reference_v<A>;

    template<class A>
    concept RValueReferenceType = is_rvalue_reference_v<A>;

    template<class T, class Deleter = default_delete<T>>
    class unique_ptr
    {
    public:
        /// Member types
        using pointer = decltype(remove_reference<Deleter>::type::pointer(), T::pointer());
        using element_type = T;
        using deleter_type = Deleter;

        /// Constructors
        constexpr unique_ptr();
        constexpr explicit unique_ptr(nullptr_t ptr) noexcept;
        explicit unique_ptr(pointer p) noexcept;

        template<NonReferenceType A>
        unique_ptr(pointer p, const A& d) noexcept;
        template<NonReferenceType A>
        unique_ptr(pointer p, A&& d) noexcept;

        template<LValueReferenceType A>
        unique_ptr(pointer p, A& d) noexcept;
        template<LValueReferenceType A>
        unique_ptr(pointer p, A&& d) = delete;

        template<RValueReferenceType A>
        unique_ptr(pointer p, const A& d) noexcept;
        template<RValueReferenceType A>
        unique_ptr(pointer p, const A&& d) = delete;

        unique_ptr(unique_ptr&& u) noexcept;

        template<class U, class E>
        explicit unique_ptr(unique_ptr<U, E>&& u) noexcept;

        /// Destructor
        ~unique_ptr();

        /// Operators
        unique_ptr& operator=(unique_ptr&& r) noexcept;
        template<class U, class E>
        unique_ptr& operator=(unique_ptr<U,E>&& r) noexcept;
        unique_ptr& operator=(nullptr_t) noexcept;

        explicit operator bool() const noexcept;
        add_lvalue_reference_t<T> operator*() const noexcept(noexcept(*declval<pointer>()));
        pointer operator->() const noexcept;

        /// Member functions
        //  Modifiers
        pointer release() noexcept;
        void reset(pointer ptr = pointer()) noexcept;
        void swap(unique_ptr& other) noexcept;

        //  Observers
        pointer get() const noexcept;
        Deleter& get_deleter() noexcept;
        const Deleter& get_deleter() const noexcept;
    private:
        template<class U, class E>
        void transfer_ownership_from(unique_ptr<U, E>&& r);

        element_type* m_element;
        deleter_type m_deleter;
    };

    /// Extern functions
    template<class T, class... Args>
    unique_ptr<T> make_unique(Args&&... args);

    template<class T>
    unique_ptr<T> make_unique_for_overwrite();

    template<class T, class D>
    void swap(unique_ptr<T, D>& lhs, unique_ptr<T, D>& rhs) noexcept;

    /// Extern operators
    //  Comparison between unique_ptrs
    template<class T1, class D1, class T2, class D2>
    bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
    template<class T1, class D1, class T2, class D2>
    bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
    template<class T1, class D1, class T2, class D2>
    bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
    template<class T1, class D1, class T2, class D2>
    bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
    template<class T1, class D1, class T2, class D2>
    bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
    template<class T1, class D1, class T2, class D2>
    bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

    //  Comparison between unique_ptr and nullptr
    template<class T, class D>
    bool operator==(const unique_ptr<T, D>& x, nullptr_t) noexcept;
    template<class T, class D>
    bool operator==(nullptr_t, const unique_ptr<T, D>& x) noexcept;
    template<class T, class D>
    bool operator!=(const unique_ptr<T, D>& x, nullptr_t) noexcept;
    template<class T, class D>
    bool operator!=(nullptr_t, const unique_ptr<T, D>& x) noexcept;
    template<class T, class D>
    bool operator<(const unique_ptr<T, D>& x, nullptr_t);
    template<class T, class D>
    bool operator<(nullptr_t, const unique_ptr<T, D>& y);
    template<class T, class D>
    bool operator<=(const unique_ptr<T, D>& x, nullptr_t);
    template<class T, class D>
    bool operator<=(nullptr_t, const unique_ptr<T, D>& y);
    template<class T, class D>
    bool operator>(const unique_ptr<T, D>& x, nullptr_t);
    template<class T, class D>
    bool operator>(nullptr_t, const unique_ptr<T, D>& y);
    template<class T, class D>
    bool operator>=(const unique_ptr<T, D>& x, nullptr_t);
    template<class T, class D>
    bool operator>=(nullptr_t, const unique_ptr<T, D>& y);

    /// Specialised structs
    //  TODO: Implement std::hash struct specialization for unique_ptr
}


#endif //STL_MEMORY_HPP
