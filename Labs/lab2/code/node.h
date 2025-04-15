#pragma once

#include <cassert>

/** Class Set::Node
 *
 * This class represents an internal node of a doubly linked list storing an int
 * All members of class Set::Node are public
 * but only class Set can access them, since Node is declared in the private part of class Set
 *
 */
class Set::Node {
public:
    /*
     * Constructor
     * \param nodeVal int to be stored in the Node
     * \param nextPtr a pointer to the next Node in the list
     * \param prevPtr a pointer to the previous Node in the list
     */
    explicit Node(int nodeVal = 0, Node* nextPtr = nullptr, Node* prevPtr = nullptr)
        : value{nodeVal}, next{nextPtr}, prev{prevPtr} {
        ++count_nodes;
    }

    /*
     * Destructor
     */
    ~Node() {
        --count_nodes;
        assert(count_nodes >= 0);  // number of existing nodes can never be negative
    }

    /*
     * Copy constructor -- disallowed to avoid shallow copying
     */
    Node(const Node& rhs) = delete;

    /*
     * Assignment operator -- disallowed to avoid shallow copying
     */
    Node& operator=(const Node& rhs) = delete;

    // Data members
    int value;   // int stored in the Node
    Node* next;  // Pointer to the next Node
    Node* prev;  // Pointer to the previous Node

    static int count_nodes;  // total number of existing nodes -- to help to detect bugs in the code
};
