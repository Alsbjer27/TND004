/* ***********************************
 * TND004: FÖ3                       *
 * Power function: x^n               *
 * O(n) algorithm                    *
 * O(log n) algorithm                *
 *  -- Recursive version             *
 *  -- Iterative version             *
 *************************************/

#include <iostream>
#include <limits>

// Return x^n, with n >= 0
// Iterative: x^n = x * x * ... * x
long long pow0(long long x, int n);

// Return x^n, with n >= 0
// Iterative: performs O(log n) multiplications
long long pow1(long long x, int n);

// Return x^n, with n >= 0
// Recursive: performs O(log n) multiplications
long long pow2(long long x, int n);

int main() {
    /*std::cout << "Max long long in this machine = " << std::numeric_limits<long long int>::max()
              << "\n";*/

    std::cout << "Enter n: ";
    int n;
    std::cin >> n;

    std::cout << "Enter x: ";
    long x;
    std::cin >> x;

    std::cout << "x^n = x * x * ... * x\n";
    std::cout << x << "^" << n << "= " << pow0(x, n) << "\n\n";

    std::cout << "Iterative: O(log n) algorithm\n";
    std::cout << x << "^" << n << "= " << pow1(x, n) << "\n\n";

    std::cout << "Recursive: O(log n) algorithm\n";
    std::cout << x << "^" << n << "= " << pow2(x, n) << "\n\n";
}

// Return x^n
// Iterative: x^n = x * x * ... * x
long long pow0(long long x, int n) {
    if (n == 0) {
        return 1;
    }

    long long res = x;
    for (; n > 1; --n) {
        res *= x;
    }
    return res;
}

// Return x^n, with n >= 0
// Iterative: performs O(log n) multiplications
long long pow1(long long x, int n) {
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return x;
    }

    long long res = 1;
    while (n > 1) {
        if (n % 2)
            res *= x;
        x *= x;
        n /= 2;
    }
    return x * res;
}

// Return x^n, with n >= 0
// Recursive: performs O(log n) multiplications
long long pow2(long long x, int n) {
    if (n == 0) {  // base case
        return 1;
    }
    if (n == 1) {  // base case
        return x;
    }
    if (n % 2 == 0) {
        return pow2(x * x, n / 2);  // tail recursion
    } else {
        return pow2(x * x, n / 2) * x;
    }
}
