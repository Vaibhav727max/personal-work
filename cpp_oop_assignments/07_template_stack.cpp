#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
class Stack {
private:
    std::vector<T> items;

public:
    void push(const T& value) { items.push_back(value); }

    void pop() {
        if (empty()) throw std::out_of_range("Cannot pop an empty stack");
        items.pop_back();
    }

    T& top() {
        if (empty()) throw std::out_of_range("Stack is empty");
        return items.back();
    }

    bool empty() const { return items.empty(); }
    std::size_t size() const { return items.size(); }
};

template <typename T>
void testStack(const T& first, const T& second) {
    Stack<T> stack;
    stack.push(first);
    stack.push(second);
    std::cout << "Top: " << stack.top()
              << " | Size: " << stack.size() << '\n';
    stack.pop();
    std::cout << "After pop, top: " << stack.top() << "\n\n";
}

int main() {
    testStack<int>(10, 20);
    testStack<float>(1.5f, 2.5f);
    testStack<std::string>("hello", "C++ templates");
}
