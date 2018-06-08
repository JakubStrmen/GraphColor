//
// Created by js on 4/3/18.
//

#ifndef GRAPHCOLOR_BREADTHFIRST2_H
#define GRAPHCOLOR_BREADTHFIRST2_H

#include "Graph.h"


class BreadthFirst : public Graph {
public:
    BreadthFirst(short int *, int);
    BreadthFirst(const BreadthFirst& orig);
    virtual ~BreadthFirst();

    int findColoring(short int);
    int findColoringRecursive(short int);

    std::vector<short int> findUncoloredEdges(short int vertex);

protected:
    void colorEdge(short fromVertex, short toVertex, short color);
    void showUsedColors();

private:
    short int *usedColors;
    short int *graphColors;
    short int allPermutationsCount;
    short int myColors[3];
};


#endif //GRAPHCOLOR_BREADTHFIRST2_H
