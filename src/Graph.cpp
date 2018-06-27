//
// Created by jakub on 6/27/18.
//

#include <iostream>
#include "Graph.h"
#include "GraphFunctions.h"

Graph::Graph() {

}

Graph::Graph(std::string graphInG6String) {

    initializeGraphFromG6(graphInG6String);
}

Graph::~Graph() {

}

void Graph::addEdge(int fromVertex, int toVertex) {

}

void Graph::addVertex(int idOfVertex) {

}

void Graph::initializeGraphFromG6(std::string inputString) {
    // init vector
    std::vector<int> graphVector = {0};
    int graphSize;
    // fill vector with adjacency matrix of graph
    GraphFunctions::readGraphFromG6String(inputString, graphVector, graphSize);

    // at beginning - reserve enough space to avoid reallocation
    // enough just for cubic graphs
    int maxNoEdges = graphSize*2;
    edges.reserve(maxNoEdges);
    // from adjacency matrix - create vector/map structure of graph
    for(int i=0; i<graphSize;i++){
        for(int j= 0; j<graphSize; j++){
            int offset = i * graphSize + j;

            //if there is edge - add edge to edges
            if(graphVector[offset]==1){
                Undirected_edge edge(i, j);
                // TODO at first check if edge isnt already there ...
                edges.push_back(edge);
            }
        }
    }
    //at the end free unused space
    edges.shrink_to_fit();

    // TODO - init also vector of vertices ...


}

void Graph::printEdges() {

    for(Undirected_edge edge : edges){
        std::cout<<edge.from() << " : " <<edge.to() << "\n";
    }

    std::cout<<"number of edges: " << edges.size() << "\n";

}




