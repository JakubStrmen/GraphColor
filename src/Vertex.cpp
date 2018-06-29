//
// Created by jakub on 6/8/18.
//

#include <sstream>
#include "Vertex.h"

Vertex::Vertex() {
    //reserve for cubic graph
    neighbors.reserve(3);
}

Vertex::Vertex(int _id) {
    id = _id;
    //reserve for cubic graph
    neighbors.reserve(3);
}

Vertex::Vertex(int _id, int allocNumber) {
    id = _id;
    //reserve for cubic graph
    neighbors.reserve(allocNumber);
}

Vertex::~Vertex() {

}

void Vertex::setMyEdge(Undirected_edge *edge) {
    myEdge = edge;
}

void Vertex::addNeighbor(int neighbor) {
    neighbors.push_back(neighbor);
}

int Vertex::getId() {
    return id;
}

std::string Vertex::toString() {
    std::stringstream sstream;
    sstream<<id << ": ";

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


