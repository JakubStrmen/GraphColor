//
// Created by jakub on 6/27/18.
//

#include "GraphFunctions.h"
#include "SATSolver.h"
#include <iostream>
#include <bitset>
#include <fstream>
#include <algorithm>



GraphFunctions::Vertex_coloring::Vertex_coloring(Vertex parent) {
    core=parent;
    availableColors.reserve(COLORS);
    for (int i = 0; i < COLORS; ++i) {
        availableColors.push_back(i);
    }
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



// to simplify work - for local store of variables of every vertex of graph
struct SATvertex{
    int vertex;
    std::vector<int> vars;
};
//TODO - not working correctly - resolved?
// for now, done for vertex - 3Coloring
void GraphFunctions::reduceVert3Col_SAT(Graph &inputGraph, Glucose::Solver& solver) {
    Glucose::vec<Glucose::Lit> lits;
    int vars    = 0;
    int clauses = 0;
    int cnt     = 0;


    //generate clauses from input graph
    vars = inputGraph.getNoVertices()*3+1;
    clauses = (5*inputGraph.getNoVertices()) + (3*inputGraph.getNoEdges());

//    std::vector<Vertex> inputVert = inputGraph.getVertices();
    std::vector<Undirected_edge> inputEdges = inputGraph.getEdges();

    //for local store of vertices vars
    std::vector<SATvertex> SATvertices;
    SATvertices.reserve(inputGraph.getNoVertices());

    //add clauses for vertices
    {
        //for local store of inserting lits
        std::vector<int > parsed_lits;
        parsed_lits.reserve(3);


        // add clauses for every vertex
        // every vertex has to be coloured with one and only one colour
        for (int i = 0; i < inputGraph.getNoVertices(); ++i) {
            // init vars for vert
            SATvertex vert;
            vert.vars.reserve(3);
            for (int k = 0; k < 3; ++k) {
                //var indexes from 0 -> numOfVertices*3
                vert.vars.push_back(((i*3)+k)+1); // +1 to avoid having 0 as var
            }
            SATvertices.push_back(vert);
            while (vert.vars.at(2) >= solver.nVars()) solver.newVar();


            // for each vertex add 5 clauses - not in loop
            int parsed_lit, var;
            lits.clear();

            //first clause (xi0 OR xi1 OR xi2)
            parsed_lits.push_back(SATvertices[i].vars[0]);
            parsed_lits.push_back(SATvertices[i].vars[1]);
            parsed_lits.push_back(SATvertices[i].vars[2]);
            for (int l = 0; l < parsed_lits.size(); ++l) {
                var = abs(parsed_lits[l]); // not -1  -> vars from 0, not from 1 as from dimacs file
                lits.push( (parsed_lits[l] > 0) ? Glucose::mkLit(var) : ~Glucose::mkLit(var) );
            }
            parsed_lits.clear();
            cnt++;
            solver.addClause(lits);


            //second clause (-xi0 OR -xi1 OR xi2)
            lits.clear();
            parsed_lits.push_back(-SATvertices[i].vars[0]);
            parsed_lits.push_back(-SATvertices[i].vars[1]);
            parsed_lits.push_back(SATvertices[i].vars[2]);
            for (int l = 0; l < parsed_lits.size(); ++l) {
                var = abs(parsed_lits[l]); // not -1  -> vars from 0, not from 1 as from dimacs file
                lits.push( (parsed_lits[l] > 0) ? Glucose::mkLit(var) : ~Glucose::mkLit(var) );
            }
            parsed_lits.clear();
            cnt++;
            solver.addClause(lits);

            //third clause (-xi0 OR xi1 OR -xi2)
            lits.clear();
            parsed_lits.push_back(-SATvertices[i].vars[0]);
            parsed_lits.push_back(SATvertices[i].vars[1]);
            parsed_lits.push_back(-SATvertices[i].vars[2]);
            for (int l = 0; l < parsed_lits.size(); ++l) {
                var = abs(parsed_lits[l]); // not -1  -> vars indexed from 0, not from 1 as from dimacs file
                lits.push( (parsed_lits[l] > 0) ? Glucose::mkLit(var) : ~Glucose::mkLit(var) );
            }
            parsed_lits.clear();
            cnt++;
            solver.addClause(lits);

            //fourth clause (xi0 OR -xi1 OR -xi2)
            lits.clear();
            parsed_lits.push_back(SATvertices[i].vars[0]);
            parsed_lits.push_back(-SATvertices[i].vars[1]);
            parsed_lits.push_back(-SATvertices[i].vars[2]);
            for (int l = 0; l < parsed_lits.size(); ++l) {
                var = abs(parsed_lits[l]); // not -1  -> vars indexed from 0, not from 1 as from dimacs file
                lits.push( (parsed_lits[l] > 0) ? Glucose::mkLit(var) : ~Glucose::mkLit(var) );
            }
            parsed_lits.clear();
            cnt++;
            solver.addClause(lits);

            //fifth clause (-xi0 OR -xi1 OR -xi2)
            lits.clear();
            parsed_lits.push_back(-SATvertices[i].vars[0]);
            parsed_lits.push_back(-SATvertices[i].vars[1]);
            parsed_lits.push_back(-SATvertices[i].vars[2]);
            for (int l = 0; l < parsed_lits.size(); ++l) {
                var = abs(parsed_lits[l]); // not -1  -> vars indexed from 0, not from 1 as from dimacs file
                lits.push( (parsed_lits[l] > 0) ? Glucose::mkLit(var) : ~Glucose::mkLit(var) );
            }
            parsed_lits.clear();
            cnt++;
            solver.addClause(lits);

        }
    }

    //add clauses for edges
    int edgeCount = 0;
    for(Undirected_edge inEdge: inputEdges){

        // every of 3 colors
        for (int i = 0; i < 3; ++i) {
            int parsed_lit1, parsed_lit2, var;
            lits.clear();

            //adds negated literals for current edge and current color
            parsed_lit1 = -SATvertices[inEdge.from()].vars[i];
            parsed_lit2 = -SATvertices[inEdge.to()].vars[i];

            var = abs(parsed_lit1); // not -1  -> vars from 0, not from 1 as from dimacs file
            lits.push( (parsed_lit1 > 0) ? Glucose::mkLit(var) : ~Glucose::mkLit(var) );
            var = abs(parsed_lit2); // not -1  -> vars from 0, not from 1 as from dimacs file
            lits.push( (parsed_lit2 > 0) ? Glucose::mkLit(var) : ~Glucose::mkLit(var) );
            cnt++;
            solver.addClause(lits);

        }
    }

    if (vars != solver.nVars())
        fprintf(stderr, "WARNING! DIMACS header mismatch: wrong number of variables.\n");
    if (cnt  != clauses)
        fprintf(stderr, "WARNING! DIMACS header mismatch: wrong number of clauses.\n");


}

/**
 * performs vertex 3 colouring using SAT
 * @param inputGraph
 */
void GraphFunctions::do3COL_withSAT(Graph &inputGraph) {

    SATSolver solver;
    solver.doSATSolve3COL(inputGraph);

}

void GraphFunctions::edge3COLBFS(Graph inputGraph) {
    edge3COLBFS(inputGraph, 0);
}

void GraphFunctions::edge3COLBFS(Graph inputGraph, int firstVertex) {
    //preprocessing
    std::map<int, Vertex_coloring> vertices;
    std::map<Undirected_edge, Undirected_edge> edges;
    int noVertices = inputGraph.getNoVertices();

    std::vector<Vertex> tempVert(inputGraph.getVertices());

    for (int i = 0; i < noVertices; ++i) {
        Vertex_coloring vert(tempVert[i]);
        std::pair<int, Vertex_coloring> pair(tempVert[i].getId(), vert);
        vertices.insert(pair);

    }

    for(Undirected_edge edge: inputGraph.getEdges()){
        std::pair<Undirected_edge, Undirected_edge> pair(edge, edge);
        edges.insert(pair);
    }

    // colouriser


    // ----------
    for (int j = 0; j < COLORS; ++j) {
        int colour = j;
        vertices.at(firstVertex).core.setColour(colour);
        // recursive call

    }

    BFScolouriser colouriser;
    colouriser.queue.push(firstVertex);
    colouriser.edge3COLBFSrecursive(vertices);

//    Undirected_edge edge(0, 1);
//    int farba = 3;
//    std::pair<Undirected_edge, Undirected_edge > pair(edge, edge);
//    std::map<Undirected_edge, Undirected_edge> edgesMap;
//    edgesMap.insert(pair);
//
//    std::cout << edgesMap.at(edge).getColour() << "\n";
//
//    Undirected_edge edge2(0, 2);
//    edge2.setColour(4);
//    std::pair<Undirected_edge, Undirected_edge > pair2(edge2, edge2);
//    edgesMap.insert(pair2);
//
//    std::cout << edgesMap.at(edge2).getColour() << "\n";


}


bool GraphFunctions::colourVertex(std::map<int, Vertex> &vertices, Vertex &vertex, int colour) {
    std::vector<int> neighbors(vertex.getNeighbors());

    for(int neighbor: neighbors){
        if(vertices.at(neighbor).getColour()==colour){
            return false;
        }
    }


}

void GraphFunctions::colourEdgesOfVertex(Vertex_coloring &vertex, std::map<Undirected_edge, Undirected_edge> edges, std::map<int, Vertex_coloring> vertices) {

    for(Undirected_edge edge: vertex.core.getEdges()){
//        Undirected_edge tempEdge = edges.at(edge);
        int to;
        // if edge not already colored
        if (!edges.at(edge).coloured()){
            (vertex.core.getId()==edge.from()) ? to=edge.to() : to = edge.from();
            Vertex_coloring *toVert = &vertices.at(to);

            // find intersection of available colors for both vertices
            std::sort(vertex.availableColors.begin(), vertex.availableColors.end());
            std::sort(toVert->availableColors.begin(), toVert->availableColors.end());
            std::vector<int> result(COLORS);
            std::set_intersection(vertex.availableColors.begin(), vertex.availableColors.end(), toVert->availableColors.begin(), toVert->availableColors.end(), result.begin());

            if (!result.empty()) {
                edges.at(edge).setColour(result[0]);
                // remove from available colors recently used color
//                toVert->availableColors.

            }

        }
    }
}


void GraphFunctions::BFScolouriser::edge3COLBFSrecursive(std::map<int, Vertex_coloring> &inVertices) {
//    std::map<int, Vertex> vertices(inVertices);

//    int cislo = queue.front();
//    std::cout<< cislo << "\n";

}


