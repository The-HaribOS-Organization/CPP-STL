#ifndef CLIPARSER_TYPE_TRAITS_HPP
#define CLIPARSER_TYPE_TRAITS_HPP


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

        constexpr operator value_type&() const noexcept;
        constexpr value_type operator()() const noexcept;

        static constexpr T value{ v };
    };

    template<bool B>
    using bool_constant = integral_constant<bool, B>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<true>;

    template<class T>
    constexpr bool always_false = false;


    // is_same
    template<class T, class U>
    struct is_same : false_type {};

    template<class T>
    struct is_same<T, T> : true_type {};

    template<class T, class U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    template<class T>
    add_rvalue_reference<T> declval() noexcept;


    // is_assignable
    template<typename T, typename U, typename = void>
    struct is_assignable : false_type {};

    template<typename T, typename U>
    struct is_assignable<T, U, decltype(declval<T>() = declval<U>(), void())> : true_type {};

    template<typename T, typename U>
    using is_assignable_v = is_assignable<T, U>::value;


    // is_copy_assignable
    template<class T>
    struct is_copy_assignable : is_assignable<add_lvalue_reference<T>, add_lvalue_reference<const T>> {};

}


#endif //CLIPARSER_TYPE_TRAITS_HPP