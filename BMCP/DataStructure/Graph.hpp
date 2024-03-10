#ifndef BANDBMCP_GRAPH_HPP
#define BANDBMCP_GRAPH_HPP

#include "IntList.hpp"

namespace BMCP
{
    class Graph
    {
    public:
        //knapsack
        int C;
        //number of items
        int m;
        //number of elements
        int n;

        int *weight;
        int *profit;

        //adjacent list
        IntList *item_neighbor;
        IntList *element_neighbor;

        Graph(int, int, int);

        Graph();

        ~Graph();
    };
}

#endif //BANDBMCP_GRAPH_HPP
