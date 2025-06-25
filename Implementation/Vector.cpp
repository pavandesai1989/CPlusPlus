#include <iostream>
#include <stdexcept>
#include <memory>     // for std::uninitialized_copy, std::allocator
#include <utility>    // for std::move, std::swap

template<typename T>
class MyVector {
private:
    T* m_data = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;

    void reallocate(size_t new_capacity) {
        T* new_data = static_cast<T*>(operator new(sizeof(T) * new_capacity));

        // Move old elements to new buffer
        for (size_t i = 0; i < m_size; ++i)
            new (new_data + i) T(std::move(m_data[i]));

        // Destroy old elements
        for (size_t i = 0; i < m_size; ++i)
            m_data[i].~T();

        // Release old buffer
        operator delete(m_data);

        m_data = new_data;
        m_capacity = new_capacity;
    }

public:
    MyVector() = default;

    explicit MyVector(size_t count, const T& value = T()) {
        m_data = static_cast<T*>(operator new(sizeof(T) * count));
        for (size_t i = 0; i < count; ++i)
            new (m_data + i) T(value);
        m_size = m_capacity = count;
    }

    // Destructor
    ~MyVector() {
        clear();
        operator delete(m_data);
    }

    // Copy constructor
    MyVector(const MyVector& other) {
        m_data = static_cast<T*>(operator new(sizeof(T) * other.m_capacity));
        for (size_t i = 0; i < other.m_size; ++i)
            new (m_data + i) T(other.m_data[i]);
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }

    // Move constructor
    MyVector(MyVector&& other) noexcept
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = other.m_capacity = 0;
    }

    // Copy assignment
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            clear();
            operator delete(m_data);
            m_data = static_cast<T*>(operator new(sizeof(T) * other.m_capacity));
            for (size_t i = 0; i < other.m_size; ++i)
                new (m_data + i) T(other.m_data[i]);
            m_size = other.m_size;
            m_capacity = other.m_capacity;
        }
        return *this;
    }

    // Move assignment
    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            clear();
            operator delete(m_data);
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
        new (m_data + m_size) T(value);
        ++m_size;
    }

    void push_back(T&& value) {
        if (m_size == m_capacity)
            reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
        new (m_data + m_size) T(std::move(value));
        ++m_size;
    }

    void pop_back() {
        if (m_size == 0) throw std::out_of_range("Vector is empty");
        m_data[--m_size].~T();
    }

    void clear() {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i].~T();
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
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    std::cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << "\n";

    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << "\n";

    v.pop_back();
    std::cout << "After pop, size: " << v.size() << "\n";

    MyVector<int> v2 = v; // copy
    MyVector<int> v3 = std::move(v); // move

    std::cout << "v2[0]: " << v2[0] << "\n";
}

| Topic               | Key Point                                                          |
| ------------------- | ------------------------------------------------------------------ |
| Rule of 5           | Implements copy/move ctor and assignment                           |
| Memory management   | Manual allocation/deallocation (`operator new`, `operator delete`) |
| Placement new       | Used to construct objects in raw memory                            |
| Capacity management | Doubles on growth (`O(1)` amortized `push_back`)                   |
| Exception safety    | Ensures proper cleanup during reallocation                         |
| Element destruction | Uses `~T()` to manually destroy objects                            |
