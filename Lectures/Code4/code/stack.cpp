/***************************************************************************
 * TND004: Fö 4                                                             *
 * Implementation of a template class Stack                                 *
 * std::array-based implementation is used                                  *
 * **************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <cassert>

template <class T, int capacity = 100>
class Stack {
public:
    // Constructor
    Stack() : topOfStack{-1} {
    }

    // Copy constructor -- disabled!!
    Stack(const Stack&) = delete;

    // Assignment operator -- disabled!!
    const Stack& operator=(const Stack&) = delete;

    bool isFull() const {
        return (topOfStack == capacity - 1);
    }

    bool isEmpty() const {
        return (topOfStack == -1);
    }

    const T& top() const {
        assert(!isEmpty());
        return theArray[topOfStack];
    }

    void makeEmpty() {
        topOfStack = -1;
    }

    void push(const T& E) {
        assert(!isFull());

        theArray[++topOfStack] = E;
    }

    void pop() {
        assert(!isEmpty());

        topOfStack--;
    }

private:
    int topOfStack;
    // int capacity;  // maximum number of elements that can be stored in the stack

    std::array<T, capacity> theArray;
};

// Program that tests whether braces ')' and '(' and '[' and ']' are
// correctly used in an expression,
// e.g.[[(4+5)*(2+5)][(4*1)/(4-9)]]
// see sec. 3.6.3 of course book, page 123
int main() {
    std::cout << "Enter a mathematical expression (just braces): ";
    std::string exp;
    std::getline(std::cin, exp);
    std::cout << '\n';

    Stack<char, 500> S;
    bool warned = false;
    for (char c : exp) {
        if (c == '(' || c == '[') {
            S.push(c);
        }
        if (c == ')') {
            if (S.isEmpty() && !warned) {
                std::cout << exp << " is not well-formed!!\n";
                warned = true;
            } else if (S.top() == '(') {
                S.pop();
            } else if (S.top() == '[') {
                std::cout << exp << " is not well-formed!!\n";
                warned = true;
            } else {
                S.pop();
            }
        } else if (c == ']') {
            if (S.isEmpty() && !warned) {
                std::cout << exp << " is not well-formed!!\n";
                warned = true;
            } else if (S.top() == '[') {
                S.pop();
            } else if (S.top() == '(') {
                std::cout << exp << " is not well-formed!!\n";
                warned = true;
            } else {
                S.pop();
            }
        }
    }
    if (!S.isEmpty() && !warned) {
        std::cout << exp << " is not well-formed.\n";
    } else if (!warned) {
        std::cout << exp << " is well-formed!!\n";
    }
}
