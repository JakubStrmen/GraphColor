//
// Created by jakub on 6/8/18.
//

#ifndef GRAPHCOLOR_VERTEX_H
#define GRAPHCOLOR_VERTEX_H


class Vertex {
#define MAX_DEGREE = 3

public:
    Vertex();

    virtual ~Vertex();


    Vertex* neighbors[MAX_DEGREE];


};


#endif //GRAPHCOLOR_VERTEX_H
