/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//
// Created by jakub on 3/24/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>
#include <sys/types.h>
#include <unistd.h>
#include "mystack.h"
#include "depthFirst.h"

//#include "DepthFirst.h"

#define COLORS 3
#define CHUNK 1

/**
 * Define global variables
 */
short int *graph;
int graphSize;
short int *graphColors;
short int *usedColors;
short int *allPermutations;
int permutationsCount;
int allPermutationsCount;
int row;
int count = 0;
// int countOfReturn1 = 0;
int countOfFree = 0;
int countOfMalloc = 0;

RESULTFROMCOLORING depthFirstSearch(int sizeOfGraph, short int *graphToColor, int firstVertex){
    graphSize = sizeOfGraph;

    int i, j, offset;
    //allocate new matrix size of graph
    graphColors = (short int *) malloc(graphSize * graphSize * sizeof(short int));
    //copy matrix of graph to new matrix and to local matrix
    for (i = 0; i < graphSize; i++) {
        for (j = 0; j < graphSize; j++) {
            offset = i * graphSize + j;
            graphColors[offset] = graphToColor[offset];
        }
    }



    RESULTFROMCOLORING result;
    result.result = findColoringSequential(firstVertex);
    result.outputGraph = graphColors;

    printf("CountOfMalloc: %d, CounOfFree: %d \n", countOfMalloc, countOfFree);

    return result;

}

