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
#include "../include/mystack.h"


void ini(MYSTACK *stack)
{
    stack -> last = (ATOM*)malloc(sizeof(ATOM));
    // stack -> last -> value = NULL;
    stack -> last -> next = NULL;
    stack -> first = stack -> last;
}
void push(MYSTACK *stack, int x)
{
    //stack -> last -> value = (int*)malloc(sizeof(int));
    ATOM *temp;
    temp = (ATOM*)malloc(sizeof(ATOM));
    temp -> value = x;
    temp -> next = stack -> first;
    stack -> first = temp;
}
int empty(MYSTACK stack)
{
    return stack.first -> next ==NULL;
}
void pop(MYSTACK *stack)
{
    if(!empty(*stack))
    {
        ATOM *temp;
        temp = stack->first;
        stack -> first = stack -> first ->next;
        //free((void*)pom->value);
        free((void*)temp);
    }
}
int front(MYSTACK stack)
{
    if(!empty(stack))
    {
        return stack.first->value;
    }else{printf("chyba \n");}
}

int findInStack(MYSTACK stack, int value){
    ATOM *temp;
    temp = stack.first;
    if(empty(stack))
    {
        return 0;
    }


    while(temp ->next != NULL){
        if(temp->value == value){
            return 1;
        }
        temp = temp->next;
    }

    //free((void*)temp);
    return 0;
}


void showStack(MYSTACK stack){
    ATOM *temp;
    temp = stack.first;
    while(temp != stack.last){
        printf("%d, ", temp->value);
        temp = temp->next;
    }
    printf("\n");

}

void emptyStack(MYSTACK *stack){
    while(!empty(*stack)){
        pop(stack);
    }
}

