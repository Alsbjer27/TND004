/*************************************************
 * file:	~\code4a\edge.h                      *
 * remark: interface for directed weighted edges *
 *************************************************/

#pragma once

#include <iostream>
#include <format>
#include <compare>  // three-way comparison operator <=>

// Represents a directed edge 'from' 'head 'to' with weight
class Edge {
public:
    // -- CONSTRUCTORS
    Edge(int u = 0, int v = 0, int w = 0) : from{u}, to{v}, weight{w} {
    }

    bool links_same_nodes(const Edge &e) const {
        return (from == e.from && to == e.to);
    }

    Edge reverse() const {
        return {to, from, weight};
    }

    // -- Three way comparison operator
    // used in exercise of Part B
    std::partial_ordering operator<=>(const Edge &e) const {
        return weight <=> e.weight;
    }

    // -- FRIENDS
    friend std::ostream &operator<<(std::ostream &os, const Edge &e) {
        os << std::format("({:2}, {:2}, {:2}) ", e.from, e.to, e.weight);
        return os;
    }

    // -- DATA MEMBERS: represent an edge 'from' 'to' with 'weight'
    int from;
    int to;
    int weight;
};
