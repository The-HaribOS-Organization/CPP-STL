//
// Created by Arthur on 12/07/2023.
//

#ifndef CPP_ITERATOR_HPP
#define CPP_ITERATOR_HPP


#include "concepts.hpp"

namespace std
{
    // Iterator categories
    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag : public input_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };
    struct contiguous_iterator_tag : public random_access_iterator_tag { };

    // Category filter
    template<class Category>
    concept CorrectCategory = same_as<Category, input_iterator_tag>
                                    || same_as<Category, output_iterator_tag>
                                    || same_as<Category, forward_iterator_tag>
                                    || same_as<Category, bidirectional_iterator_tag>
                                    || same_as<Category, random_access_iterator_tag>
                                    || same_as<Category, contiguous_iterator_tag>;

    // Iterator struct
    template<
            CorrectCategory Category,
            class T,
            class Pointer = T*,
            class Reference = T&
    >
    struct iterator
    {
        using value_type = T;
        using pointer = Pointer;
        using reference = Reference;
        using iterator_category = Category;
    };
}


#endif //CPP_ITERATOR_HPP
