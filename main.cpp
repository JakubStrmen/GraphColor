/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: jakub
 *
 * Created on March 5, 2018, 10:10 PM
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "include/OldGraph.h"
#include "include/Filter.h"
#include "include/BreadthFirst.h"
#include "src/Graph.h"
#include "src/GraphFunctions.h"


/*
extern "C" {
    int readGraphFromFile(char *);
    void showGraph(int *);
    void printSomething();
}
*/

//using namespace std;

/*
 * 
 */

// mostly testing functions
void workWithOldGraph(){

    //const char *inputFile =  "/home/jakub/NetBeansProjects/GraphColor/GraphInput/output";

    // TODO - check if path is OK - if there is file
    std::string path = "/home/jakub/Git/GraphColor/GraphInput/";
    //std::string path = "/home/js/CLionProjects/GraphColor/GraphInput/";
    //std::string file = "Generated_graphs.30.05.sn.cyc4.g6";
    //std::string file = "graph_6700_snark_18vert.g6";
    //std::string file = "graphG6.g6";
    //std::string file = "graph_flowerSnarkJ5_20vert.g6";
    std::string file = "graph_1108_snark56Vert.g6";
    std::string inputFile = path + file;

    OldGraph *graph1 = new OldGraph(inputFile, "g6file");
//    graph1->doSATSolve();

    graph1->showGraph();

    //graph1->showGraph();

    /*
    graph1->showColoredGraph();
    graph1->DepthFirstSearchColoring(0);
    graph1->showColoredGraph();

    graph1->showGraph();
    std::vector<OldGraph> output= graph1->removeEveryAdjacentPair();
    std::cout<< output.size()<<"\n";
    //output[0].showGraph();
    std::vector<OldGraph> output2= graph1->removeEveryNotAdjacentPair();
    std::cout<< output2.size()<<"\n";

     */

//    graph1->showGraph();
//    graph1->DepthFirstSearchColoring(0);



    //bool critical = graph1->testCriticality();
    //std::cout<< "graph is critical: " << critical << "\n";

    //graph1->BreadthFirstSearchColoring(0);


    //graph1->BreadthFirstSearchColoring(0);

    //output[1].showGraph();
    //output[2].showGraph();
    //std::cout<<output[0].getGraphSize()<<"\n";

    //graph2.showGraph();
    //graph2.DepthFirstSearchColoring(0);
    //graph1->DepthFirstSearchColoring(0);




    std::cout<<"Hello!" << std::endl;

//    delete graph1;

    //std::cout<< sizeof(OldGraph)<<"\n";

}

int main(int argc, char** argv) {

    std::string path = "/home/jakub/Git/GraphColor/GraphInput/";
    std::string file = "graph_6700_snark_18vert.g6";
    std::string inputFile = path + file;


//    workWithOldGraph();
//    std::vector<int> graph = {0};
//    GraphFunctions::readGraphFromG6File(inputFile, graph);

    std::string line;
    std::ifstream myfile;

    myfile.open(inputFile);
    // read line from input file
    std::getline(myfile, line);
    myfile.close();


    Graph myGraph(line);

//    myGraph.printEdges();

    Graph newGraph = GraphFunctions::reduceEdge3Col_Vert3Col(myGraph);

//    myGraph.printEdges();
    newGraph.printEdges();
    newGraph.printVertices();
//    myGraph.printVertices();


//    std::cout<<myGraph.ve





//    OldGraph graph2(inputFile, "g6file");
//    graph2.writeGraphToFile("graph_6700_snakr_adjMatrix");



//    std::for_each()


    return 0;
}

