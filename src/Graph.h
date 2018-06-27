//
// Created by jakub on 6/27/18.
//

#ifndef GRAPHCOLOR_GRAPH_H
#define GRAPHCOLOR_GRAPH_H


#include <vector>
#include <map>
#include "Edge.h"
#include "Vertex.h"

// should be universal class for graphs
class Graph {
public:
    Graph();

    Graph(std::string graphInG6String);

    virtual ~Graph();

    void addVertex(int idOfVertex);
    void addEdge(int fromVertex, int toVertex);

    void printEdges();

private:
    int graphSize;
    Vertex vertex;
    // TODO - to discuss - if map/vector
    std::map<int, Vertex> vertices;
    std::vector<Undirected_edge> edges;

    void initializeGraphFromG6(std::string inputString);

};


#endif //GRAPHCOLOR_GRAPH_H
