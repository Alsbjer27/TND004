#pragma once

#include <iostream>
#include <vector>
#include <compare>  // three-way comparison operator <=>

/** Class to represent a Set of ints
 *
 * Set is implemented as a sorted doubly linked list
 * Sets should not contain repetitions, i.e.
 * two ints with the same value cannot belong to a Set
 *
 * All Set operations must have a linear time complexity, in the worst case
 */
class Set {

public:
    /*
     *  Default constructor :create an empty Set
     */
    Set();

    /*
     *  Conversion constructor: convert val into a singleton {val}
     */
    Set(int val);

    /*
     * Constructor to create a Set from a sorted vector of unique ints
     * \param list_of_values is an increasingly sorted vector of unique ints
     */
    explicit Set(const std::vector<int>& list_of_values);

    /*
     * Copy constructor: create a new Set as a copy of Set S
     * \param S Set to be copied
     * Function does not modify Set S in any way
     */
    Set(const Set& S);

    /*
     * Transform the Set into an empty set
     * Remove all nodes from the list, except the dummy nodes
     */
    void make_empty();

    /*
     * Destructor: deallocate all memory (Nodes) allocated for the list
     */
    ~Set();

    /*
     * Assignment operator: assign new contents to the *this Set, replacing its current content
     * \param S Set to be copied into Set *this
     * Use copy-and swap idiom -- TNG033: lecture 5
     */
    Set& operator=(Set S);

    /*
     * Test whether val belongs to the Set
     * Return true if val belongs to the set, otherwise false
     * This function does not modify the Set in any way
     */
    bool is_member(int val) const;

    /*
     * Test whether the Set is empty
     * Return true if the set is empty, otherwise false
     * This function does not modify the Set in any way
     */
    bool is_empty() const {
        return (counter == 0);
    }

    /*
     * Count the number of values stored in the Set
     * Return number of elements in the set
     * This function does not modify the Set in any way
     */
    size_t cardinality() const {
        return counter;
    }


    /*
     * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
     * Return std::partial_ordering::equivalent, if *this == S
     * Return std::partial_ordering::less, if *this < S (*this is contained in Set S)
     * Return std::partial_ordering::greater, if *this > S (*this constains Set S)
     * Return std::partial_ordering::unordered, otherwise (Sets *this and S are not comparable)
     * 
     * Requirement: S1<=>S2 should iterate through each set S1 and S2 no more than once
     */
    std::partial_ordering operator<=>(const Set& S) const;

    /*
     * Test whether Set *this and S represent the same set
     * Return true, if *this has same elements as set S
     * Return false, otherwise
     * 
     * Requirement: S1 == S2 should iterate through each set S1 and S2 no more than once
     */
    bool operator==(const Set& S) const;

    /*
     * Modify Set *this such that it becomes the union of *this with Set S
     * Set *this is modified and then returned
     */
    Set& operator+=(const Set& S);

    /*
     * Modify Set *this such that it becomes the intersection of *this with Set S
     * Set *this is modified and then returned
     */
    Set& operator*=(const Set& S);

    /*
     * Modify Set *this such that it becomes the Set difference between Set *this and Set S
     * Set *this is modified and then returned
     */
    Set& operator-=(const Set& S);

    /*
     * Return number of existing nodes
     * Used solely for debug purposes
     */
    static int get_count_nodes();

    /* ******************************************* *
     * Overloaded operators: non-member functions  *
     * ******************************************* */

    /*
     * Overloaded operator<<
     * \param os ostream object where the set S elements are written
     */
    friend std::ostream& operator<<(std::ostream& os, const Set& S) {
        S.write_to_stream(os);
        return os;
    }

    /*
     * Overloaded operator+: Set union S1+S2
     * S1+S2 is the Set of elements in Set S1 or in Set S2 (without repeated elements)
     * Return a new Set representing the union of S1 with S2, S1+S2
     */
    friend Set operator+(Set S1, const Set& S2) {
        return (S1 += S2);
    }

    /*
     * Overloaded operator*: Set intersection S1*S2
     * S1*S2 is the Set of elements in both sets S1 and S2
     * Return a new Set representing the intersection of S1 with S2, S1*S2
     */
    friend Set operator*(Set S1, const Set& S2) {
        return (S1 *= S2);
    }

    /*
     * Overloaded operator-: Set difference S1-S2
     * S1-S2 is the Set of elements in Set S1 that do not belong to Set S2
     * Return a new Set representing the set difference S1-S2
     */
    friend Set operator-(Set S1, const Set& S2) {
        return (S1 -= S2);
    }

private:
    class Node;  // nested class defined in node.h

    Node* head;      // pointer to the dummy header Node
    Node* tail;      // pointer to the dummy tail Node
    size_t counter;  // number of values in the Set

    /* ************************** *
     * Private Member Functions    *
     * **************************  */

    /*
     * Insert a new Node storing val after the Node pointed by p
     * \param p pointer to a Node
     * \param val value to be inserted  after position p
     */
    void insert_node(Node* p, int val);

    /*
     * Remove the Node pointed by p
     * \param p pointer to a Node
     */
    void remove_node(Node* p);

    /*
     * Write Set *this to stream os
     */
    void write_to_stream(std::ostream& os) const;
};