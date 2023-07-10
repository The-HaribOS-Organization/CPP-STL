#include "string.hpp"


// Constructors
std::string::string(char* str) noexcept
        : m_content(str)
{
}


// Operators
char& std::string::operator[](size_t pos) noexcept
{
    return pos >= size() ? *static_cast<char*>(0) : m_content[pos];
}

char const& std::string::operator[](size_t pos) const noexcept
{
    return pos >= size() ? *static_cast<char*>(0) : m_content[pos];
}

bool std::operator==(const string& lhs, const string& rhs) noexcept
{
    if (lhs.size() != rhs.size()) return false;

    for (size_t i = 0; i < lhs.size(); i++)
    {
        if (rhs[i] != lhs[i])
        {
            return false;
        }
    }

    return true;
}


// Methods | Member functions
char& std::string::at(size_t pos)
{
    return this->operator[](pos);
}

size_t std::string::size() const noexcept
{
    size_t size{ 0 };

    while (m_content[size] != '\0')
    {
        size++;
    }

    return size;
}