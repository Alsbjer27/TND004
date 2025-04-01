#include <iostream>
#include <cassert>

long long fibonacci(int n);

int main() {
    int n;
    std::cout << "Enter n: ";  // try n = 50, for iterative and recursive functions

    while (std::cin >> n && n >= 0) {
        std::cout << "\nfibonacci(" << n << ") = " << fibonacci(n) << "\n\n";

        std::cout << "Enter n: ";
    }
}

long long fibonacci(int n) {  // recursive function
    assert(n >= 0);           // fibonacci is not defined for n < 0

    if (n == 0 || n == 1) {  // base cases
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  // recursive function calls
}

/*
long long fibonacci(int n) {  // iterative function
    assert(n >= 0);           // fibonacci is not defined for n < 0

    if (n == 0 || n == 1) {
        return n;
    } else {
        long long before_last = 0;
        long long last = 1;
        long long current = 0;

        for (int i = 2; i <= n; i++) {
            current = before_last + last;
            before_last = last;
            last = current;
        }
        return current;
    }
}
*/
