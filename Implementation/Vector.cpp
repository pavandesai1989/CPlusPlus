#include <iostream>
#include <stdexcept>
#include <utility>  // for std::move

template<typename T>
class MyVector {
private:
    T* m_data = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];  // uses constructor

        for (size_t i = 0; i < m_size; ++i)
            new_data[i] = std::move(m_data[i]);  // move elements

        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }

public:
    MyVector() = default;

    explicit MyVector(size_t count, const T& value = T()) {
        m_data = new T[count];
        for (size_t i = 0; i < count; ++i)
            m_data[i] = value;
        m_size = m_capacity = count;
    }

    ~MyVector() {
        delete[] m_data;
    }

    MyVector(const MyVector& other) {
        m_data = new T[other.m_capacity];
        for (size_t i = 0; i < other.m_size; ++i)
            m_data[i] = other.m_data[i];
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] m_data;
            m_data = new T[other.m_capacity];
            for (size_t i = 0; i < other.m_size; ++i)
                m_data[i] = other.m_data[i];
            m_size = other.m_size;
            m_capacity = other.m_capacity;
        }
        return *this;
    }

    MyVector(MyVector&& other) noexcept
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = other.m_capacity = 0;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_data = nullptr;
            other.m_size = other.m_capacity = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        if (m_size == m_capacity)
            reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
        m_data[m_size++] = value;
    }

    void push_back(T&& value) {
        if (m_size == m_capacity)
            reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
        m_data[m_size++] = std::move(value);
    }

    void pop_back() {
        if (m_size == 0)
            throw std::out_of_range("Vector is empty");
        --m_size;
    }

    void clear() {
        m_size = 0;
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }

    T& operator[](size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    T* begin() { return m_data; }
    T* end() { return m_data + m_size; }
    const T* begin() const { return m_data; }
    const T* end() const { return m_data + m_size; }
};


int main() {
    MyVector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    std::cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << "\n";

    for (auto val : v)
        std::cout << val << " ";
    std::cout << "\n";

    v.pop_back();
    std::cout << "After pop, size = " << v.size() << "\n";
}

| Feature                         | `new[]`         | `operator new` + placement |
| ------------------------------- | --------------- | -------------------------- |
| Simpler syntax                  | ✅ Yes           | ❌ No (more verbose)        |
| Calls constructor               | ✅ Yes           | ❌ You call it manually     |
| Destructor automatically called | ✅ on `delete[]` | ❌ You must do it manually  |
| Control over allocation         | ❌ Less flexible | ✅ More control             |
