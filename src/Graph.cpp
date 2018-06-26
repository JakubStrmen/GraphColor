/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   GraphColor.cpp
 * Author: jakub
 *
 * Created on March 7, 2018, 10:44 AM
 */


#include <chrono>
#include <ctime>
#include <fstream>
#include <bitset>
#include <cstring>
#include <sstream>

#include "../include/Graph.h"
#include "iostream"
#include "cstdlib"
#include "cstdio"
#include "../include/depthFirst.h"
#include "../include/BreadthFirst.h"
#include "SATSolver.h"

#define COLORS 3

Graph::Graph() {

}

/*
Graph::Graph(Graph const &) {

}
*/

/**
 * to create object of Graph using existing graph (short int adjacency matrix of graph)
 */
Graph::Graph(short int *inputGraph, int inputGraphSize){

    graphSize = inputGraphSize;
    //allocate new matrix size of graph
    //graph = (short int *) malloc(graphSize * graphSize * sizeof (short int));

    graph = new short int[graphSize * graphSize];

    int i, j, offset;
    // undo graphColors
    for (i = 0; i < graphSize; i++) {
        for (j = 0; j < graphSize; j++) {
            offset = i * graphSize + j;
            graph[offset] = inputGraph[offset];
        }
    }

}

Graph::Graph(const char *inputFile, const char *format) {
    //if input is name of file containing graph in g6 format
    if(std::strcmp(format, "g6file")==0){
        readGraphFromG6File(inputFile);
    }else
        //if input is name of file containing graph in adjacency matrix
    if(std::strcmp(format, "txt")==0){
        readGraphFromFile(inputFile);
    }else
        //if input is string containing graph in g6 format
    if(std::strcmp(format, "g6string")==0){
        readGraphFromG6String(inputFile);
    }
}

Graph::Graph(std::string inputFile, const char *format) {
    //if input is name of file containing graph in g6 format
    if(std::strcmp(format, "g6file")==0){
        readGraphFromG6File(inputFile);
    }else
        //if input is name of file containing graph in adjacency matrix
    if(std::strcmp(format, "txt")==0){
        const char *cstr = inputFile.c_str();
        readGraphFromFile(cstr);
    }else
        //if input is string containing graph in g6 format
    if(std::strcmp(format, "g6string")==0){
        readGraphFromG6String(inputFile);
    }

}

Graph::~Graph() {
    // std::cout << "item deleted!" << std::endl;
    // free(graph);
}

int Graph::getGraphSize(){
    return graphSize;
}
void Graph::setGraphSize(int size) {
    graphSize = size;
}


int Graph::readGraphFromFile(const char *inputFile){
    //in input file is stored adjacency matrix of graph
    int i, j, offset;
    short int number;
    FILE *input;
    // at first find out how big is graph
    input = fopen(inputFile, "r");
    char c;
    int size = 0;
    c = getc(input);
    while(c!='\n'){  // \n
        if(c==' '){
            c = getc(input);
            continue;
        }
        if(c!='0' && c!='1'){
            c = getc(input);
            continue;
        }
        size++;
        c = getc(input);
    }
    fclose(input);
    graphSize = size;
    // graph = (short int *)malloc(graphSize * graphSize * sizeof(short int));

    graph = new short int [graphSize * graphSize];
    // read file and store values to graph - if are valid
    input = fopen(inputFile, "r");
    i=0; j=0;
    while(!feof(input)){
        c = getc(input);
        if(j==graphSize && c!='\n'){
            continue;
        }
        if(feof(input)){
            if(j!=graphSize){
                return (-1);
            }
            continue;
        }
        if(c=='\n'){
            if(j!=graphSize){
                return (-1);
            }
            if(j==(i+1) && (i+1)==graphSize){
                fclose(input);
                return 0;
            }
            i++;
            j=0;
            continue;
        }
        if(c==' '){
            continue;
        }
        if(c!='0' && c!='1'){
            return (-1);
        }
        if(j==i && c=='1'){
            return (-1);
        }
        offset = i * graphSize + j;
        if(c=='0'){
            number = 0;
        }else{
            number = 1;
        }
        graph[offset] = number;

        j++;

    }

    fclose(input);

    maxDegree = findMaxDegree();

    return(0);
}

void Graph::writeGraphToFile(short int *graphToWrite, int size, const char *outputFile){
    int i, j, offset;
    std::ofstream myfile;
    myfile.open(outputFile);

    for(i=0; i<size;i++){
        for(j=0; j<size;j++){
            int offset = i * size + j;
            myfile << graphToWrite[offset];
        }
        myfile<<"\n";
    }
    myfile.close();
}

