#include <iostream>
#include <limits>  // std::numeric_limits
#include <cassert>

// Return n!
// Recursive
long long factorial(int n);

// Return n!
// Iterative
long long factorial2(int n);

int main() {
    // std::cout << "Maximum value for long long: " << std::numeric_limits<long long>::max() <<
    // '\n';

    int n;
    std::cout << "Enter n: ";
    std::cin >> n;

    std::cout << "Recursive\n";
    std::cout << "fact(" << n << ")= " << factorial(n) << "\n";

    std::cout << "\nIterative\n";
    std::cout << "fact(" << n << ")= " << factorial2(n) << "\n";
}

// Return n!
// Recursive
long long factorial(int n) {
    assert(n >= 0);  // factorial is not defined for negative ints

    if (n == 0) {  // base case
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

// Return n!
// Iterative
long long factorial2(int n) {
    assert(n >= 0);  // factorial is not defined for negative ints

    long long res = 1;

    for (int i = 1; i <= n; ++i) {
        res *= i;
    }
    return res;
}
