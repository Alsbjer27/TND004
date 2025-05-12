/*********************************************
 * file:	~\code4b\graph.h                  *
 * remark: interface for undirected graphs    *
 **********************************************/

#pragma once

#include <vector>
#include <list>

#include "edge.h"

class Graph {
public:
    // -- CONSTRUCTOR
   
    // Create a graph with n vertices and the edges in V
    Graph(const std::vector<Edge>& V, int n);

    // Disallow copying
    Graph(const Graph &) = delete;
    Graph &operator=(const Graph &) = delete;

    // -- MEMBER FUNCTIONS

    // insert undirected edge (u, v) with weight w
    // update weight w if edge (u, v) is present
    void insertEdge(const Edge& e);

    // remove undirected edge e
    void removeEdge(const Edge& e);

    // Prim's minimum spanning tree algorithm
    void mstPrim() const;

    // Kruskal's minimum spanning tree algorithm
    void mstKruskal() const;

    // print graph
    void printGraph() const;

private:
    // -- Private CONSTRUCTOR
    explicit Graph(int n);  // Create a graph with n vertices and no vertices

    // -- DATA MEMBERS
    std::vector<std::list<Edge>> table;  // table of adjacency lists
    int size;                            // number of vertices
    int n_edges;                         // number of edges
};