void permute(short int *array,int i,int length) {
    int j, offset;
    if (length == i){
        for(j=0;j<length;j++){
            offset=row*COLORS+j;
            allPermutations[offset]=array[j];
        }
        row++;
        return;
    }
    for (j = i; j < length; j++) {
        short int temp;
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
        permute(array,i+1,length);
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
    // return;
}

/**
* method to undo coloring steps in call of function on specific vertex
*/
void undo(short int *myRow, int vertex, short int *myGraphColors, short int *myUsedColors){
    int i, offset, fromVertex, toVertex;
    // in loop find difference - made changes
    for(i=0;i<graphSize;i++){
        offset = vertex*graphSize+i;
        fromVertex = vertex*COLORS;
        toVertex = i*COLORS;
        // if difference found - undo changes
        if(myGraphColors[offset]!=myRow[i]){
            // set used color as unused
            myUsedColors[fromVertex+myGraphColors[offset]-2]=0;
            myUsedColors[toVertex+myGraphColors[offset]-2]=0;
            // set actual colors of edges as before changes in actual function call
            myGraphColors[offset]=myRow[i];
            myGraphColors[i*graphSize+vertex]=myRow[i];
        }
    }
}
/**
 * method to reset whole graph to input state after trying to color specific vertex
 * */
void undoGraph(short int *initGraph, short int *initColors, short int *myGraphColors, short int *myUsedColors){
    int i, j, offset, fromVertex, toVertex;
    // undo graphColors
    for(i=0; i<graphSize;i++){
        for(j=0; j<graphSize;j++){
            offset = i * graphSize + j;
            myGraphColors[offset] = initGraph[offset];
        }
    }
    // undo usedColors
    for(i=0; i<graphSize;i++){
        for(j=0; j<COLORS;j++){
            offset = i * COLORS + j;
            myUsedColors[offset] = initColors[offset];
        }
    }

}


/**
* Method to find regular n-coloring of graph
*/
int findColoring(int firstNode){

    // initialize new variables
    bool flag = false;
    int i, j, k, offset, tid, nthreads, chunk;
    int finderId = -1;
    int conflict = 0;
    int actualVertex = firstNode;
    int found = 0;
    short int *myRow; // new array to store input state of starting row of graph

    myRow = (short int *)malloc(graphSize * sizeof(short int));


    // matrix of permutations of coloring edges of first vertex ---- later
    allPermutationsCount = 1;
    short int myColors[COLORS];
    // compute permutations count
    for(i=1;i<=COLORS;i++){
        allPermutationsCount*=i;
    }
    //compute permutations count for all other vertexes - unused
    permutationsCount=1;
    for(i=1;i<=COLORS-1;i++){
        permutationsCount*=i;
    }
    //fill temporary array with our available colors
    for(i=1;i<=COLORS;i++){
        myColors[i-1]= (short) (i + 1);
    }
    allPermutations = (short int *)malloc(allPermutationsCount * COLORS * sizeof(short int));
    // precompute all permutations of colors - store to allPermutations
    row=0;
    permute(myColors, 0, COLORS);
    row=0;

    // copy input setting of actualVertex --- for later undo
    for(i=0;i<graphSize;i++){
        offset = actualVertex*graphSize+i;
        myRow[i] = graphColors[offset];
    }

#if defined (_OPENMP)
    chunk = CHUNK;
#endif

    /**
     * Here is coloring!!
    * do in parallel
    */
#pragma omp parallel shared(allPermutationsCount, chunk, flag, finderId, count, allPermutations, found, countOfFree, countOfMalloc)  private(i,j,k,tid, offset, conflict)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d starting...\n",tid);

        short int *myGraphColors;
        myGraphColors = (short int *)malloc(graphSize * graphSize * sizeof(short int));
        int actColor = 0;
        int addedVertexes=0;
        int myColor, offset2, fromVertex, toVertex;
        //showGraph(graph);
        MYSTACK stack1;
        ini(&stack1);
        //copy graph to local matrix
        for(j=0; j<graphSize;j++){
            for(k=0; k<graphSize;k++){
                offset = j * graphSize + k;
                myGraphColors[offset] = graphColors[offset];
            }
        }

        //create matrix with color stamps for each row and color - init with zeros
        short int *usedColors;
        usedColors = (short int *)malloc(graphSize * COLORS * sizeof(short int));
        //fill matrix with init values - zeros
        for(i=0; i<graphSize;i++){
            for(j=0; j<COLORS;j++){
                offset = i * COLORS + j;
                usedColors[offset] = 0;
            }
        }

        //showGraph(myGraphColors);

        // iteracie cyklu su dynamicky planovane na vlakna. kazde vlakno vykona chunk iteracii naraz
#pragma omp for schedule(static, chunk)
        for (i = 0; i < allPermutationsCount; i++)
        {
            //printf("Node no.: %d Thread %d starting permutation %d ... start in vertex: %d \n", nodeNo, tid, i, actualVertex);
            if(flag) continue;
            // copy graphColors to our private matrix

            offset2 = COLORS * i;
            // in loop - color edges of first vertex
            actColor = 0;
            //find edge
            for(j=0;j<graphSize;j++){
                offset = actualVertex*graphSize+j;
                //check if there is edge between actual Vertex and other vertex
                if(myGraphColors[offset]==1){
                    //if it is, continue with checking color conflict
                    //indexes of rows of vertex in matrix with colors
                    fromVertex = actualVertex*COLORS;
                    toVertex = j*COLORS;
                    myColor = allPermutations[offset2+actColor];
                    // color found edge with next color
                    myGraphColors[offset]= myColor;
                    myGraphColors[j*graphSize+actualVertex]=myColor;
                    usedColors[fromVertex+myColor-2]=1;
                    usedColors[toVertex+myColor-2]=1;
                    actColor++;
                    // add vertex to stack
                    if(findInStack(stack1, j)==0){
                        push(&stack1, j);
                    }
                    addedVertexes++;
                }
            }

            int result;
            int finalResult=0;
            //start coloring from vertices in stack
            for(j=0;j<addedVertexes;j++){
                // try to find coloring with next vertex from stack
                result = findColoringRecursive(front(stack1), actualVertex, myGraphColors, usedColors);
                if(result == 0){
                    finalResult=0;
                    break;
                    // for now END - return 0 - couldnt find coloring
                }else{finalResult=1;}
                // remove vertex from stack
                pop(&stack1);
                // TODO --- do while stack is not empty loop ...
            }
            /**
             * if coloring from all vertices in stack returned 1 - finalResult stays 1
             * */
            addedVertexes=0;
            if(finalResult == 1){
                found = 1;
                printf("-------------- We found something!! ---------------- \n");
                flag = true;
                finderId = tid;

                // copy myGraphColor to graphColor
                for (k = 0; k < graphSize; k++) {
                    for (j = 0; j < graphSize; j++) {
                        offset = k * graphSize + j;
                        graphColors[offset] = myGraphColors[offset];
                    }
                }

                continue;
            }
            if(i==allPermutationsCount){
                printf("There is no other permutation!! --- undo completely \n");

            }else{
                printf("Try another permutation!! --- undo completely before \n");
            }
            undo(myRow, firstNode, myGraphColors, usedColors);
            emptyStack(&stack1);
        }

        free(myGraphColors);
        free(usedColors);
    }
    /**
     * If tried all permutations of coloring starting vertex and found = 0
     * */
    if(found == 0){
        printf("................. We found NOTHING :-\\ !! ...................... \n");
    }else{
        free(allPermutations);
        free(myRow);
        printf("...\nSUM of function4 call: %d \n", count);
        return 1;
    }
    printf("...\nSUM of function4 call: %d \n", count);

    free(allPermutations);
    free(myRow);
    return 0;
}

