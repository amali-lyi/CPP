#include <iostream>
using namespace std;

template <typename T>
class SmartArray
{
private:

    T* data;               
    size_t size;            
    size_t capacity;        

    static size_t objectCount; 

    void ResizeInternal(size_t newCapacity)
    {
        T* newData = new T[newCapacity];

        for (size_t i = 0; i < size; i++)
            newData[i] = std::move(data[i]);

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:

    SmartArray() : data(nullptr), size(0), capacity(0)
    {
        objectCount++;
    }

    explicit SmartArray(size_t size) : size(size), capacity(size)
    {
        data = new T[size]{};
        objectCount++;
    }

    SmartArray(size_t size, const T& value) : size(size), capacity(size)
    {
        data = new T[size];
        for (size_t i = 0; i < size; i++)
            data[i] = value;
        objectCount++;
    }

    SmartArray(const SmartArray& other)
    {
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];

        for (size_t i = 0; i < size; i++)
            data[i] = other.data[i];

        objectCount++;
    }

    SmartArray(SmartArray&& other) noexcept
    {
        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        objectCount++;
    }

    ~SmartArray()
    {
        delete[] data;
        objectCount--;
    }

    SmartArray& operator=(const SmartArray& other)
    {
        if (this == &other) return *this;

        delete[] data;

        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];

        for (size_t i = 0; i < size; i++)
            data[i] = other.data[i];

        return *this;
    }

    SmartArray& operator=(SmartArray&& other) noexcept
    {
        if (this == &other) return *this;

        delete[] data;

        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        return *this;
    }

    T& operator[](size_t index) { return data[index]; }

    const T& operator[](size_t index) const { return data[index]; }

    T& Front()
    {
        if (Empty())
            throw std::out_of_range("Front() - array is empty");
        return data[0];
    }

    T& Back()
    {
        if (Empty())
            throw std::out_of_range("Back() - array is empty");
        return data[size - 1];
    }

    const T& Front() const
    {
        if (Empty())
            throw std::out_of_range("Front() - array is empty");
        return data[0];
    }

    const T& Back() const
    {
        if (Empty())
            throw std::out_of_range("Back() - array is empty");
        return data[size - 1];
    }

    T* Data() { return data; }

    const T* Data() const { return data; }

    size_t Size() const { return size; }

    size_t Capacity() const { return capacity; }

    bool Empty() const { return size == 0; }

    void Clear() { size = 0; }

    void Resize(size_t newSize)
    {
        if (newSize > capacity)
            ResizeInternal(newSize);

        if (newSize > size)
        {
            for (size_t i = size; i < newSize; i++)
                data[i] = T();
        }

        size = newSize;
    }

    void Reserve(size_t newCapacity)
    {
        if (newCapacity > capacity)
            ResizeInternal(newCapacity);
    }

    void ShrinkToFit()
    {
        if (size < capacity)
            ResizeInternal(size);
    }

    void PushBack(const T& value)
    {
        if (size >= capacity)
            ResizeInternal(capacity == 0 ? 1 : capacity * 2);

        data[size++] = value;
    }

    void PushBack(T&& value)
    {
        if (size >= capacity)
            ResizeInternal(capacity == 0 ? 1 : capacity * 2);

        data[size++] = std::move(value);
    }

    void PopBack()
    {
        if (size > 0)
            size--;
    }

    void Insert(size_t index, const T& value)
    {
        if (index > size) return;

        if (size >= capacity)
            ResizeInternal(capacity == 0 ? 1 : capacity * 2);

        for (size_t i = size; i > index; i--)
            data[i] = std::move(data[i - 1]);

        data[index] = value;
        size++;
    }

    void RemoveAt(size_t index)
    {
        if (index >= size) return;

        for (size_t i = index; i < size - 1; i++)
            data[i] = std::move(data[i + 1]);

        size--;
    }

    void Remove(const T& value)
    {
        int index = Find(value);
        if (index != -1)
            RemoveAt(index);
    }

    void Swap(SmartArray& other)
    {
        std::swap(data, other.data);
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);
    }

    void Fill(const T& value)
    {
        for (size_t i = 0; i < size; i++)
            data[i] = value;
    }

    int Find(const T& value) const
    {
        for (size_t i = 0; i < size; i++)
            if (data[i] == value)
                return (int)i;

        return -1;
    }

    bool Contains(const T& value) const
    {
        return Find(value) != -1;
    }

    int Count(const T& value) const
    {
        int count = 0;
        for (size_t i = 0; i < size; i++)
            if (data[i] == value)
                count++;
        return count;
    }

    T* begin() { return data; }

    T* end() { return data + size; }

    const T* begin() const { return data; }

    const T* end() const { return data + size; }

    SmartArray operator+(const SmartArray& other) const
    {
        SmartArray result(size + other.size);

        for (size_t i = 0; i < size; i++)
            result.data[i] = data[i];

        for (size_t i = 0; i < other.size; i++)
            result.data[size + i] = other.data[i];

        return result;
    }

    SmartArray operator-(const SmartArray& other) const
    {
        SmartArray result;

        for (size_t i = 0; i < size; i++)
            if (!other.Contains(data[i]))
                result.PushBack(data[i]);

        return result;
    }

    SmartArray operator*(const T& value) const
    {
        SmartArray result(size);

        for (size_t i = 0; i < size; i++)
            result.data[i] = data[i] * value;

        return result;
    }

    SmartArray& operator+=(const SmartArray& other)
    {
        for (size_t i = 0; i < other.size; i++)
            PushBack(other.data[i]);
        return *this;
    }

    SmartArray& operator-=(const SmartArray& other)
    {
        for (size_t i = 0; i < other.size; i++)
            Remove(other.data[i]);
        return *this;
    }

    bool operator==(const SmartArray& other) const
    {
        if (size != other.size) return false;

        for (size_t i = 0; i < size; i++)
            if (data[i] != other.data[i])
                return false;

        return true;
    }

    bool operator!=(const SmartArray& other) const
    {
        return !(*this == other);
    }

    bool operator<(const SmartArray& other) const
    {
        return size < other.size;
    }

    bool operator>(const SmartArray& other) const
    {
        return size > other.size;
    }

    bool operator<=(const SmartArray& other) const
    {
        return !(*this > other);
    }

    bool operator>=(const SmartArray& other) const
    {
        return !(*this < other);
    }

    bool operator!() const
    {
        return Empty();
    }

    static size_t GetObjectCount()
    {
        return objectCount;
    }

    template <typename U>
    void Transform(SmartArray<U>& out) const
    {
        out.Clear();
        out.Reserve(size);

        for (size_t i = 0; i < size; i++)
            out.PushBack(static_cast<U>(data[i]));
    }

    template <typename U>
    friend ostream& operator<<(ostream& out, const SmartArray<U>& arr);

    template <typename U>
    friend istream& operator>>(istream& in, SmartArray<U>& arr);
};

template <typename T>
size_t SmartArray<T>::objectCount = 0;

template <typename U>
ostream& operator<<(ostream& out, const SmartArray<U>& arr)
{
    for (size_t i = 0; i < arr.size; i++)
        out << arr.data[i] << " ";
    return out;
}

template <typename U>
istream& operator>>(istream& in, SmartArray<U>& arr)
{
    size_t n;
    in >> n;

    arr.Resize(n);

    for (size_t i = 0; i < n; i++)
        in >> arr.data[i];

    return in;
}

int main()
{
    SmartArray<int> arr;

    arr.PushBack(1);
    arr.PushBack(2);
    arr.PushBack(3);

    cout << arr << endl;

    return 0;
}