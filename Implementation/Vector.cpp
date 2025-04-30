#include <iostream>
#include <stdexcept>  // For std::out_of_range

template<typename T>
class MyVector {
private:
    T* data;             // Pointer to the dynamically allocated array
    size_t capacity;     // Capacity of the vector (how much space it can hold)
    size_t size;         // Current size (number of elements in the vector)

    // Function to resize the vector (doubling the capacity)
    void resize() {
        capacity *= 2;
        T* new_data = new T[capacity];  // Create a new array with double capacity

        // Copy elements from the old array to the new array
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        // Delete the old array
        delete[] data;

        // Point the data to the new array
        data = new_data;
    }

public:
    // Constructor
    MyVector() : data(nullptr), capacity(0), size(0) {}

    // Destructor to clean up allocated memory
    ~MyVector() {
        delete[] data;
    }

    // Function to add an element to the back of the vector
    void push_back(const T& value) {
        if (size == capacity) {
            // Resize the array if there is no space left
            if (capacity == 0) {
                capacity = 1;  // Start with capacity 1 if the vector is empty
            }
            resize();
        }

        // Add the new element
        data[size] = value;
        ++size;
    }

    // Function to remove the last element
    void pop_back() {
        if (size > 0) {
            --size;
        } else {
            throw std::out_of_range("pop_back called on an empty vector.");
        }
    }

    // Function to access an element by index
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range.");
        }
        return data[index];
    }

    // Const version of operator[] for read-only access
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range.");
        }
        return data[index];
    }

    // Function to get the current size of the vector
    size_t get_size() const {
        return size;
    }

    // Function to check if the vector is empty
    bool empty() const {
        return size == 0;
    }

    // Function to get the capacity of the vector
    size_t get_capacity() const {
        return capacity;
    }

    // Function to clear the vector
    void clear() {
        size = 0;
    }

    // Function to print the vector elements (for debugging purposes)
    void print() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    MyVector<int> vec;

    // Add elements to the vector
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);
    
    std::cout << "Vector contents: ";
    vec.print();  // Output: 10 20 30 40

    // Access elements by index
    std::cout << "Element at index 2: " << vec[2] << std::endl;  // Output: 30

    // Remove the last element
    vec.pop_back();
    std::cout << "After pop_back, vector contents: ";
    vec.print();  // Output: 10 20 30

    // Get size and capacity
    std::cout << "Size: " << vec.get_size() << std::endl;  // Output: 3
    std::cout << "Capacity: " << vec.get_capacity() << std::endl;  // Output: 4

    // Clear the vector
    vec.clear();
    std::cout << "After clear, vector is empty: " << (vec.empty() ? "Yes" : "No") << std::endl;  // Output: Yes

    return 0;
}
