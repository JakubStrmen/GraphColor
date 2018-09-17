//
// Created by jakub on 6/27/18.
//

#include "GraphFunctions.h"
#include "SATSolver.h"
#include "../BA_GraphFormat/io_bagraph.h"
#include <iostream>
#include <bitset>
#include <fstream>
#include <algorithm>
#include <sstream>


GraphFunctions::Vertex_coloring::Vertex_coloring(Vertex parent) {
    core=parent;
    availableColors.reserve(COLORS);
    for (int i = 0; i < COLORS; ++i) {
        availableColors.push_back(1); // i
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

Graph GraphFunctions::readGraphFromG6file(std::string path) {

    return Graph();
}

Graph GraphFunctions::readGraphFromG6String(std::string graphInG6String) {
    // init vector
    std::vector<int> graphVector = {0};
    int graphSize = 0;
    // fill vector with adjacency matrix of graph
    GraphFunctions::readGraphFromG6String(graphInG6String, graphVector, graphSize);

// inits edges to vector and vertices to vector
    // at beginning - reserve enough space for edges (to avoid reallocation)
    // enough just for cubic graphs
    int maxNoEdges = graphSize*2;
    std::vector<Undirected_edge> edgesVector;
    edgesVector.reserve(maxNoEdges);
    //reserve space for vertices
    std::vector<Vertex> verticesVector;
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
                if(!findEdge(i, j, edgesVector)) {
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

    Graph outputGraph(verticesVector, edgesVector);

    // TODO - init also vector of vertices ...


    return outputGraph;
}

Graph GraphFunctions::graphFrom_BAfile(std::string path) { //std::ifstream &inputStream

    GraphBAReader baReader(path);
    Graph outputGraph = baReader.read();

    return outputGraph;
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

    Graph outputGraph(outVertices, outEdges);

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

// TODO - not working
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
//    for (int j = 0; j < COLORS; ++j) {
//        int colour = j;
//        vertices.at(firstVertex).core.setColour(colour);
//        // recursive call
//
//    }


    // all starting permutations
    std::vector<std::vector<int>> permutations = {{0,1,2},{0,2,1},{1,0,2},
                                                  {1,2,0},{2,0,1},{2,1,0}};

    Vertex_coloring *vert1 = &vertices.at(firstVertex);
//    Vertex_coloring *vert2 = &vertices.at(1);

    // print edges - temp
//    for(std::pair<Undirected_edge, Undirected_edge> pair1: edges){
//        std::cout<<pair1.second.toString() << "\n";
//    }

    // printing to file
    std::ofstream outputStream("output.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(outputStream.rdbuf()); //redirect std::cout to out.txt!


//    BFScolouriser colouriser;
//    colouriser.list.push_front(firstVertex);
//    bool result = false;
//  result = colouriser.edge3COLBFSrecursive(vertices, edges, *vert1);

    std::cout<<"Start!! \n";

    // coloring
    bool result;
    BFScolouriser colouriser;
    for (int l = 0; l < permutations.size(); ++l) {
        //temp
        std::cout<<"OPTION: " << l << ">>>" << permutations[l][0] << permutations[l][1] << permutations[l][2] << "\n";

        // save to memory input edges and vertices
        // copy of input state
        std::map<int, Vertex_coloring> localVertices(vertices);
        std::map<Undirected_edge, Undirected_edge> localEdges(edges);

        colouriser.list.clear();
        // color edges of first vertex with l-permutation
        int k=0;
        for (int i = 0; i < vert1->core.getNeighbors().size(); ++i) {
            Undirected_edge edge = vert1->core.getEdges().at(i);
            int color = permutations[l].at(i);
            colourEdge(edge, localEdges, localVertices, color);

            // indices of vertices in edge could be swapped
            int to;
            (vert1->core.getId()==edge.from()) ? to=edge.to() : to = edge.from();
            // add to queue
            colouriser.list.push_front(to);

        }

//        colouriser.list.push_front(firstVertex);

        // start coloring of whole graph
        while (!colouriser.list.empty()){
            Vertex_coloring nextVert = localVertices.at(colouriser.list.back());
            result = colouriser.edge3COLBFSrecursive(localVertices, localEdges, nextVert);
            if (result== false){
                break;
            }
        }

        if (result==true) {
            vertices = localVertices;
            edges = localEdges;
            break;
        }
    }


//    colouriser.edge3COLBFSrecursive(vertices, edges, vertices.at(colouriser.queue.front()));




    std::cout<< "QUEUE: " << "(size: " << colouriser.list.size() << ")";
    int size = colouriser.list.size();
    for (int k = 0; k < size; ++k) {
        std::cout<<colouriser.list.front() << ", ";
        colouriser.list.pop_front();
    }
    std::cout<<"\n";

    if (result== false){
        std::cout<<"Graph not colourable!!! \n";
    } else {
        std::cout<<"Success!!! Graph is colourable!! \n";
        // print edges - temp
        for(std::pair<Undirected_edge, Undirected_edge> pair1: edges){
            std::cout<<pair1.second.toString() << "\n";
        }
    }


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


// colors exact vertex and surrounding
bool GraphFunctions::BFScolouriser::edge3COLBFSrecursive(std::map<int, Vertex_coloring> &inVertices, std::map<Undirected_edge, Undirected_edge> &inEdges, Vertex_coloring &vertex) {

//    Vertex_coloring vertex = inVertices.at(0);

    // store input state to memory
    std::list<int> localList(list);


    // availableColors vector contains actual numbers of unused colors for current vertex
    std::vector<int> availableColors;
    // try for every possible permutation of colors ...
    // find available colors for current vertex
    for (int j = 0; j < COLORS; ++j) {
        if (vertex.availableColors.at(j)==1) availableColors.push_back(j);
    }

    int numberPossibleColors = availableColors.size();

    // if has not any not colored edges
    if (numberPossibleColors==0) {
        if (list.empty()) {std::cout<<"DONE!!! \n"; return true;}
        Vertex_coloring nextVert = inVertices.at(list.back());

        // temp
        std::cout<<vertex.core.getId() << ": coloured (" << inEdges.at(vertex.core.getEdges().at(0)).getColour()
                 << inEdges.at(vertex.core.getEdges().at(1)).getColour()
                 << inEdges.at(vertex.core.getEdges().at(2)).getColour()
                 << ")---> next vertex: " << nextVert.core.getId() << "\n";

        return edge3COLBFSrecursive(inVertices, inEdges, nextVert);
    }

    for (int i = 0; i < numberPossibleColors; ++i) {
        // copy of input state
        std::map<int, Vertex_coloring> localVertices(inVertices);
        std::map<Undirected_edge, Undirected_edge> localEdges(inEdges);

        int colour;
        // temp
        int orderOfEdge = 0;
        bool coloured;
        // colour edges of vertex
        // for every edge of vertex
//        if(vertex.core.getId()==77){
//            std::cout<<"id:77 \n";
//        }
        // TODO - if ... info about colored edges also in vertexMap - ?? better? maybe later
        for(Undirected_edge edge: vertex.core.getEdges()){
            // if edge not already colored
            if (!localEdges.at(edge).coloured()){
                // choose color
                colour=availableColors.at((i+orderOfEdge)%numberPossibleColors); // +i ...

                // color edge
                coloured = colourEdge(edge, localEdges, localVertices, colour);
                // indices of vertices in edge could be swapped
                int to;
                (vertex.core.getId()==edge.from()) ? to=edge.to() : to = edge.from();
                // move to another colour
                // add to queue

                if (coloured) {
                    if (!listContains(list, to)) list.push_front(to);
                }
                else {
                    break;
//                    std::cout<<vertex.core.getId()<< ": not coloured ---> GO BACK!!!" << "\n";
//                    return false;

                }
                orderOfEdge++;
            }
        }
        // if some edge couldnt be colored -> go to another option of coloring
        if (!coloured){
            // or if last option - return
            if (i+1==numberPossibleColors) {
                // temp
                std::cout<<vertex.core.getId()<< ": not coloured ---> GO BACK!!!" << "\n";
                list=localList;return false;}
            list=localList;
            continue;
        }

        list.pop_back();
        if (list.empty()) {
            inEdges= localEdges;
            inVertices = localVertices;

            // temp
            std::cout<<"DONE!!! \n";
            return true;
        }
        Vertex_coloring nextVertex = localVertices.at(list.back());

        // temp
        std::cout<<vertex.core.getId() << ": coloured (" << localEdges.at(vertex.core.getEdges().at(0)).getColour()
                << localEdges.at(vertex.core.getEdges().at(1)).getColour()
                << localEdges.at(vertex.core.getEdges().at(2)).getColour()
                 << ")---> next vertex: " << nextVertex.core.getId() << "\n";

        // go to the next vertex in queue/list
        bool next = edge3COLBFSrecursive(localVertices, localEdges, nextVertex);
        if(next){
            // if branch succesfully colored
            // copy output state to global-input
            inEdges= localEdges;
            inVertices = localVertices;
            return true;
        } else {
            // try another option of coloring of edges of vertex
            // return to input state and try another option
            list=localList;

//            return false;
        }


    }

    // temp
    std::cout<<vertex.core.getId()<< ": not coloured ---> GO BACK!!!" << "\n";
    return false;


}

bool GraphFunctions::colourVertex(std::map<int, Vertex> &vertices, Vertex &vertex, int colour) {
    std::vector<int> neighbors(vertex.getNeighbors());

    for(int neighbor: neighbors){
        if(vertices.at(neighbor).getColour()==colour){
            return false;
        }
    }


}

// TODO - delete
//bool GraphFunctions::colourEdgesOfVertex(Vertex_coloring &vertex, std::map<Undirected_edge, Undirected_edge> &edges, std::map<int, Vertex_coloring> &vertices) {
//
//    // for every edge of vertex
//    for(Undirected_edge edge: vertex.core.getEdges()){
////        Undirected_edge tempEdge = edges.at(edge);
//        int to;
//        // if edge not already colored
//        if (!edges.at(edge).coloured()){
//            (vertex.core.getId()==edge.from()) ? to=edge.to() : to = edge.from();
//            Vertex_coloring *toVert = &vertices.at(to);
//
//            // find intersection of available colors for both vertices
//            std::sort(vertex.availableColors.begin(), vertex.availableColors.end());
//            std::sort(toVert->availableColors.begin(), toVert->availableColors.end());
//            std::vector<int> result(COLORS);
//            std::set_intersection(vertex.availableColors.begin(), vertex.availableColors.end(), toVert->availableColors.begin(), toVert->availableColors.end(), result.begin());
//
//            // if found mutual available color
//            if (!result.empty() && result.at(0)!=-1) {
//                edges.at(edge).setColour(result[0]);
//                // remove from available colors recently used color
//                for (int i = 0; i < COLORS; ++i) {
//                    if (vertex.availableColors.at(i)==result[0]) vertex.availableColors.at(i)=-1;
//                    if (toVert->availableColors.at(i)==result[0]) toVert->availableColors.at(i)=-1;
//                }
////                toVert->availableColors.
//
//            } else return false;
//
//        }
//    }
//}

// private
bool GraphFunctions::colourEdge(Undirected_edge &edge, std::map<Undirected_edge, Undirected_edge> &edges,
                                std::map<int, GraphFunctions::Vertex_coloring> &vertices, int colour) {

    Vertex_coloring *fromVertex = &vertices.at(edge.from());
    Vertex_coloring *toVertex = &vertices.at(edge.to());

    int to;
//    (vertex.core.getId()==edge.from()) ? to=edge.to() : to = edge.from();
//    Vertex_coloring *toVert = &inVertices.at(to);

    // find intersection of available colors for both vertices
    std::vector<int> fromVertAvailable;
    std::vector<int> toVertAvailable;
    for (int i = 0; i < COLORS; ++i) {
        if (fromVertex->availableColors.at(i)==1) fromVertAvailable.push_back(i);
        if (toVertex->availableColors.at(i)==1) toVertAvailable.push_back(i);
    }

    std::vector<int> result = {-1, -1, -1};
    std::set_intersection(fromVertAvailable.begin(), fromVertAvailable.end(), toVertAvailable.begin(), toVertAvailable.end(), result.begin());

    // if found mutual available color
    if (!result.empty() && result.at(0)!=-1) {
        // check if there is wanted colour in result vector
        bool wanted = false;
        int indexOfWanted = 0;
        for (indexOfWanted; indexOfWanted < result.size(); ++indexOfWanted) {
            if (result[indexOfWanted]==colour){
                wanted = true;
                break;
            }
        }
        if (!wanted)
            return false;

        edges.at(edge).setColour(result[indexOfWanted]);
        // set recently used color as unavailable
        fromVertex->availableColors.at(result[indexOfWanted])=-1;
        toVertex->availableColors.at(result[indexOfWanted])=-1;

//        for (int i = 0; i < COLORS; ++i) {
//            if (fromVertex->availableColors.at(i)==result[indexOfWanted]) fromVertex->availableColors.at(result[indexOfWanted])=-1;
//            if (toVertex->availableColors.at(i)==result[indexOfWanted]) toVertex->availableColors.at(result[indexOfWanted])=-1; break;
//        }

    } else return false;


    return true;
}

bool GraphFunctions::BFScolouriser::listContains(std::list<int> &list, int number) {
    for(int num : list){
        if(num==number) return true;
    }
    return false;
}


void GraphFunctions::testBFS(Graph inputGraph) {

}




// TODO - tests and removings

/*



OldGraph OldGraph::removeVertex(int vertexToRemove){
    short int *newGraphMatr = new short int[(graphSize)*(graphSize)];

    int i, j, offset;
    // undo graphColors
    for (i = 0; i < graphSize; i++) {
        for (j = 0; j < graphSize; j++) {
            offset = i * graphSize + j;
            if(i==vertexToRemove){
                newGraphMatr[offset] = 0;
            }else if(j==vertexToRemove){
                newGraphMatr[offset] = 0;
            }else{
                newGraphMatr[offset] = graph[offset];
            }

        }
    }

    OldGraph graphWV = OldGraph(newGraphMatr, this->graphSize);

    delete[] newGraphMatr;
    return graphWV;
}


//TODO write method to return array of graphs - every pair of adjacent vertices are missing

std::vector<OldGraph> OldGraph::removeEveryAdjacentPair() {

    std::vector<OldGraph> arrayOfGraphs;
    //arrayOfGraphs.reserve(2);
    int i, j, offset;

    for(i=0;i<graphSize;i++){
        for(j=0;j<graphSize;j++){
            offset = i*graphSize+j;
            if(graph[offset]==1){
                OldGraph gr = this->removeVertex(i);
                arrayOfGraphs.push_back(gr.removeVertex(j));
            }
        }
    }

    //OldGraph gr1 = this->removeVertex(0);
    //std::cout<<gr1.getGraphSize()<<"\n";

    //arrayOfGraphs.push_back(this->removeVertex(0));
    //arrayOfGraphs.push_back(this->removeVertex(1));

    return arrayOfGraphs;
}

std::vector<OldGraph> OldGraph::removeEveryNotAdjacentPair() {

    std::vector<OldGraph> arrayOfGraphs;
    //arrayOfGraphs.reserve(2);
    int i, j, offset;

    for(i=0;i<graphSize;i++){
        for(j=0;j<graphSize;j++){
            offset = i*graphSize+j;
            if(graph[offset]==0&&i!=j){
                OldGraph gr = this->removeVertex(i);
                arrayOfGraphs.push_back(gr.removeVertex(j));
            }
        }
    }

    //OldGraph gr1 = this->removeVertex(0);
    //std::cout<<gr1.getGraphSize()<<"\n";

    return arrayOfGraphs;
}

bool OldGraph::testCriticality() {
    std::vector<OldGraph> graphVector = removeEveryAdjacentPair();
    bool result = true;

    std::vector<bool> results;
    int i;
    for(i=0;i<graphVector.size();i++){
        graphVector[i].DepthFirstSearchColoring(0);
        bool thisIsSnark = graphVector[i].isSnark;
        results.push_back(thisIsSnark);
        if (thisIsSnark) result = false;
    }

    return result;
}

bool OldGraph::testCoCriticality() {
    return false;
}

bool OldGraph::testIreducibility() {
    return false;
}

void OldGraph::doSATSolve() {

    SATSolver *mySolver = new SATSolver();

    char *arg1 = const_cast<char *>("/home/jakub/Git/GraphColor/SATtests/input.cnf");
    char *arg2 = "/home/jakub/Git/GraphColor/SATtests/output.txt";

//    char *arg1 = "input.cnf";
//    char *arg2 = "output.txt";

    char **myArgV;

//     simulating int argc, char* argv
    myArgV[0] = "/home/jakub/Git/GraphColor/cmake-build-debug/GraphColor";
    myArgV[1] = arg1;
    myArgV[2] = arg2;

    int myArgc = 3;


//    std::ifstream inputFile(arg1);
//    char c;
//    inputFile.get(c);
//    std::cout<<c << "\n";
//    std::cout<<inputFile.get() << "\n";


    mySolver->glucoseSimpFromFile(3, myArgV);


}


 */