/**
 * Method to print default graph of object to stdout
 */
void Graph::showGraph(){
    int i, j;
    //int *myGraph = *graphToShow;
    //iterate our matrix and print values
    printf("-- ");
    for(j=0; j<graphSize;j++){
        printf("%d ", j);
    }
    printf("\n");
    printf("--");
    for(j=0; j<graphSize;j++){
        printf("--");
    }
    printf("\n");
    for(i=0; i<graphSize;i++){
        printf("%d: ", i);
        if(i<10){
            printf(" ");
        }
        for(j=0; j<graphSize;j++){
            int offset = i * graphSize + j;
            printf("%hd ", graph[offset]);
            if(j>9){
                printf(" ");
            }

        }
        printf("\n");

    }
}

/**
 * Method to print 3edgeColored graph of object to stdout
 */
void Graph::showColoredGraph(){
    if(edge3Colorable==false&&isSnark==true){
        std::cout<< "This graph is not 3-edge colorable! \n";
        return;
    }else if(edge3Colorable!=true){
        std::cout<<"Graph hasn't been colored \n";
        return;
    }
    int i, j;
    //iterate our matrix and print values
    printf("-- ");
    for(j=0; j<graphSize;j++){
        printf("%d ", j);
    }
    printf("\n");
    printf("--");
    for(j=0; j<graphSize;j++){
        printf("--");
    }
    printf("\n");
    for(i=0; i<graphSize;i++){
        printf("%d: ", i);
        if(i<10){
            printf(" ");
        }
        for(j=0; j<graphSize;j++){
            int offset = i * graphSize + j;
            printf("%hd ", coloredGraph[offset]);
            if(j>9){
                printf(" ");
            }

        }
        printf("\n");

    }
}

/**
 * Method to print selected graph to stdout
 * @param graphToShow
 * @param size
 */
void Graph::showGraph(short int *graphToShow, int size){
    int i, j;
    //int *myGraph = *graphToShow;
    //iterate our matrix and print values
    printf("-- ");
    for(j=0; j<size;j++){
        printf("%d ", j);
    }
    printf("\n");
    printf("--");
    for(j=0; j<size;j++){
        printf("--");
    }
    printf("\n");
    for(i=0; i<size;i++){
        printf("%d: ", i);
        if(i<10){
            printf(" ");
        }
        for(j=0; j<size;j++){
            int offset = i * size + j;
            printf("%hd ", graphToShow[offset]);
            if(j>9){
                printf(" ");
            }

        }
        printf("\n");

    }
}


void Graph::showMatrix(int *matrixToShow, int rows, int cols){
    int i, j;
    //int *myGraph = *graphToShow;
    //iterate our matrix and print values
    printf("-- ");
    for(j=0; j<cols;j++){
        printf("%d ", j+2);
    }
    printf("\n");
    printf("--");
    for(j=0; j<cols;j++){
        printf("--");
    }
    printf("\n");
    for(i=0; i<rows;i++){
        printf("%d: ", i);
        for(j=0; j<cols;j++){
            int offset = i * cols + j;
            printf("%d ", matrixToShow[offset]);

        }
        printf("\n");

    }
}

int Graph::findMaxDegree(){
    //iterate our matrix and find maximal degree of vertex in graph
    int i, j, offset;
    int max = 0;
    int temp = 0;
    for(i=0; i<graphSize;i++){
        for(j=0; j<graphSize;j++){
            offset = i * graphSize + j;
            temp += graph[offset];
        }
        if(temp>max){
            max = temp;
        }
        temp = 0;

    }
    return max;
}

int Graph::DepthFirstSearchColoring(int startVertex){
    //for time measurement
    auto start = std::chrono::high_resolution_clock::now();

    RESULTFROMCOLORING result = depthFirstSearch(graphSize, graph, startVertex);

    //showGraph(result.outputGraph, graphSize);

    if(result.result = 0){
        edge3Colorable = false;
        isSnark = true;
    } else if(result.result == 1) {
        edge3Colorable=true;
        isSnark = false;
        coloredGraph = new short int [graphSize*graphSize];
        coloredGraph = result.outputGraph;

        }

    //showGraph(coloredGraph, graphSize);

    //for time measurement - end -> print
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed3 = end - start;
    std::cout << "Duration depthFirstSearch: " << elapsed3.count() << std::endl;



    return 0;

}

