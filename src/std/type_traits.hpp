#ifndef CLIPARSER_TYPE_TRAITS_HPP
#define CLIPARSER_TYPE_TRAITS_HPP


namespace std
{
    // remove_reference struct
    template<class T>
    struct remove_reference { using type = T; };

    template<class T>
    struct remove_reference<T&> { using type = T; };

    template<class T>
    struct remove_reference<T&&> { using type = T; };

    template<class T>
    using remove_reference_t = remove_reference<T>::type;

    // integral_constant struct
    template<class T, T v>
    struct integral_constant
    {
        using value_type = T;
        using type = std::integral_constant<T, v>;

        constexpr operator value_type&() const noexcept;
        constexpr value_type operator()() const noexcept;

        static constexpr T value{ v };
    };

    template<bool B>
    using bool_constant = integral_constant<bool, B>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<true>;


    // is_same
    template<class T, class U>
    struct is_same : std::false_type {};

    template<class T>
    struct is_same<T, T> : std::true_type {};

    template< class T, class U >
    inline constexpr bool is_same_v = is_same<T, U>::value;
}


#endif //CLIPARSER_TYPE_TRAITS_HPP