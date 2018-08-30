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
    Vertex(int _id);
    Vertex(int _id, int allocNumber);
    virtual ~Vertex();


//    Vertex* neighbors[MAX_DEGREE];

//    void setMyEdge(Undirected_edge *edge);

    void addNeighbor(int neighbor);
    void addEdge(Undirected_edge edge);
    int getId();
    void setColour(int newColour);
    int getColour();
    void colourEdge(Undirected_edge edge, int colour);

    std::vector<int> getNeighbors();
    std::vector<Undirected_edge> getEdges();
    std::string toString();
private:

    int id;
    std::vector<int> neighbors;
    std::vector<Undirected_edge> edges;

    int _colour;
    bool _isColoured;


    // pointre na hrany zatial vynechame
    Undirected_edge *myEdge;
    Undirected_edge *myEdge2;
    std::vector<Undirected_edge*> myEdges;
    void init();

};


#endif //GRAPHCOLOR_VERTEX_H
