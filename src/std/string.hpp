#ifndef CLIPARSER_STRING_HPP
#define CLIPARSER_STRING_HPP


namespace std
{
    class string
    {
    public:
        // Constructors
        explicit string(char* str) noexcept;
        string(const string& string) noexcept = default;
        string& operator=(const string& string) noexcept = default;
        string(string&& string) noexcept = default;
        string& operator=(string&& string) noexcept = default;


        // Operators
        char& operator[](size_t pos) noexcept;
        char const& operator[](size_t pos) const noexcept;
        friend bool operator==(const string& lhs, const string& rhs) noexcept;


        // Methods | Member functions
        char& at(size_t pos);
        size_t size() const noexcept;
    private:
        char* m_content;
    };
}


#endif