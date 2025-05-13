/*********************************************
 * file:	~\code4a\digraph.cpp              *
 * remark: implementation of directed graphs  *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <limits>  //std::numeric_limits
#include <vector>
#include <cassert>
#include <queue>
#include <format>

#include "digraph.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

Digraph::Digraph(int n)
    : table(n + 1)  // slot zero not used
    , size{n}       // number of verices
    , n_edges{0}
    , dist(n + 1)
    , path(n + 1)
    , done(n + 1) {
    assert(n >= 1);
    // Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<Edge>& V, int n) : Digraph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

// insert directed edge e = (u, v, w)
// update weight w if edge (u, v) is present
void Digraph::insertEdge(const Edge& e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    // Check if edge e already exists
    if (auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
                               [e](const Edge& ed) { return e.links_same_nodes(ed); });
        it == end(table[e.from])) {
        table[e.from].push_back(e);  // insert new edge e
        ++n_edges;
    } else {
        it->weight = e.weight;  // update the weight
    }
}

// remove directed edge e
void Digraph::removeEdge(const Edge& e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
                           [e](const Edge& ed) { return e.links_same_nodes(ed); });

    assert(it != end(table[e.from]));
    table[e.from].erase(it);
    --n_edges;
}

// construct unweighted single source shortest path-tree for start vertex s
void Digraph::uwsssp(int s) const {
    assert(s >= 1 && s <= size);

    // *** TODO ***

    std::queue<int> Q; // Queue<vertex> q;

    // Using the pseudo-code from Course book figure 9.18
    for (size_t i = 0; i <= size; i++) {
        dist[i] = std::numeric_limits<int>::max();
        path[i] = 0;
    }

    dist[s] = 0;
    Q.push(s); //q.enqueue(s)

    while (!Q.empty()) {
        int v = Q.front(); // Vertex v = q.dequeue 
        Q.pop();

        for (const auto& e : table[v]) { // for each vertex w adjacent to v
            int w = e.to;
            if (dist[w] == std::numeric_limits<int>::max()) { // if(w.dist == INF)
                dist[w] = dist[v] + 1;
                path[w] = v;
                Q.push(w);
            }
        }
    }
}

// construct positive weighted single source shortest path-tree for start vertex s
// Dijktra’s algorithm
void Digraph::pwsssp(int s) const {
    assert(s >= 1 && s <= size);

    // *** TODO ***

    for (size_t i = 1; i <= size; i++){
        dist[i] = std::numeric_limits<int>::max();
        path[i] = 0;
        done[i] = false;
    }
   dist[s] = 0;
   dist[s] = true;
   int v = s;

   while (true) {
       int v = -1;
       int minDist = std::numeric_limits<int>::max();

       for (int i = 0; i < size; ++i) {
           if (!done[i] && dist[i] < minDist) {
               minDist = dist[i];
               v = i;
           }
       }
       if (v == -1) break;
       done[v] = true;

       for (const auto& e : table[v]) {
           int w = e.to;
           if (!done[w]) {
               int cost = e.weight;
               if (dist[v] + cost < dist[w]) {
                   dist[w] = dist[v];
                   path[w] = v;
               }
           }
       }
   }
}

// print graph
void Digraph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; ++v) {
        std::cout << std::format("{:4} : ", v);
        for (auto const& e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}

// print shortest path tree for s
void Digraph::printTree() const {
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";
    std::cout << "vertex    dist    path\n";
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";

    for (int v = 1; v <= size; ++v) {
        std::cout << std::format("{:4} : {:6} {:6}\n", v,
                                 ((dist[v] == std::numeric_limits<int>::max()) ? -1 : dist[v]),
                                 path[v]);
    }
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";
}

// print shortest path from s to t and the corresponding path length
// Hint: consider using recursion
void Digraph::printPath(int t) const {
    assert(t >= 1 && t <= size);

    // *** TODO ***
    printRecursion(t);
    std::cout << "(" << dist[t] << ")";
}

void Digraph::printRecursion(int t) const {
    if(path[t] == 0){
        std::cout << t;
        return;
    }

    printRecursion(path[t]);
    std::cout << " -> " << t;
}