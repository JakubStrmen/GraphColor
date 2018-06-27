//
// Created by jakub on 6/27/18.
//

#ifndef GRAPHCOLOR_GRAPHFUNCTIONS_H
#define GRAPHCOLOR_GRAPHFUNCTIONS_H


#include <string>
#include <vector>
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

private:
    static int getGraphSizeG6(const char *);

};


#endif //GRAPHCOLOR_GRAPHFUNCTIONS_H
