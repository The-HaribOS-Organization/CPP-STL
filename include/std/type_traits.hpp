#ifndef STL_TYPE_TRAITS_HPP
#define STL_TYPE_TRAITS_HPP


#include "cstddef.hpp"

namespace std
{
    // add_(l/r)value_reference
    namespace detail
    {
        template<class T>
        struct type_identity { using type = T; };

        template<class T> // Handle every existing types except T = void because void& creates a compilation error
                          // so the compiler tries the following one
        auto try_add_lvalue_reference(int) -> type_identity<T&>;
        template<class T> // Handle when T = void, creates no void& but only returns void itself
        auto try_add_lvalue_reference(...) -> type_identity<T>;

        template<class T>
        auto try_add_rvalue_reference(int) -> type_identity<T&&>;
        template<class T>
        auto try_add_rvalue_reference(...) -> type_identity<T>;
    }

    template<class T>
    struct add_lvalue_reference
            : decltype(detail::try_add_lvalue_reference<T>(0)) {};

    template<class T>
    struct add_rvalue_reference
            : decltype(detail::try_add_rvalue_reference<T>(0)) {};

    template<class T>
    using add_lvalue_reference_t = add_lvalue_reference<T>::type;

    template<class T>
    using add_rvalue_reference_t = add_rvalue_reference<T>::type;


    // add_const
    template<class T>
    struct add_const { using type = const T; };

    template<class T>
    using add_const_t = add_const<T>::type;


    // remove_reference
    template<class T>
    struct remove_reference { using type = T; };

    template<class T>
    struct remove_reference<T&> { using type = T; };

    template<class T>
    struct remove_reference<T&&> { using type = T; };

    template<class T>
    using remove_reference_t = remove_reference<T>::type;


    // integral_constant
    template<class T, T v>
    struct integral_constant
    {
        using value_type = T;
        using type = integral_constant<T, v>;

        constexpr explicit operator value_type&() const noexcept;
        constexpr value_type operator()() const noexcept;

        static constexpr T value{ v };
    };

    template<bool B>
    using bool_constant = integral_constant<bool, B>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    template<class T>
    constexpr bool always_false = false;


    // is_same
    template<class T, class U>
    struct is_same : false_type {};

    template<class T>
    struct is_same<T, T> : true_type {};

    template<class T, class U>
    inline constexpr bool is_same_v = is_same<T, U>::value;


    // is_lvalue_reference
    template<class T>
    struct is_lvalue_reference : false_type {};

    template<class T>
    struct is_lvalue_reference<T&> : true_type {};

    template<class T>
    inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;


    // is_rvalue_reference
    template<class T>
    struct is_rvalue_reference : false_type {};

    template<class T>
    struct is_rvalue_reference<T&&> : true_type {};

    template<class T>
    inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;


    // is_reference
    template<class T>
    inline constexpr bool is_reference = is_lvalue_reference_v<T> || is_rvalue_reference_v<T>;


    // is_pointer
    template<class T>
    struct is_pointer : false_type {};

    template<class T>
    struct is_pointer<T*> : true_type {};

    template<class T>
    struct is_pointer<T* const> : true_type {};

    template<class T>
    struct is_pointer<T* volatile> : true_type {};

    template<class T>
    struct is_pointer<T* const volatile> : true_type {};

    template<class T>
    inline constexpr bool is_pointer_v = is_pointer<T>::value;


    // is_array
    template<class T>
    struct is_array : false_type {};

    template<class T>
    struct is_array<T[]> : true_type {};

    template<class T, size_t N>
    struct is_array<T[N]> : true_type {};

    template<class T>
    inline constexpr bool is_array_v = is_array<T>::value;


    // convertible_test
    template<class T>
    add_rvalue_reference_t<T> declval() noexcept;

    template<class From, class To>
    constexpr To convertible_test() {
        return declval<From>();
    }


    // is_convertible
    template<class From, class To>
    struct is_convertible : bool_constant< requires {
        convertible_test<From, To>();
    } || is_same_v<From, void> && is_same_v<To, void>> {};

    template<class From, class To>
    inline constexpr bool is_convertible_v = is_convertible<From, To>::value;


    // is_nothrow_convertible
    template<class From, class To>
    struct is_nothrow_convertible : bool_constant< requires {
        noexcept(convertible_test<From, To>());
    } || is_same_v<From, void> && is_same_v<To, void>> {};

