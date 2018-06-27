//
// Created by jakub on 6/8/18.
//

#ifndef GRAPHCOLOR_VERTEX_H
#define GRAPHCOLOR_VERTEX_H


#include "Edge.h"

class Vertex {

//just for cubic graphs - in reduction will be problem
#define MAX_DEGREE 3

public:
    Vertex();
    virtual ~Vertex();


//    Vertex* neighbors[MAX_DEGREE];

    void setMyEdge(Undirected_edge *edge);
private:
    std::vector<int> neighbors;

    // pointre na hrany zatial vynechame
    Undirected_edge *myEdge;
    Undirected_edge *myEdge2;
    std::vector<Undirected_edge*> myEdges;

};


#endif //GRAPHCOLOR_VERTEX_H
