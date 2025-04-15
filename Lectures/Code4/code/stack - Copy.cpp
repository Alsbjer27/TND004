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

// Program that tests whether braces ')' and '(' are
// correctly used in an expression, e.g. (3+4)*(a+b)
// Note: square brackets, "[" and "]", not considered
int main()  // see sec. 3.6.3 of course book, page 123
{
    Stack<char, 500> S;

    std::string exp;
    bool warned = false;

    std::cout << "Enter a mathematical expression: ";
    std::getline(std::cin, exp);

    std::cout << '\n';

    for (char c : exp) {
        if (c == '(') {
            S.push(c);
        }

        if (c == ')') {
            if (S.isEmpty() && !warned) {
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
