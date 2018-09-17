//
// Created by jakub on 6/27/18.
//

#ifndef GRAPHCOLOR_GRAPHFUNCTIONS_H
#define GRAPHCOLOR_GRAPHFUNCTIONS_H


#include <string>
#include <vector>
#include <Solver.h>
#include <queue>
#include <fstream>
#include "Graph.h"
#include <list>

class GraphFunctions {

//for method from Nauty&Traces - showg.c
#define BIAS6 63
#define SMALLN 62
#define COLORS 3

public:

    static void readGraphFromG6File(std::string, std::vector<int> &adjMatrix);

    static Graph readGraphFromG6String(std::string graphInG6String);
    static Graph readGraphFromG6file(std::string path);
    static Graph graphFrom_BAfile(std::string path); //std::ifstream &inputStream

    static Graph reduceEdge3Col_Vert3Col(Graph inputGraph);
    static void reduceVert3Col_SAT(Graph &inputGraph, Glucose::Solver& solver);

    static void do3COL_withSAT(Graph &inputGraph);
    static void edge3COLBFS(Graph inputGraph);
    static void edge3COLBFS(Graph inputGraph, int firstVertex);
    static bool findEdge(int from, int to, int& position, std::vector<Undirected_edge> &edges);

    static bool findVertex(int vertexId, std::vector<Vertex> &vertices);
    static void addVertex(Vertex newVertex, std::vector<Vertex> &vertices);
    static void addEdge(int fromVertex, int toVertex, std::vector<Undirected_edge> &edges);
    static void addEdge(Undirected_edge newEdge, std::vector<Undirected_edge> &edges);
    //TODO - farbit hrany postupne podla listu
    static void edge3COLedgeList();

    class Vertex_coloring{
    public:
        Vertex_coloring(Vertex parent);

        Vertex core;
        std::vector<int> availableColors;
    };

    static void testBFS(Graph inputGraph);


private:

    static void readGraphFromG6String(std::string g6String, std::vector<int> &adjMatrix, int &graphSize);

    static bool colourEdgesOfVertex(Vertex_coloring &vertex, std::map<Undirected_edge, Undirected_edge> &edges, std::map<int, Vertex_coloring> &vertices);
    static bool colourEdge(Undirected_edge &edge, std::map<Undirected_edge, Undirected_edge> &edges, std::map<int, Vertex_coloring> &vertices, int colour);

    static int getGraphSizeG6(const char *);
    static bool colourVertex(std::map<int, Vertex> &vertices, Vertex &vertex, int colour);

    static bool findEdge(int from, int to, std::vector<Undirected_edge> &edges);


    class BFScolouriser{
    public:
        std::list<int> list;
        bool listContains(std::list<int> &list, int number);
        bool edge3COLBFSrecursive(std::map<int, GraphFunctions::Vertex_coloring> &vertices, std::map<Undirected_edge, Undirected_edge> &edges, Vertex_coloring &vertex);

    };



};


#endif //GRAPHCOLOR_GRAPHFUNCTIONS_H