    template<class From, class To>
    inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<From, To>::value;


    // is_assignable
    template<typename T, typename U, typename = void>
    struct is_assignable : false_type {};

    template<typename T, typename U>
    struct is_assignable<T, U, decltype(declval<T>() = declval<U>(), void())> : true_type {};

    template<typename T, typename U>
    inline constexpr bool is_assignable_v = is_assignable<T, U>::value;


    // is_trivially_assignable
    template<class T, class U, typename = void>
    struct is_trivially_assignable : false_type {};

    template<class T, class U>
    struct is_trivially_assignable<T, U, decltype(T(declval<U>()), void())> : true_type {};

    template<typename T, typename U>
    inline constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;


    // is_nothrow_assignable
    template<class T, class U, typename = void>
    struct is_nothrow_assignable : false_type {};

    template<class T, class U>
    struct is_nothrow_assignable<T, U, decltype(noexcept(declval<T>() = declval<U>()) ? declval<T>() = declval<U>() : void(), void())> : true_type {};

    template<typename T, typename U>
    inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;


    // is_copy_assignable
    template<class T>
    struct is_copy_assignable : is_assignable<add_lvalue_reference<T>, add_lvalue_reference<const T>> {};

    template<class T>
    inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;


    // is_trivially_copy_assignable
    template<class T>
    struct is_trivially_copy_assignable : is_trivially_assignable<add_lvalue_reference<T>, add_lvalue_reference<const T>> {};

    template<class T>
    inline constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;


    // is_nothrow_copy_assignable
    template<class T>
    struct is_nothrow_copy_assignable : is_nothrow_assignable<add_lvalue_reference<T>, add_lvalue_reference<const T>> {};

    template<class T>
    inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;


    // is_move_assignable
    template<class T>
    struct is_move_assignable : is_assignable<add_lvalue_reference_t<T>, add_rvalue_reference_t<T>> {};

    template<class T>
    inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;


    // is_trivially_move_assignable
    template<class T>
    struct is_trivially_move_assignable : is_trivially_assignable<add_lvalue_reference_t<T>, add_rvalue_reference_t<T>> {};

    template<class T>
    inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;


    // is_nothrow_move_assignable
    template<class T>
    struct is_nothrow_move_assignable : is_nothrow_assignable<add_lvalue_reference_t<T>, add_rvalue_reference_t<T>> {};

    template<class T>
    inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;


    // is_constructible
    template<class T, class... Args>
    struct is_constructible : bool_constant< requires{ T{ declval<Args>()... }; }> {};

    template<class T, class... Args>
    inline constexpr bool is_constructible_t = is_constructible<T, Args...>::type;


    // is_nothrow_constructible
    template<class T, typename = void, class... Args>
    struct is_nothrow_constructible : false_type {};

    template<class T, class... Args>
    struct is_nothrow_constructible<T, decltype(noexcept(T{ declval<Args>()... }) ? T{ declval<Args>()... } : void(), void()), Args...> : true_type {};

    template<class T, class... Args>
    inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;


    // is_move_constructible
    template<class T>
    struct is_move_constructible : is_constructible<T, add_rvalue_reference_t<T>> {};

    template<class T>
    inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;


    // is_nothrow_move_constructible
    template<class T>
    struct is_nothrow_move_constructible : is_constructible<T, add_rvalue_reference_t<T>> {};

    template<class T>
    inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;


    // is_copy_constructible
    template<class T>
    struct is_copy_constructible : is_constructible<T,
        typename add_lvalue_reference<
            typename add_const<T>::type
        >::type
    > {};

    template<class T>
    inline constexpr bool is_copy_constructible_t = is_copy_constructible<T>::type;


    // is_nothrow_copy_constructible
    template<class T>
    struct is_nothrow_copy_constructible : is_nothrow_constructible<T,
            typename add_lvalue_reference<
                    typename add_const<T>::type
            >::type
    > {};

    template<class T>
    inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;


    // is_default_constructible
    template<class T>
    struct is_default_constructible : is_constructible<T> {};

    template<class T>
    inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;


    // is_nothrow_default_constructible
    template<class T>
    struct is_nothrow_default_constructible : is_nothrow_constructible<T> {};

    template<class T>
    inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;
}


#endif //STL_TYPE_TRAITS_HPP
