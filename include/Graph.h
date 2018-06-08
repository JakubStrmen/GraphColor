/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   GraphColor.h
 * Author: jakub
 *
 * Created on March 7, 2018, 10:44 AM
 */

#include <iostream>
#include <vector>


#ifndef GRAPHCOLOR_H
#define GRAPHCOLOR_H

class Graph {
public:
    Graph();
    Graph(short int *, int);
    Graph(const char *inputFile, const char *format);
    Graph(std::string inputFile, const char *format);
    //Graph(const Graph& orig);
    virtual ~Graph();
    void showGraph(short int *, int);
    void showGraph();
    void showMatrix(int *, int, int);
    int DepthFirstSearchColoring(int);
    int BreadthFirstSearchColoring(short int);
    void writeGraphToFile(short int *, int, const char *);
    int getGraphSize();
    void showColoredGraph();
    Graph removeVertex(int);
    std::vector<Graph> removeEveryAdjacentPair();
    std::vector<Graph> removeEveryNotAdjacentPair();
    bool testCriticality();
    bool testCoCriticality();
    bool testIreducibility();


    //for method from Nauty&Traces - showg.c
    #define BIAS6 63
    #define MAXBYTE 126
    #define SMALLN 62

protected:

    //short int *usedColors;
    int graphSize;
    short int *graph;
    short int *coloredGraph;
    int maxDegree;
    bool edge3Colorable;
    bool isSnark;
    bool isCritical;
    bool isCoCritical;
    bool isStable;
    bool isCoStable;


private:
    int readGraphFromFile(const char *);
    void readGraphFromG6File(std::string);
    void readGraphFromG6String(std::string);
    int getGraphSizeG6(const char *);
    int findMaxDegree();
    void setGraphSize(int);
};

#endif /* GRAPHCOLOR_H */

