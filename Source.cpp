#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <cstring>

using namespace std;

class String
{
    char* data;
    size_t length;

public:
    String()
    {
        length = 0;
        data = new char[1];
        data[0] = '\0';
    }

    String(const char* str)
    {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }

    String(const String& other)
    {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
    }

    String(String&& other) noexcept
    {
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
    }

    ~String()
    {
        delete[] data;
    }

    String& operator=(const String& other)
    {
        if (this != &other)
        {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    String& operator=(String&& other) noexcept
    {
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            length = other.length;
            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }

    String operator+(const String& other) const
    {
        String result;
        delete[] result.data;
        result.length = length + other.length;
        result.data = new char[result.length + 1];
        strcpy(result.data, data);
        strcat(result.data, other.data);
        return result;
    }

    String& operator+=(const String& other)
    {
        *this = *this + other;
        return *this;
    }

    bool operator==(const String& other) const
    {
        return strcmp(data, other.data) == 0;
    }

    bool operator!=(const String& other) const
    {
        return !(*this == other);
    }

    bool operator<(const String& other) const
    {
        return strcmp(data, other.data) < 0;
    }

    bool operator>(const String& other) const
    {
        return strcmp(data, other.data) > 0;
    }

    char& operator[](int index)
    {
        return data[index];
    }

    const char& operator[](int index) const
    {
        return data[index];
    }

    bool operator!() const
    {
        return length == 0;
    }

    operator const char* () const
    {
        return data;
    }
};

ostream& operator<<(ostream& os, const String& s)
{
    os << (const char*)s;
    return os;
}

int main()
{
    String s1("Hello");
    String s2(" World");

    cout << s1 << endl;
    cout << s2 << endl;

    String s3 = s1 + s2;
    cout << s3 << endl;

    s1 += s2;
    cout << s1 << endl;

    cout << (s1 == s3) << endl;
    cout << (s1 != s2) << endl;

    cout << (s1 > s2) << endl;
    cout << (s1 < s2) << endl;

    cout << s1[1] << endl;

    s1[0] = 'h';
    cout << s1 << endl;

    String empty;
    cout << !empty << endl;

    return 0;
}