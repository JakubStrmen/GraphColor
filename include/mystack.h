//
// Created by jakub on 3/24/18.
//

#ifndef GRAPHCOLOR_MYSTACK_H
#define GRAPHCOLOR_MYSTACK_H


typedef struct atom
{
    int value;
    struct atom *next;
}ATOM;

typedef struct mystack
{

    ATOM *first, *last;

}MYSTACK;

void ini(MYSTACK *r);
void push(MYSTACK *r, int value);
void pop(MYSTACK *r);
int front(MYSTACK r);
void emptyStack(MYSTACK *stack);
int findInStack(MYSTACK stack, int value);
void showStack(MYSTACK stack);


#endif //GRAPHCOLOR_MYSTACK_H
