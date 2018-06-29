//
// Created by jakub on 6/27/18.
//

#ifndef GRAPHCOLOR_GRAPHFUNCTIONS_H
#define GRAPHCOLOR_GRAPHFUNCTIONS_H


#include <string>
#include <vector>
#include <Solver.h>
#include "Graph.h"

class GraphFunctions {

//for method from Nauty&Traces - showg.c
#define BIAS6 63
#define SMALLN 62

public:
    GraphFunctions();
    virtual ~GraphFunctions();

    static void readGraphFromG6String(std::string g6String, std::vector<int> &adjMatrix, int &graphSize);
    static void readGraphFromG6File(std::string, std::vector<int> &adjMatrix);

    static Graph reduceEdge3Col_Vert3Col(Graph inputGraph);
    static void reduceVert3Col_SAT(Graph &inputGraph, Glucose::Solver& solver);

    static void do3COL_withSAT(Graph &inputGraph);

    static bool findEdge(int from, int to, std::vector<Undirected_edge> &edges);
    static bool findEdge(int from, int to, int& position, std::vector<Undirected_edge> &edges);
    static bool findVertex(int vertexId, std::vector<Vertex> &vertices);
    static void addVertex(Vertex newVertex, std::vector<Vertex> &vertices);
    static void addEdge(int fromVertex, int toVertex, std::vector<Undirected_edge> &edges);
    static void addEdge(Undirected_edge newEdge, std::vector<Undirected_edge> &edges);
private:
    static int getGraphSizeG6(const char *);

};


#endif //GRAPHCOLOR_GRAPHFUNCTIONS_H
