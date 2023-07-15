#ifndef STL_FUNCTIONAL_HPP
#define STL_FUNCTIONAL_HPP


namespace std
{
    template<class T = void>
    struct less
    {
        constexpr bool operator()(const T& lhs, const T& rhs) const;
    };

    // TODO: Implement std::hash
}


#endif //STL_FUNCTIONAL_HPP