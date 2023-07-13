#ifndef CLIPARSER_MEMORY_HPP
#define CLIPARSER_MEMORY_HPP


#include "cstddef.hpp"
#include "type_traits.hpp"

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
        using pointer = decltype(remove_reference<Deleter>::type::pointer(), T::pointer());
        using element_type = T;
        using deleter_type = Deleter;

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
        unique_ptr(unique_ptr<U, E>&& u) noexcept;
    private:
        element_type* m_element;
        deleter_type m_deleter;
    };
}


#endif //CLIPARSER_MEMORY_HPP