/**
* method for recursive calls to color edges of vertex
*/
int findColoringRecursive(int vertex, int parent, short int *myGraphColors, short int *myUsedColors){
    count++;

    int conflict = 0;
    int actualVertex = vertex;
    int actColor = 0;
    int fromVertex, toVertex, i, j, k, offset, offset2, temp, edgeNO, found, foundEdge;
    int result = -1;
    int nextVertex;
    int usedColor = -1; //if COLORS = 3 - else array needed

    // new array to store input state of graphColors
    short int *initUsedColors, *initGraphColors;

    /**
     * store initial graphColors and usedColors to local matrix - for later restore
     * */
    //allocate new matrix size of graph
    countOfMalloc++;
    initGraphColors = (short int *)malloc(graphSize * graphSize * sizeof(short int));
    //copy matrix of graph to new matrix
    for(i=0; i<graphSize;i++){
        for(j=0; j<graphSize;j++){
            offset = i * graphSize + j;
            initGraphColors[offset] = myGraphColors[offset];
        }
    }
    //create matrix with color stamps for each row and color - init with zeros
    initUsedColors = (short int *)malloc(graphSize * COLORS * sizeof(short int));
    //fill matrix with init values - zeros
    for(i=0; i<graphSize;i++){
        for(j=0; j<COLORS;j++){
            offset = i * COLORS + j;
            initUsedColors[offset] = myUsedColors[offset];
        }
    }


    foundEdge = 0;
    //find edge
    for(i=0;i<graphSize;i++){   //TODO could be while(not found 3 edges) ... ...
        offset = actualVertex*graphSize+i;
        found = 0;
        //check if there is edge (not colored) between actual Vertex and other vertex
        if(myGraphColors[offset]==1){
            foundEdge = 1;
            conflict = 1;
            fromVertex = actualVertex*COLORS;
            toVertex = i*COLORS;

            // find available color
            for(j=0;j<COLORS;j++){
                offset2 = fromVertex+j; //position in myUsedColors
                if(myUsedColors[offset2]==0){
                    if(myUsedColors[toVertex+j]==0){
                        //if there isnt conflict, color edge with actual color
                        // color edge, unavailable this color
                        actColor=j;
                        myGraphColors[offset]= (short) (actColor + 2);
                        myGraphColors[i*graphSize+actualVertex]= (short) (actColor + 2);
                        myUsedColors[fromVertex+actColor]=1;
                        myUsedColors[toVertex+actColor]=1;
                        // set conflict mark to = 0
                        conflict = 0;
                        found = 1;
                        usedColor = j;
                        //We found color from actualVertex to i color: j+2
                        break;

                    }
                }
            }

            if(found == 1){
                result = findColoringRecursive(i, vertex, myGraphColors, myUsedColors);
            }else{ // if did not find available color
                undoGraph(initGraphColors, initUsedColors, myGraphColors, myUsedColors);

                countOfFree++;
                free(initGraphColors);
                free(initUsedColors);
                return 0;
            }

            // TODO-------- should be in loop
            // ---- Attempt no. 2  ------

            // if result = 0 -> try with next color
            if(result == 0){
                //COULDNT find coloring of edge with first color
                // UNDO
                myGraphColors[offset]=1;
                myGraphColors[i*graphSize+actualVertex]=1;
                myUsedColors[fromVertex+actColor]=0;
                myUsedColors[toVertex+actColor]=0;
                found = 0;
                // try next color

                //find next available color
                for(j=0;j<COLORS;j++){
                    if(j!=usedColor){
                        offset2 = fromVertex+j;
                        if(myUsedColors[offset2]==0){
                            if(myUsedColors[toVertex+j]==0){
                                //if there isnt conflict, color edge with actual color
                                // color edge, unavailable this color
                                // add vertex to stack - if not in stack
                                actColor=j;
                                myGraphColors[offset]= (short) (actColor + 2);
                                myGraphColors[i*graphSize+actualVertex]= (short) (actColor + 2);
                                myUsedColors[fromVertex+actColor]=1;
                                myUsedColors[toVertex+actColor]=1;
                                // set conflict mark to = 0
                                conflict = 0;
                                //actColor=0;
                                found = 1;
                                usedColor = j+2;
                                //printf("We found color fromVertex: %d toVertex: %d j: %d \n", actualVertex, i, j+2);
                                break;
                            }
                        }
                    }

                }
                //try to color again
                if(found == 1){
                    result = findColoringRecursive(i, parent, myGraphColors, myUsedColors);
                }
                // if still cant color graph - return - it has no sense to continue
                if(result == 0){
                    //COULDNT find coloring of edge with any color

                    undoGraph(initGraphColors, initUsedColors, myGraphColors, myUsedColors);
                    countOfFree++;
                    free(initGraphColors);
                    free(initUsedColors);
                    return 0;
                }
                //else continue with other edges
            }
        }

    }
    if(foundEdge==0){
        //printf("ACTUAL VERTEX: %d has not any uncolored edge ---- return to VERTEX: %d RETURN 1 \n", actualVertex, parent);
        countOfFree++;
        free(initGraphColors);
        free(initUsedColors);
        return 1;
    }
    if(result == 1){
        countOfFree++;
        free(initGraphColors);
        free(initUsedColors);
        //printf("ACTUAL VERTEX: %d WE found any edge and coloring was succesful ... --- return to VERTEX: %d RETURN 1 \n", actualVertex, parent);
        return 1;
    }

    //printf("Here!! \n");
    countOfFree++;
    free(initGraphColors);
    free(initUsedColors);
    //free(myUsedColors);
    //free(myGraphColors);
    return 0;
}

