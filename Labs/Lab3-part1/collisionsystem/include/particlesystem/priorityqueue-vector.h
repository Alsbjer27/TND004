#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

/**
 * A priority queue implemented as a decreasingly sorted vector
 * the smallest element is at the end of the vector
 */
template <class Comparable>
class PriorityQueue {
public:
    /**
     * Constructor to create a queue with the given capacity
     */
    explicit PriorityQueue(int initCapacity = 100) {
        pq.reserve(initCapacity);
        makeEmpty();
        assert(isEmpty());
    }

    /**
     * Constructor to initialize a priority queue based on a given vector V
     */
    explicit PriorityQueue(const std::vector<Comparable>& V) : pq{V} { heapify(); }

    // Disable copying
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    /**
     * Make the queue empty
     */
    void makeEmpty() { pq.clear(); }

    /**
     * Check is the queue is empty
     * Return true if the queue is empty, false otherwise
     */
    bool isEmpty() const { return std::ssize(pq) == 0; }

    /**
     * Get the size of the queue, i.e. number of elements in the queue
     */
    size_t size() const { return std::ssize(pq); }

    /**
     * Get the smallest element in the queue
     */
    Comparable findMin() {
        assert(isEmpty() == false);
        return pq.back();
    }

    /**
     * Remove and return the smallest element in the queue
     */
    Comparable deleteMin() {
        assert(!isEmpty());
        Comparable x = pq.back();
        pq.pop_back();
        return x;
    }

    /**
     * Add a new element x to the queue
     */
    void insert(const Comparable& x) {
        pq.push_back(x);
        heapify();
    }

private:
    std::vector<Comparable> pq;

    // Auxiliary member functions

    /**
     * Restore the heap property
     */
    void heapify() {
        // sort decreasingly
        std::sort(pq.begin(), pq.end(), std::greater<Comparable>());
    }

    /**
     * Test whether pq is a min heap
     */
    bool isMinHeap() const {
        return std::is_sorted(pq.begin(), pq.end(), std::greater<Comparable>());
    }
};
