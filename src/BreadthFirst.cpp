//
// Created by js on 4/3/18.
//

#include "../include/BreadthFirst.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

#define COLORS 3

BreadthFirst::BreadthFirst(short int *initGraph, int getGraphSize){

    std::cout<< "New Breadthfirst! " << std::endl;

    graphSize = getGraphSize;
    //allocate new matrix size of graph
    usedColors = new short int[graphSize * COLORS];
    graphColors = new short int[graphSize * graphSize];

    short int i;
    int j, offset;

    // create matrix of used colors

    for (i = 0; i < graphSize; i++) {
        for (j = 0; j < COLORS; j++) {
            offset = i * graphSize + j;
            usedColors[offset] = 0;
        }
    }
    graph = new short int[graphSize * graphSize];
    // undo graphColors
    for (i = 0; i < graphSize; i++) {
        for (j = 0; j < graphSize; j++) {
            offset = i * graphSize + j;
            graph[offset] = initGraph[offset];
            graphColors[offset] = initGraph[offset];
        }
    }
    //compute matrix of all permutations of edge colors of one vertex
    allPermutationsCount = 1;
    for(i=1;i<=COLORS;i++){
        allPermutationsCount*=i;
    }

    myColors[0] = 2; myColors[1] = 3; myColors[2] = 4;


}

BreadthFirst::BreadthFirst(const BreadthFirst &orig) {

}

BreadthFirst::~BreadthFirst() {

}


std::vector<short int> BreadthFirst::findUncoloredEdges(short int vertex) {

    std::vector<short int> result;
    short int i;
    int j, offset;
    for(i=0;i<graphSize;i++){
        offset = vertex*graphSize + i;
        if(graph[offset]==1){
            result.push_back(i);
        }
    }

    return result;
}

// TODO - color inserted check
void BreadthFirst::colorEdge(short fromVertex, short toVertex, short color) {

    if(color<2 || color>4) {
        std::cout<< "Wrong color number insterted!! \n";
        return;
    }
    graph[fromVertex*graphSize + toVertex]= color;
    graph[toVertex*graphSize + fromVertex]= color;
    usedColors[fromVertex*COLORS+color] = 1;
    usedColors[toVertex*COLORS+color]=1;
}

void BreadthFirst::showUsedColors() {
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
        for(j=0; j<COLORS;j++){
            int offset = i * graphSize + j;
            printf("%hd ", usedColors[offset]);
            if(j>9){
                printf(" ");
            }

        }
        printf("\n");

    }

}

int BreadthFirst::findColoringRecursive(short int) {
    return 0;
}

int BreadthFirst::findColoring(short int) {

    int i,j, offset;

    findUncoloredEdges(0);
    showUsedColors();
    showGraph();

    for (i = 0; i < graphSize; i++) {
        for (j = 0; j < COLORS; j++) {
            offset = i * graphSize + j;
            usedColors[offset] = 0;
        }
    }

    showUsedColors();
    showGraph();

    return 0;
}