int findColoringSequential(int firstNode){

    // initialize new variables
    short int i, j; int offset, fromVertex, toVertex, offset2;
    short int myColor;
    int conflict = 0;
    int actualVertex = firstNode;
    int actColor = 0;
    int numberOfPermutations = 1;
    int addedVertexes=0;
    int found = 0;
    short int *myRow; // new array to store input state of graphColors
    myRow = (short int *)malloc(graphSize * sizeof(short int));

    // create and initiate stack
    MYSTACK stack1;
    ini(&stack1);


    //create matrix with color stamps for each row and color - init with zeros
    usedColors = (short int *)malloc(graphSize * COLORS * sizeof(short int));
    //fill matrix with init values - zeros
    for(i=0; i<graphSize;i++){
        for(j=0; j<COLORS;j++){
            offset = i * COLORS + j;
            usedColors[offset] = 0;
        }
    }
    // matrix of permutations of coloring edges of first vertex ---- later
    allPermutationsCount = 1;
    short int myColors[COLORS];
    // compute permutations count
    for(i=1;i<=COLORS;i++){
        allPermutationsCount*=i;
    }
    //compute permutations count for all other vertexes - unused
    permutationsCount=1;
    for(i=1;i<=COLORS-1;i++){
        permutationsCount*=i;
    }
    //fill temporary array with our available colors
    for(i=1;i<=COLORS;i++){
        myColors[i-1]= (short) (i + 1);
    }
    allPermutations = (short int *)malloc(allPermutationsCount * COLORS * sizeof(short int));
    // precompute all permutations of colors - store to allPermutations
    row=0;
    permute(myColors, 0, COLORS);
    row=0;

    // copy input setting of actualVertex --- for later undo
    for(i=0;i<graphSize;i++){
        offset = actualVertex*graphSize+i;
        myRow[i] = graphColors[offset];
    }
    // showGraph2(allPermutations, allPermutationsCount, COLORS);

    /**
    * Here is coloring!
    */

    // at first - do it as loop with all permutations
    //printf("Begin!!! ------------ initial first vertex colors \n");
    for(i=0; i<allPermutationsCount; i++){ // to:
        offset2 = COLORS * i;
        // in loop - color edges of first vertex
        // find edge
        /*
printf("initial colors: ");
for(j=0;j<COLORS;j++){
    printf("%d ", allPermutations[offset2+j]);
}
printf("\n");
*/
        actColor = 0;
        for(j=0;j<graphSize;j++){
            offset = actualVertex*graphSize+j;

            //check if there is edge between actual Vertex and other vertex
            if(graphColors[offset]==1){

                //if it is, continue with checking color conflict
                //indexes of rows of vertex in matrix with colors
                fromVertex = actualVertex*COLORS;
                toVertex = j*COLORS;
                myColor = allPermutations[offset2+actColor];
                // color found edge with next color
                graphColors[offset]= myColor;
                graphColors[j*graphSize+actualVertex]=myColor;
                usedColors[fromVertex+myColor-2]=1;
                usedColors[toVertex+myColor-2]=1;
                actColor++;
                // add vertex to stack
                if(findInStack(stack1, j)==0){
                    push(&stack1, j);
                }
                addedVertexes++;
            }
        }
        //showStack(stack1);
        //showGraph(graphColors);
        //printf("------------------------ Start with STACK ----------------------------------\n");
        int result;
        int finalResult=1;
        for(j=0;j<addedVertexes;j++){
            //printf("-----------------------------------NEW CALL FROM ROOT \n");

            // try to find coloring with next vertex from stack
            result = findColoringRecursive(front(stack1), firstNode, graphColors, usedColors);
            // TODO if colored without conflict and stack is empty - end with success
            if(result == 0){
                finalResult=0;
                break;
                // for now END - return 0 - couldnt find coloring
            }
            // remove vertex from stack
            pop(&stack1);
            // TODO --- do while stack is not empty loop ...
        }
        addedVertexes=0;
        if(finalResult == 1){
            // we found coloring
            found = 1;
            break;
        }
        if(i==allPermutationsCount-1){
            //printf("There is no other permutation!! --- undo completely \n");

        }else{
            //printf("Try another permutation!! --- undo completely before \n");
        }
        undo(myRow, firstNode, graphColors, usedColors);
        emptyStack(&stack1);

    }

    if(found == 0){
        printf("................. We found NOTHING :-\\ !! ...................... \n");
        //showStack(stack1);
        printf("SUM of function4 call: %d \n", count);
        free(allPermutations);
        free(usedColors);
        free(myRow);
        return 0;
    }else if(found==1){
        printf("-------------- We found something!! ---------------- \n");
        printf("SUM of function4 call: %d \n", count);
        free(allPermutations);
        free(usedColors);
        free(myRow);
        return 1;
    }

}




