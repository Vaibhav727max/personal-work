#include <iostream>

class Complex {
    double real, imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Function overloading
    void set(double r) { real = r; imag = 0; }
    void set(double r, double i) { real = r; imag = i; }

    // Operator overloading
    Complex operator+(const Complex& other) const {
        return {real + other.real, imag + other.imag};
    }

    Complex operator*(const Complex& other) const {
        return {real * other.real - imag * other.imag,
                real * other.imag + imag * other.real};
    }

    friend std::ostream& operator<<(std::ostream& out, const Complex& c) {
        return out << c.real << (c.imag < 0 ? " - " : " + ")
                   << (c.imag < 0 ? -c.imag : c.imag) << 'i';
    }
};

int main() {
    Complex a(2, 3), b(4, -1);
    Complex sum = a + b;
    Complex product = a * b;
    Complex c;
    c.set(5);
    c.set(5, 2);

    std::cout << "a + b = " << sum << "\na * b = " << product
              << "\nOverloaded set result = " << c << '\n';
}
