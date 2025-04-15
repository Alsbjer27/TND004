#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include "set.h"

int main() {
    /*****************************************************
     * TEST PHASE 0                                       *
     * Default constructor, conversion constructor,       *
     * make_empty, destructor, and operator<<             *
     ******************************************************/
    std::cout << "TEST PHASE 0: default and conversion constructor\n";

    {
        Set S1{};
        assert(Set::get_count_nodes() == 2);

        Set S2{-4};
        assert(Set::get_count_nodes() == 5);

        Set S3{999};
        assert(Set::get_count_nodes() == 8);

        S3.make_empty();
        assert(Set::get_count_nodes() == 7);

        // Test
        std::ostringstream os{};
        os << S1 << ' ' << S2 << ' ' << S3;

        std::string tmp{os.str()};
        assert((tmp == std::string{"Set is empty! { -4 } Set is empty!"}));
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 1                                       *
     * Constructor: create a Set from a sorted vector     *
     ******************************************************/
    std::cout << "\nTEST PHASE 1: constructor from a vector\n";

    {
        std::vector<int> A1{1, 3, 5};
        std::vector<int> A2{2, 3, 4};

        Set S1{A1};
        assert(Set::get_count_nodes() == 5);

        Set S2{A2};
        assert(Set::get_count_nodes() == 10);

        // Test
        std::ostringstream os{};
        os << S1 << " " << S2;

        std::string tmp{os.str()};
        assert((tmp == std::string{"{ 1 3 5 } { 2 3 4 }"}));
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 2                                       *
     * Copy constructor                                   *
     ******************************************************/
    std::cout << "\nTEST PHASE 2: copy constructor\n";

    {
        std::vector<int> A1{1, 3, 5};

        Set S1{A1};
        Set S2{S1};

        assert(Set::get_count_nodes() == 10);

        // Test
        std::ostringstream os{};
        os << S1 << " " << S2;

        std::string tmp{os.str()};
        assert((tmp == std::string{"{ 1 3 5 } { 1 3 5 }"}));
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 3                                       *
     * Assignment operator: operator=                     *
     ******************************************************/
    std::cout << "\nTEST PHASE 3: operator=\n";

    {
        Set S1{};

        std::vector<int> A1{1, 3, 5};
        Set S2{A1};

        std::vector<int> A2{2, 3, 4};
        Set S3{A2};

        assert(Set::get_count_nodes() == 12);

        S1 = S2 = S3;

        assert(Set::get_count_nodes() == 15);

        // Test
        std::ostringstream os{};
        os << S1 << " " << S2 << " " << S3;

        std::string tmp{os.str()};
        assert((tmp == std::string{"{ 2 3 4 } { 2 3 4 } { 2 3 4 }"}));
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 4                                       *
     * is_member                                          *
     ******************************************************/
    std::cout << "\nTEST PHASE 4: is_member\n";

    {
        std::vector<int> A1{1, 3, 5};
        Set S1{A1};

        // Test
        assert(S1.is_member(1));
        assert(S1.is_member(2) == false);
        assert(S1.is_member(3));
        assert(S1.is_member(5));
        assert(S1.is_member(99999) == false);
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 5                                       *
     * cardinality, is_empty                              *
     ******************************************************/
    std::cout << "\nTEST PHASE 5: cardinality and is_empty\n";

    {
        std::vector<int> A1{1, 3, 5};
        Set S1{A1};

        // Test
        assert(S1.cardinality() == 3);
        assert(Set::get_count_nodes() == 5);

        S1.make_empty();
        assert(S1.is_empty());
        assert(Set::get_count_nodes() == 2);
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 6                                       *
     * Overloaded operators: operator== and operator<=>   *
     ******************************************************/
    std::cout << "\nTEST PHASE 6: equality and <=>\n";

    {
        std::vector<int> A1{1, 3, 5, 8};
        std::vector<int> A2{3, 5};

        Set S1{A1};
        Set S2{A2};

        // Test
        assert(S2 <= S1);
        assert((S1 <= S2) == false);
        assert((S1 < S1) == false);
        assert((S1 > S1) == false);
        assert(S1 <= S1);
        assert((S1 == S2) == false);
        assert(S1 != S2);

        std::vector<int> A3{3, 5, 8};
        // Test
        assert((Set{A3} <= S2) == false);
        assert(3 < Set{A3});

        std::vector<int> A4{10};  // singleton
        // Test
        assert(Set{A4} == 10);
        assert(10 == Set{A4});

        std::vector<int> A5{1, 2};
        Set S3{A5};
        // Test
        assert((S3 <= S2) == false);
        assert((S2 >= S3) == false);
        assert((S2 == S3) == false);
        assert((S3 > S2) == false);
        assert((S3 < S2) == false);
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 7                                       *
     * Overloaded operators: operator+=, operator*=       *
     *                   and operator-=                   *
     ******************************************************/
    std::cout << "\nTEST PHASE 7: operator+=, operator*=, operator-=\n";

    {
        std::vector<int> A1{1, 3, 5, 8};
        std::vector<int> A2{2, 3, 7};

        Set S1{A1};
        Set S2{A2};

        S1 += S2;
        assert(Set::get_count_nodes() == 13);

        S2 *= S2;
        assert(Set::get_count_nodes() == 13);

        // Test
        std::vector<int> A3{1, 2, 3, 5, 7, 8};
        assert(S1 == Set{A3});
        assert(S2 == S2);

        S1 -= S1;
        assert(S1.is_empty());

        assert(Set::get_count_nodes() == 7);
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 8                                       *
     * Overloaded operators: union, intersection, and     *
     * and difference                                     *
     ******************************************************/
    std::cout << "\nTEST PHASE 8: union, intersection, and difference\n";

    {
        std::vector<int> A1{1, 3, 5, 8};
        std::vector<int> A2{2, 3, 7};

        Set S1{A1};
        Set S2{A2};
        Set S3{};

        S3 = S1 + S2;
        assert(Set::get_count_nodes() == 19);

        // test
        std::vector<int> A3{1, 2, 3, 5, 7, 8};
        assert(S3 == Set{A3});

        S3 = S1 * S2;
        assert(Set::get_count_nodes() == 14);

        // test
        std::vector<int> A4{3};
        assert(S3 == Set{A4});

        S3 = S1 - S2;
        // test
        std::vector<int> A5{1, 5, 8};
        assert(S3 == Set{A5});
    }

    assert(Set::get_count_nodes() == 0);

    /*****************************************************
     * TEST PHASE 9                                       *
     * Overloaded operators: mixed-mode arithmetic        *
     ******************************************************/
    std::cout << "\nTEST PHASE 9: mixed-mode arithmetic\n";

    {
        std::vector<int> A1{1, 3, 5};
        std::vector<int> A2{2, 3, 4};
        std::vector<int> A3{3, 10};

        Set S1{A1};
        Set S2{A2};
        Set S3{A3};

        // Note: conversion constructor is called
        S3 = 4 - S1 - 5 - (S1 + S2) - 99999;
        assert(Set::get_count_nodes() == 12);
        // test
        assert(S3 == Set{});

        S3 = 3 * S2 + 4;
        assert(Set::get_count_nodes() == 14);
        // test
        assert(S3 == Set(std::vector<int>{3, 4}));

        std::vector<int> A4{3, 4, 24};
        assert((S2 - 2 + S3 + 24) == Set{A4});
        assert(Set::get_count_nodes() == 14);

        S2 += 6;
        assert(Set::get_count_nodes() == 15);

        // test
        A2.push_back(6);
        assert(S2 == Set{A2});
    }
    assert(Set::get_count_nodes() == 0);

    std::cout << "Success!!\n";
}
