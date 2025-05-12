/*********************************************
* file:	~\code4b\dsets.cpp                   *
* remark: interface for disjoint sets        *
**********************************************/
 #pragma once

#include <vector>

class DSets
{
public:
    // -- CONSTRUCTORS
    explicit DSets(int theSize = 10);

	//Disallow copying
	DSets(const DSets&) = delete;
	DSets& operator=(const DSets&) = delete;

    // -- MEMBER FUNCTIONS

    // create initial sets
    void init();

    // join sets named r and s where r != s
    // i.e. join trees with roots r and s
    void join(int r, int s);

    // return name of current set for x
    // i.e. return root of tree for x
    int find(int x);

    // just in case ...
    void print() const;

private:
    // -- DATA MEMBERS
    std::vector<int> V;
};
