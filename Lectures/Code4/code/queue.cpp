/***************************************************************************
 * TND004: Fö 4                                                             *
 * Implementation of a template class Queue                                 *
 * std::array-based implementation is used                                  *
 * **************************************************************************/

#include <iostream>
#include <iomanip>
#include <array>
#include <cassert>

template <class T, int capacity = 100>
class Queue {
public:
    // Constructor
    Queue() : counter{0}, back{capacity - 1}, front{0} {
    }

    // Copy constructor -- disabled!!
    Queue(const Queue&) = delete;

    // Assignment operator -- disabled!!
    const Queue& operator=(const Queue&) = delete;

    bool isFull() const {
        return (counter == capacity);
    }

    bool isEmpty() const {
        return (counter == 0);
    }

    const T& getFront() const {
        assert(!isEmpty());

        return theArray[front];
    }

    int size() const {
        return counter;
    }

    void makeEmpty() {
        counter = 0;
        front = 0;
        back = capacity - 1;
    }

    void enqueue(const T& E) {
        assert(!isFull());

        back = (++back) % capacity;
        theArray[back] = E;
        ++counter;
    }

    const T& dequeue() {
        assert(!isEmpty());

        const T& temp{theArray[front]};
        front = (++front) % capacity;
        --counter;

        return temp;
    }

private:
    int counter;  // how many elements are stored in the queue
    int front;
    int back;

    std::array<T, capacity> theArray;
};

template <int n>
int mystery(Queue<int, n>& Q) {
    int sum = 0;
    Queue<int, n> T;
    while (!Q.isEmpty()) {
        int x = Q.getFront();
        sum += x;
        Q.dequeue();
        T.enqueue(x);
    }

    while (!T.isEmpty()) {
        Q.enqueue(T.getFront());
        T.dequeue();
    }
    return sum;
}

int main() {  // simple test program
    Queue<int, 4> Q;
    Q.enqueue(5);
    Q.enqueue(2);
    Q.enqueue(-1);
    Q.enqueue(3);

    int sum = mystery(Q);
    std::cout << "sum = " << sum << "\n";

    while (!Q.isEmpty()) {
        std::cout << Q.dequeue() << " ";
    }
    std::cout << "\n";
}
