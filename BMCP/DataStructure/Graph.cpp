#include "Graph.hpp"
#include <algorithm>

BMCP::Graph::Graph(int m, int n, int C) : m(m), n(n), C(C)
{
    weight = new int[m + 1];
    profit = new int[n + 1];
    item_neighbor = new IntList[m + 1];
    element_neighbor = new IntList[n + 1];
}

BMCP::Graph::Graph() {}

BMCP::Graph::~Graph()
{
    delete[] weight;
    delete[] profit;
    delete[] item_neighbor;
    delete[] element_neighbor;
}
