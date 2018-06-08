//
// Created by jakub on 3/24/18.
//

#ifndef GRAPHCOLOR_DEPTHFIRST_H
#define GRAPHCOLOR_DEPTHFIRST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct resultFromColoring
{
    int result;
    short int *outputGraph;
}RESULTFROMCOLORING;
/**
 * Define functions
 */
void permute(short int*, int, int);
void undo(short int *, int, short int *, short int *);
void undoGraph(short int *, short int *, short int *, short int *);
int findColoring(int);
int findColoringRecursive(int, int, short int *, short int *);
int findColoringSequential(int);
RESULTFROMCOLORING depthFirstSearch(int, short int *, int);


#ifdef __cplusplus
}
#endif


#endif //GRAPHCOLOR_DEPTHFIRST_H
