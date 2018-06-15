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
#include "include/Graph.h"
#include "include/Filter.h"
#include "include/BreadthFirst.h"






//using namespace std;

/*
 * 
 */

/*
extern "C" {
    int readGraphFromFile(char *);
    void showGraph(int *);
    void printSomething();
}
*/

int main(int argc, char** argv) {

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

    /*
    Filter *filter1 = new Filter(inputFile);
    
    filter1->startWork();
    delete filter1;
     
     */


    std::cout<<argc<<"\n";
    for(int i=0; i<argc; i++){
        std::cout<< i << ": " << argv[i] <<"\n";
    }

    Graph *graph1 = new Graph(inputFile, "g6file");

    graph1->doSATSolve();


    //graph1->showGraph();

    /*
    graph1->showColoredGraph();
    graph1->DepthFirstSearchColoring(0);
    graph1->showColoredGraph();

    graph1->showGraph();
    std::vector<Graph> output= graph1->removeEveryAdjacentPair();
    std::cout<< output.size()<<"\n";
    //output[0].showGraph();
    std::vector<Graph> output2= graph1->removeEveryNotAdjacentPair();
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



    //std::cout<< sizeof(Graph)<<"\n";


    return 0;
}

