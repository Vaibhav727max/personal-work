#include <algorithm>
#include <cstring>
#include <iostream>
#include <utility>

class TextBuffer {
private:
    char* data;
    std::size_t size;

public:
    // Default constructor
    TextBuffer() : data(new char[1]{'\0'}), size(0) {}

    // Parameterized constructor
    explicit TextBuffer(const char* text)
        : data(new char[std::strlen(text) + 1]), size(std::strlen(text)) {
        std::memcpy(data, text, size + 1);
    }

    // Copy constructor (deep copy)
    TextBuffer(const TextBuffer& other)
        : data(new char[other.size + 1]), size(other.size) {
        std::memcpy(data, other.data, size + 1);
    }

    // Move constructor
    TextBuffer(TextBuffer&& other) noexcept
        : data(std::exchange(other.data, nullptr)),
          size(std::exchange(other.size, 0)) {}

    // Copy assignment
    TextBuffer& operator=(const TextBuffer& other) {
        if (this != &other) {
            char* replacement = new char[other.size + 1];
            std::memcpy(replacement, other.data, other.size + 1);
            delete[] data;
            data = replacement;
            size = other.size;
        }
        return *this;
    }

    // Move assignment
    TextBuffer& operator=(TextBuffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = std::exchange(other.data, nullptr);
            size = std::exchange(other.size, 0);
        }
        return *this;
    }

    // Destructor
    ~TextBuffer() { delete[] data; }

    void print() const {
        std::cout << (data ? data : "(moved-from)") << '\n';
    }
};

int main() {
    TextBuffer empty;
    TextBuffer first("C++ Rule of Five");
    TextBuffer copied(first);
    TextBuffer moved(std::move(first));
    empty = copied;
    TextBuffer assigned;
    assigned = std::move(copied);

    empty.print();
    moved.print();
    assigned.print();
}
