//
// Created by jakub on 6/27/18.
//

#include <iostream>
#include "Graph.h"
#include "GraphFunctions.h"

Graph::Graph() {

}

Graph::Graph(int maxNoEdges, int maxNoVertices) {
    edgesVector.reserve(maxNoEdges);
    verticesVector.reserve(maxNoVertices);
}

Graph::Graph(std::vector<Undirected_edge> _edges, std::vector<Vertex> _vertices) {
    edgesVector=_edges;
    verticesVector=_vertices;
}


Graph::Graph(std::string graphInG6String) {

    initGraphFromG6String(graphInG6String);
}

Graph::~Graph() {

}

void Graph::addEdge(int fromVertex, int toVertex) {
    if(!findEdge(fromVertex, toVertex)){
        Undirected_edge newEdge(fromVertex, toVertex);
        edgesVector.push_back(newEdge);
    }
}

void Graph::addEdge(Undirected_edge newEdge) {
    if(!findEdge(newEdge.from(), newEdge.to())){
        edgesVector.push_back(newEdge);
    }
}

void Graph::addVertex(Vertex newVertex) {
    if (!findVertex(newVertex.getId())) verticesVector.push_back(newVertex);
}

void Graph::initGraphFromG6String(std::string inputString) {
    // init vector
    std::vector<int> graphVector = {0};
//    int graphSize;
    // fill vector with adjacency matrix of graph
    GraphFunctions::readGraphFromG6String(inputString, graphVector, graphSize);


//    initEdges(graphVector);
    initEdgesAndVertices(graphVector);


    // TODO - init also vector of vertices ...


}

// TODO - improve findEdge - maybe replace with better data structure (unordered_set/hashSet)
//// with vector
//void Graph::initEdges(std::vector<int> &graphVector) {
//    // at beginning - reserve enough space for edges (to avoid reallocation)
//    // enough just for cubic graphs
//    int maxNoEdges = graphSize*2;
//    edges.reserve(maxNoEdges);
//    // from adjacency matrix - create vector/map structure of graph
//    for(int i=0; i<graphSize;i++){
//        for(int j= 0; j<graphSize; j++){
//            int offset = i * graphSize + j;
//
//            //if there is edge - add edge to edges
//            if(graphVector[offset]==1){
//                Undirected_edge edge(i, j);
//                // if edge not already there - add
//                if(!findEdge(i, j)) edges.push_back(edge);
//            }
//        }
//    }
//    //at the end free unused space
//    edges.shrink_to_fit();
//
//}

// inits edges to vector and vertices to vector
void Graph::initEdgesAndVertices(std::vector<int> &graphVector) {
    // at beginning - reserve enough space for edges (to avoid reallocation)
    // enough just for cubic graphs
    int maxNoEdges = graphSize*2;
    edgesVector.reserve(maxNoEdges);
    //reserve space for vertices
    verticesVector.reserve(graphSize);

    // TODO - could work just with upper triangle of adjMatrix
    // from adjacency matrix - create vector/map structure of graph
    for(int i=0; i<graphSize;i++){
        Vertex vertex(i);
        for(int j= 0; j<graphSize; j++){
            int offset = i * graphSize + j;

            //if there is edge - add edge to edges
            //also add neighbor to vertex
            if(graphVector[offset]==1){
                // if edge not already there - add
                Undirected_edge edge(i, j);
                if(!findEdge(i, j)) {
                    edgesVector.push_back(edge);
                }
                vertex.addNeighbor(j);
                vertex.addEdge(edge);
            }
        }
        verticesVector.push_back(vertex);
    }
    //at the end free unused space
    edgesVector.shrink_to_fit();


}



void Graph::printEdges() {

    for(Undirected_edge edge : edgesVector){
        std::cout<<edge.from() << " : " <<edge.to() << "\n";
    }

    std::cout<<"number of edges: " << edgesVector.size() << "\n";

}


void Graph::printVertices() {
    for(Vertex vertex : verticesVector){
        std::cout<<vertex.toString()<<"\n";
    }
    std::cout<<"number of vertices: " << verticesVector.size() << "\n";

}

// returns position of edge in vector
bool Graph::findEdge(int from, int to) {
    if (from > to)
        std::swap(from, to);

    for (int i=0; i<edgesVector.size(); i++){
        if(from == edgesVector[i].from() && to==edgesVector[i].to()){
            return true;
        }
    }
    return false;
}

bool Graph::findEdge(int from, int to, int &position) {
    if (from > to)
        std::swap(from, to);

    for (int i=0; i<edgesVector.size(); i++){
        if(from == edgesVector[i].from() && to==edgesVector[i].to()){
            position = i;
            return true;
        }
    }
    return false;
}

int Graph::getEdgePosition(Undirected_edge edge) {
    int position;
    findEdge(edge.from(), edge.to(), position);

    return position;
}

bool Graph::findVertex(int vertexId) {
    for (Vertex vertex : verticesVector){
        if (vertex.getId()==vertexId) return true;
    }
    return false;
}

Vertex Graph::getVertex(int vertexId) {
    Vertex returnVertex;
    for (Vertex vertex : verticesVector){
        if (vertex.getId()==vertexId) returnVertex = vertex;
    }
    return returnVertex;
}

// returns number of edges
int Graph::getNoEdges() {
    return edgesVector.size();
}

// returns number of vertices in vector
int Graph::getNoVertices() {
    return verticesVector.size();
}

std::vector<Undirected_edge> Graph::getEdges() {
    return edgesVector;
}

std::vector<Vertex> Graph::getVertices() {
    return verticesVector;
}

std::string Graph::toString() {
    std::string output="";
    for (int j = 0; j < graphSize; ++j) {
        output+=verticesVector[j].toString();
        output+="\n";
    }

    return output;
}

// ---- also verticesVector???
void Graph::colourEdge(Undirected_edge &edge, int colour) {
    // at first - colour edge in vector of edges
    edge.setColour(colour);

    // then - change also info about edges in verticesMap
    verticesMap.at(edge.from()).colourEdge(edge, colour);
    verticesMap.at(edge.to()).colourEdge(edge, colour);

}
















