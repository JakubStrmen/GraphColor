//
// Created by jakub on 6/8/18.
//

#include <sstream>
#include "Vertex.h"

Vertex::Vertex() {
    //reserve for cubic graph
    neighbors.reserve(3);
    init();

}

Vertex::Vertex(int _id) {
    id = _id;
    //reserve for cubic graph
    neighbors.reserve(3);
    init();
}

Vertex::Vertex(int _id, int allocNumber) {
    id = _id;
    //reserve for cubic graph
    neighbors.reserve(allocNumber);
    init();
}

Vertex::~Vertex() {

}

void Vertex::init() {
    _isColoured= false;
    _colour = -1;
}

//void Vertex::setMyEdge(Undirected_edge *edge) {
//    myEdge = edge;
//}

void Vertex::addNeighbor(int neighbor) {
    neighbors.push_back(neighbor);
}

int Vertex::getId() {
    return id;
}

std::string Vertex::toString() {
    std::stringstream sstream;
    sstream<<id << ": ";

//    for (int i = 0; i < edges.size(); ++i) {
//        sstream<<edges[i]<<"("<< edges[i].getColour()<< ")";
//        if(i==edges.size()-1) break;
//        sstream<<", ";
//
//    }
    for (int i = 0; i < neighbors.size(); ++i) {
        sstream<<neighbors[i];
        if(i==neighbors.size()-1) break;
        sstream<<", ";
    }

    return sstream.str();
}

std::vector<int> Vertex::getNeighbors() {
    return neighbors;
}


std::vector<Undirected_edge> Vertex::getEdges() {
    return edges;
}

void Vertex::setColour(int newColour) {
    _colour=newColour;
    _isColoured = true;

}

int Vertex::getColour() {
    return _colour;
}

void Vertex::addEdge(Undirected_edge edge) {
    edges.push_back(edge);
}

void Vertex::colourEdge(Undirected_edge inEdge, int colour) {
    for(Undirected_edge edge: edges){
        if (inEdge==edge) edge.setColour(colour);
    }

}




