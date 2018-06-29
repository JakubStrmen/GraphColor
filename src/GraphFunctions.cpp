//
// Created by jakub on 6/27/18.
//

#include "GraphFunctions.h"
#include <iostream>
#include <bitset>
#include <fstream>


GraphFunctions::GraphFunctions() {

}

GraphFunctions::~GraphFunctions() {

}

void GraphFunctions::readGraphFromG6File(std::string inputFile, std::vector<int> &adjMatrix){

    std::string line;
    std::ifstream myfile;

    // TODO - opens even not existing file ...
    try {
        // open input file
        myfile.open(inputFile);

        // read line from input file
        std::getline(myfile, line);

        myfile.close();
    }catch (std::ifstream::failure e){
        std::cerr << "Exception opening/reading/closing file!! \n";
    }

    int graphSize;
    readGraphFromG6String(line, adjMatrix, graphSize);

}


void GraphFunctions::readGraphFromG6String(std::string inputString, std::vector<int> &adjMatrix, int &graphSize) {
    // convert string to char*
    const char *cstr = inputString.c_str();
    int size = getGraphSizeG6(cstr);
    graphSize = size;

    adjMatrix.reserve(size*size);

    // check size of graph and remove first few bytes from inputString according to size
    if(size >= 0 && size < 63) inputString.erase(0, 1);
    else if(size >= 63 && size < 258048) inputString.erase(0, 4);
    else if (size >= 258048 && size < 68719476735) inputString.erase(0, 8);
    else std::cout<<"We have problem!! too large graph or something else!!! \n";

    // convert line of characters to binary form (line of 0 and 1)
    std::string graphInLine = "";
    for(char& c : inputString){
        int value = c - BIAS6;
        std::string binary = std::bitset<6>(value).to_string();
        graphInLine += binary;
    }

    // store values from binary line form to graph matrix
    // TODO - necessary here?
    int i, j, offset, offset2;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            offset = i * size +j;
            adjMatrix[offset]=0;
        }
    }


    i=0; j=1;
    int border = 0;
    // put every bit value to the right place in graph matrix
    for(char& c : graphInLine){
        short int bit;
        offset = i* size +j;
        offset2 = j*size +i;
        if(c=='0') bit = 0;
        else        bit = 1;

        // offset - for upper triangle
        adjMatrix[offset]=bit;
        // offset2 - for lower triangle
        adjMatrix[offset2]=bit;

        // if remains some zeros in the end of line
        if((i+2)==size && (j+1)==size) break;
        // border+1 = how many values fits in actual column j
        if(i==border){
            j++;
            i=0;
            border++;
        }else if(i<border){
            i++;
        }
    }

}

/**
 * Method from Nauty&Traces - showg.c
 * @param s
 * @return
 */
int GraphFunctions::getGraphSizeG6(const char *s){

    const char *p;
    int n;

    if (s[0] == ':' || s[0] == '&') p = s+1;
    else                            p = s;
    n = *p++ - BIAS6;

    if (n > SMALLN)
    {
        n = *p++ - BIAS6;
        if (n > SMALLN)
        {
            n = *p++ - BIAS6;
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
        }
        else
        {
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
        }
    }
    return n;

}

// TODO - how to do this at best?

void GraphFunctions::addEdge(int fromVertex, int toVertex, std::vector<Undirected_edge> &edges) {
    if(!findEdge(fromVertex, toVertex, edges)){
        Undirected_edge newEdge(fromVertex, toVertex);
        edges.push_back(newEdge);
    }
}

void GraphFunctions::addEdge(Undirected_edge newEdge, std::vector<Undirected_edge> &edges) {
    if(!findEdge(newEdge.from(), newEdge.to(), edges)){
        edges.push_back(newEdge);
    }
}

void GraphFunctions::addVertex(Vertex newVertex, std::vector<Vertex> &vertices) {
    if (!findVertex(newVertex.getId(), vertices)) vertices.push_back(newVertex);
}

// returns position of edge in vector
bool GraphFunctions::findEdge(int from, int to, std::vector<Undirected_edge> &edges) {
    if (from > to)
        std::swap(from, to);

    for (int i=0; i<edges.size(); i++){
        if(from == edges[i].from() && to==edges[i].to()){
            return true;
        }
    }
    return false;
}

bool GraphFunctions::findEdge(int from, int to, int &position, std::vector<Undirected_edge> &edges) {
    if (from > to)
        std::swap(from, to);

    for (int i=0; i<edges.size(); i++){
        if(from == edges[i].from() && to==edges[i].to()){
            position = i;
            return true;
        }
    }
    return false;
}

bool GraphFunctions::findVertex(int vertexId, std::vector<Vertex> &vertices) {
    for (Vertex vertex : vertices){
        if (vertex.getId()==vertexId) return true;
    }
    return false;
}


// TODO - complexity??
Graph GraphFunctions::reduceEdge3Col_Vert3Col(Graph inputGraph) {
    // approx max number of edges in new graph = 3*inputGraph.vertices.size --for cubic graph
    // number of vertices in new graph = inputGraph.edges.size
    int reserveEdges = 3* inputGraph.getNoEdges();
    int reserveVertices = inputGraph.getNoVertices();

    // load edges and vertices from input graph
    std::vector<Undirected_edge> inputEdges = inputGraph.getEdges();
    std::vector<Vertex> inputVertices = inputGraph.getVertices();

    //init edges and vertices for output graph
    std::vector<Undirected_edge> outEdges;
        outEdges.reserve(reserveEdges);
    std::vector<Vertex> outVertices;
        outVertices.reserve(reserveVertices);

    int counter=0;
    for (Undirected_edge inEdge : inputEdges){
        int edgePosition = counter;
        int otherEdgePosition;
        Vertex newVertex(counter, 4); // alloc 4 for transformed cubic graph

        int i =-1;
        // for every edge with common vertex - add outEdge
        for(Undirected_edge otherEdge : inputEdges){
            i++;
            // if the same edge - continue
            if (inEdge==otherEdge) continue;

            //if have in common FROM vertex
            if(inEdge.from()==otherEdge.from() || inEdge.from()==otherEdge.to()){
                otherEdgePosition = inputGraph.getEdgePosition(otherEdge);
                Undirected_edge outEdge(counter, otherEdgePosition);
                addEdge(outEdge, outEdges);
                newVertex.addNeighbor(otherEdgePosition);
            }else
            // else if have in common TO vertex
            if (inEdge.to()==otherEdge.from() || inEdge.to()==otherEdge.to()){
                otherEdgePosition = inputGraph.getEdgePosition(otherEdge);
                Undirected_edge outEdge(counter, otherEdgePosition);
                addEdge(outEdge, outEdges);
                newVertex.addNeighbor(otherEdgePosition);
            }

        }

        outVertices.push_back(newVertex);
        counter++;
    }

    Graph outputGraph(outEdges, outVertices);

    return outputGraph;
}



Glucose::Solver GraphFunctions::reduceVert3Col_SAT(Graph inputGraph) {
    return Glucose::Solver();
}