int Graph::BreadthFirstSearchColoring(short int startVertex){


    // TODO - rewrite BreadthFirst ...

    BreadthFirst *br2 = new BreadthFirst(graph, this->getGraphSize());

    //br2->findUncoloredEdges(0);
    //br2->showGraph();

    //br2->findColoring(0);

    delete(br2);


    std::cout<< "Destructor Done! " << std::endl;

    return 0;
}


/**
 * Method from Nauty&Traces - showg.c
 * @param s
 * @return
 */
int Graph::getGraphSizeG6(const char *s){

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

void Graph::readGraphFromG6File(std::string inputFile){

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

    // convert string to char*
    const char *cstr = line.c_str();
    int size = getGraphSizeG6(cstr);
    graphSize = size;


    // check size of graph and remove first few bytes from line according to size
    if(size >= 0 && size < 63) line.erase(0, 1);
    else if(size >= 63 && size < 258048) line.erase(0, 4);
    else if (size >= 258048 && size < 68719476735) line.erase(0, 8);
    else {
        std::cout<<"We have problem!! too large graph or something else!!! \n";
        //return -1;
    }

    // convert line of characters to binary form (line of 0 and 1)
    std::string graphInLine = "";
    for(char& c : line){
        int value = c - BIAS6;
        std::string binary = std::bitset<6>(value).to_string();
        graphInLine += binary;
    }

    // store values from binary line form to graph matrix
    graph = new short int [size*size];
    int i, j, offset, offset2;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            offset = i * size +j;
            graph[offset]=0;
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
        graph[offset]=bit;
        // offset2 - for lower triangle
        graph[offset2]=bit;

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

    maxDegree = findMaxDegree();

    //const char *outputFile = "/home/jakub/NetBeansProjects/GraphColor/graphOutput.txt";
    //writeGraphToFile(graph, size, outputFile);
}

void Graph::readGraphFromG6String(std::string inputString){
    // convert string to char*

    const char *cstr = inputString.c_str();
    int size = getGraphSizeG6(cstr);
    graphSize = size;


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
    graph = new short int [size*size];
    int i, j, offset, offset2;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            offset = i * size +j;
            graph[offset]=0;
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
        graph[offset]=bit;
        // offset2 - for lower triangle
        graph[offset2]=bit;

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

    maxDegree = findMaxDegree();

}

Graph Graph::removeVertex(int vertexToRemove){
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

    Graph graphWV = Graph(newGraphMatr, this->graphSize);

    delete[] newGraphMatr;
    return graphWV;
}


//TODO write method to return array of graphs - every pair of adjacent vertices are missing

std::vector<Graph> Graph::removeEveryAdjacentPair() {

    std::vector<Graph> arrayOfGraphs;
    //arrayOfGraphs.reserve(2);
    int i, j, offset;

    for(i=0;i<graphSize;i++){
        for(j=0;j<graphSize;j++){
            offset = i*graphSize+j;
            if(graph[offset]==1){
                Graph gr = this->removeVertex(i);
                arrayOfGraphs.push_back(gr.removeVertex(j));
            }
        }
    }

    //Graph gr1 = this->removeVertex(0);
    //std::cout<<gr1.getGraphSize()<<"\n";

    //arrayOfGraphs.push_back(this->removeVertex(0));
    //arrayOfGraphs.push_back(this->removeVertex(1));

    return arrayOfGraphs;
}

std::vector<Graph> Graph::removeEveryNotAdjacentPair() {

    std::vector<Graph> arrayOfGraphs;
    //arrayOfGraphs.reserve(2);
    int i, j, offset;

    for(i=0;i<graphSize;i++){
        for(j=0;j<graphSize;j++){
            offset = i*graphSize+j;
            if(graph[offset]==0&&i!=j){
                Graph gr = this->removeVertex(i);
                arrayOfGraphs.push_back(gr.removeVertex(j));
            }
        }
    }

    //Graph gr1 = this->removeVertex(0);
    //std::cout<<gr1.getGraphSize()<<"\n";

    return arrayOfGraphs;
}

bool Graph::testCriticality() {
    std::vector<Graph> graphVector = removeEveryAdjacentPair();
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

bool Graph::testCoCriticality() {
    return false;
}

bool Graph::testIreducibility() {
    return false;
}

void Graph::doSATSolve() {

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


    mySolver->glucoseSimp(3, myArgV);


}


void Graph::test() {


}

// TODO - test criticality ... etc methods


