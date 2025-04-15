#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

/*
 *  Default constructor :create an empty Set
 */
int Set::get_count_nodes() {
    return Set::Node::count_nodes;
}

/*
 *  Default constructor :create an empty Set
 */
Set::Set() : counter{0} {
    // IMPLEMENT before Lab2 HA
    head = new Node{};
    tail = new Node{};

    head->next = tail;
    tail->prev = head;
}

/*
 *  Conversion constructor: convert val into a singleton {val}
 */
Set::Set(int val) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    insert_node(head, val);
}

/*
 * Constructor to create a Set from a sorted vector of unique ints
 * \param list_of_values is an increasingly sorted vector of unique ints
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    Node* ptr = head;
    for (int val : list_of_values) {
        insert_node(ptr, val);
        ptr = ptr->next;
    }
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    Node* source = S.head->next;
    Node* destination = head;
    while (source != S.tail) {
        insert_node(destination, source->value);
        destination = destination->next;
        source = source->next;
    }
}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
    // IMPLEMENT before Lab2 HA
    Node* current = head->next;
    while (current != tail) {
        Node* next = current->next;
        remove_node(current);
        current = next;
    }
}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
    // IMPLEMENT before Lab2 HA
    make_empty();
    delete head;
    delete tail;
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Use copy-and swap idiom -- TNG033: lecture 5
 */
Set& Set::operator=(Set S) {
    // IMPLEMENT before Lab2 HA
    std::swap(head, S.head);
    std::swap(tail, S.tail);
    std::swap(counter, S.counter);
    return *this;
}

/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
    // IMPLEMENT before Lab2 HA
    Node* current = head->next;
    while (current != tail && current->value < val) {
        current = current->next;
    }
    return (current != tail && current->value == val);
}

/*
 * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
 * Return std::partial_ordering::equivalent, if *this == S
 * Return std::partial_ordering::less, if *this < S
 * Return std::partial_ordering::greater, if *this > S
 * Return std::partial_ordering::unordered, otherwise
 * 
 * Requirement: must iterate through each set no more than once
 */
std::partial_ordering Set::operator<=>(const Set& S) const {
    Node* a = head->next;
    Node* b = S.head->next;

    bool a_missing = false;
    bool b_missing = false;

    while (a != tail && b != S.tail) {
        if (a->value == b->value) {
            a = a->next;
            b = b->next;
        }
        else if (a->value < b->value) {
            b_missing = true;  // *this has a value S doesn't
            a = a->next;
        }
        else { // a->value > b->value
            a_missing = true;  // S has a value *this doesn't
            b = b->next;
        }
    }
    while (a != tail) {
        b_missing = true;  // *this has extra elements
        a = a->next;
    }

    while (b != S.tail) {
        a_missing = true;  // S has extra elements
        b = b->next;
    }
    if (!a_missing && !b_missing) return std::partial_ordering::equivalent;
    if (!a_missing && b_missing)  return std::partial_ordering::greater;
    if (a_missing && !b_missing)  return std::partial_ordering::less;
    return std::partial_ordering::unordered;
}


/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 * 
 * Requirement: must iterate through each set no more than once
 */
bool Set::operator==(const Set& S) const {
    // IMPLEMENT before Lab2 HA
    Node* a = head->next;
    Node* b = S.head->next;

    while (a != tail && b != S.tail) {
        if (a->value != b->value)
            return false;
        a = a->next;
        b = b->next;
    }

    return (a == tail && b == S.tail);
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
    Set result;
    Node* currentA = head->next;
    Node* currentB = S.head->next;
    Node* result_end = result.head; 

    while (currentA != tail && currentB != S.tail) {
        if (currentA->value < currentB->value) {
            result.insert_node(result_end, currentA->value);
            currentA = currentA->next;
        }
        else if (currentA->value > currentB->value) {
            result.insert_node(result_end, currentB->value);
            currentB = currentB->next;
        }
        else {
            result.insert_node(result_end, currentA->value);  // same value, insert once
            currentA = currentA->next;
            currentB = currentB->next;
        }
        result_end = result_end->next;
    }

    while (currentA != tail) {
        result.insert_node(result_end, currentA->value);
        currentA = currentA->next;
        result_end = result_end->next;
    }

    while (currentB != S.tail) {
        result.insert_node(result_end, currentB->value);
        currentB = currentB->next;
        result_end = result_end->next;
    }

    *this = result;
    return *this;
}


/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
    // IMPLEMENT
    return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
    // IMPLEMENT
    return *this;
}


/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

/*
 * Insert a new Node storing val after the Node pointed by p
 * \param p pointer to a Node
 * \param val value to be inserted  after position p
 */
void Set::insert_node(Node* p, int val) {
    // IMPLEMENT before Lab2 HA
    Node* newNode = new Node{ val, p->next, p };
    p->next->prev = newNode;
    p->next = newNode;
    // p <-> newNode <-> oldNext
    ++counter;
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
    // IMPLEMENT before Lab2 HA
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    --counter;
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
    if (is_empty()) {
        os << "Set is empty!";
    } else {
        Set::Node* ptr{head->next};

        os << "{ ";
        while (ptr != tail) {
            os << ptr->value << " ";
            ptr = ptr->next;
        }
        os << "}";
    }
}
