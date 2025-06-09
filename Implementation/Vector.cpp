#include <iostream>
#include <stdexcept>
#include <utility> // for std::move

template<typename T>
class Vector {
private:
    T* data;
    size_t _size;
    size_t _capacity;

    void resize() {
        _capacity = (_capacity == 0) ? 1 : _capacity * 2;
        T* new_data = new T[_capacity];

        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = std::move(data[i]); // Move for performance
        }

        delete[] data;
        data = new_data;
    }

public:
    // Constructors
    Vector() : data(nullptr), _size(0), _capacity(0) {}

    // Destructor
    ~Vector() {
        delete[] data;
    }

    // Copy constructor
    Vector(const Vector& other) : _size(other._size), _capacity(other._capacity) {
        data = new T[_capacity];
        for (size_t i = 0; i < _size; ++i)
            data[i] = other.data[i];
    }

    // Move constructor
    Vector(Vector&& other) noexcept : data(other.data), _size(other._size), _capacity(other._capacity) {
        other.data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    // Copy assignment
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            _size = other._size;
            _capacity = other._capacity;
            data = new T[_capacity];
            for (size_t i = 0; i < _size; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }

    // Move assignment
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            _size = other._size;
            _capacity = other._capacity;
            other.data = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
        return *this;
    }

    // Add element
    void push_back(const T& val) {
        if (_size == _capacity)
            resize();
        data[_size++] = val;
    }

    void push_back(T&& val) {
        if (_size == _capacity)
            resize();
        data[_size++] = std::move(val);
    }

    // Element access
    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    T& at(size_t index) {
        if (index >= _size)
            throw std::out_of_range("Index out of range in Vector::at()");
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= _size)
            throw std::out_of_range("Index out of range in Vector::at()");
        return data[index];
    }

    // Size and capacity
    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }

    // Clear
    void clear() {
        delete[] data;
        data = nullptr;
        _size = 0;
        _capacity = 0;
    }
};


int main() {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";  // Output: 10 20 30

    std::cout << "\nElement at 1: " << v.at(1) << std::endl;

    // Test move semantics
    Vector<int> v2 = std::move(v);
    std::cout << "v2[0]: " << v2[0] << std::endl;
}

