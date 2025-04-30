#pragma once

#include <iostream>
#include <vector>
#include <cassert>

#define TEST_PRIORITY_QUEUE

/**
 * A heap based priority queue where the root is the smallest element -- min heap
 */
template <class Comparable>
class PriorityQueue {
public:
    /**
     * Constructor to create a queue with the given capacity
     */
    explicit PriorityQueue(int initCapacity = 100);

    /**
     * Constructor to initialize a priority queue based on a given vector V
     */
    explicit PriorityQueue(const std::vector<Comparable>& V);

    // Disable copying
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    /**
     * Make the queue empty
     */
    void makeEmpty();

    /**
     * Check is the queue is empty
     * Return true if the queue is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * Get the size of the queue, i.e. number of elements in the queue
     */
    size_t size() const;

    /**
     * Get the smallest element in the queue
     */
    Comparable findMin();

    /**
     * Remove and return the smallest element in the queue
     */
    Comparable deleteMin();

    /**
     * Add a new element x to the queue
     */
    void insert(const Comparable& x);

private:
    std::vector<Comparable> pq;

    // Auxiliary member functions

    /**
     * Restore the heap property
     */
    void heapify();

    /**
     * Test whether pq is a min heap
     */
    bool isMinHeap() const;

    // PercolateDown
    void percolateDown(std::vector<Comparable>& pq, int i);

    // PercolateUp
    void percolateUp(std::vector<Comparable>& pq, int i);
};

/* *********************** Member functions implementation *********************** */

/**
 * Constructor to create a queue with the given capacity
 */
template <class Comparable>
PriorityQueue<Comparable>::PriorityQueue(int initCapacity) {
    /*
     * ADD CODE HERE
     */
    pq.reserve(initCapacity + 1);
    pq.push_back(Comparable{});
    assert(isEmpty());  // do not remove this line
}

/**
 * Constructor to initialize a priority queue based on a given vector V
 */
template <class Comparable>
PriorityQueue<Comparable>::PriorityQueue(const std::vector<Comparable>& V) : pq{V} {
    // Implementation is provided for you
    heapify();
#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif
}

/**
 * Make the queue empty
 */
template <class Comparable>
void PriorityQueue<Comparable>::makeEmpty() {
    /*
     * ADD CODE HERE
     */
    pq.clear();
    pq.push_back(Comparable{});
}

/**
 * Check is the queue is empty
 * Return true if the queue is empty, false otherwise
 */
template <class Comparable>
bool PriorityQueue<Comparable>::isEmpty() const {
    /*
     * ADD CODE HERE
     */
    return pq.size() <= 1;  // replace this line by the correct return value
}

/**
 * Get the size of the queue, i.e. number of elements in the queue
 */
template <class Comparable>
size_t PriorityQueue<Comparable>::size() const {
    /*
     * ADD CODE HERE
     */
    return pq.size() - 1;  // replace this line by the correct return value
}

/**
 * Get the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::findMin() {
    assert(isEmpty() == false);  // do not remove this line
    /*
     * ADD CODE HERE
     */
    return pq[1];  // replace this line by the correct return value
}

/**
 * Remove and return the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::deleteMin() {
    assert(!isEmpty());  // Ensure the queue is not empty

    Comparable minElement = pq[1];

    pq[1] = pq[pq.size() - 1];
    pq.pop_back();


    // Ensure the heap property is maintained
#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif

    return minElement;  // Return the smallest element
}


/**
 * Add a new element x to the queue
 */
template <class Comparable>
void PriorityQueue<Comparable>::insert(const Comparable& x) {
    /*
     * ADD CODE HERE
     */

    pq.push_back(x);
    percolateUp(pq, pq.size() - 1);
    // Do not remove this code block
#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif
}

/* ******************* Private member functions ********************* */

/**
 * Restore the heap property
 */
template <class Comparable>
void PriorityQueue<Comparable>::heapify() {
    assert(pq.size() > 1);  // do not remove this line
    /*
     * ADD CODE HERE
     */
    int n = pq.size() - 1;

    for (int i = n / 2; i >= 1; --i) {
        percolateDown(pq, i);
    }
}

/**
 * Test whether pq is a min heap
 */
template <class Comparable>
bool PriorityQueue<Comparable>::isMinHeap() const {
    /*
     * ADD CODE HERE
    */
    for (size_t i = 1; i < this->size(); i++) {

        if (this->size() == 1) return true;

        bool isLeafL = (2 * i) > this->size();
        if (isLeafL) break;  // stops when first leaf is hit
        auto leftChild = 2 * i;
        if (pq[i] > pq[leftChild]) return false;

        bool isLeafR = 2 * i + 1 > this->size();
        if (isLeafR) break;
        auto rightChild = leftChild + 1;
        if (pq[i] > pq[rightChild]) return false;
    }
    return true;  // delete this line
}

template <class Comparable>
void PriorityQueue<Comparable>::percolateDown(std::vector<Comparable>& pq, int i) {
    Comparable temp = pq[i];
    auto c = 2 * i;  // left child

    while (c < pq.size()) {
        if (c < pq.size() - 1) {
            if (pq[c + 1] < pq[c])  // largest child?
                c++;
        }
        // percolate down
        if (pq[c] < temp) {
            pq[i] = pq[c];
            i = c;
            c = 2 * i;
        } else {
            break;
        }
    }
    pq[i] = temp;
}

template <class Comparable>
void PriorityQueue<Comparable>::percolateUp(std::vector<Comparable>& pq, int i) {
    int child = pq.size() - 1;
    int parent = child / 2;

    while (parent > 0) {
        if (pq[child] < pq[parent]) {
            std::swap(pq[child], pq[parent]);
            child = parent;
            parent = child / 2;
        } else
            break;
    }
}