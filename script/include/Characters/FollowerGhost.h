#include "raylib.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include "math.h"

// #include "./Character.h"
// #include "./PacMaiden.h"
// #include "../Map/Map.h"

// #pragma once

typedef struct {
    int x;
    int y;
} GridVector;


typedef struct TNode {
    GridVector position;
    int distanceFromStart;
    int aproximateDistanceFromEnd;
    int heuristicGuess;

    struct TNode* parent; // !!!!!! O caminho é construido através da hierarquia dos parents !!!!! 
} Node;


typedef struct TGridStep {
    Node node;
    struct TGridStep* next;
} NodeListElement;


typedef struct {
    NodeListElement* start;
    int size;
} NodeList;


int innitGridStep(NodeListElement* element, GridVector elementPosition, GridVector start, GridVector end){
    element->next = NULL;

    element->node.position = elementPosition;

    element->node.distanceFromStart = abs(start.x - end.x) + abs(start.y - end.y); // Distância de Manhatan
    element->node.aproximateDistanceFromEnd = abs(elementPosition.x - end.x) + abs(elementPosition.y - end.y);
    element->node.heuristicGuess = element->node.aproximateDistanceFromEnd + element->node.distanceFromStart;
    element->node.parent = NULL;
}


void innitNodeList(NodeList* list){
    list->start = NULL;
    list->size = 0;
}


bool insertIntoNodeList(NodeList* list, NodeListElement* step, int index){
    if(index<0 || index > list->size)
        return false;
    
    NodeListElement* newElement = (NodeListElement*) malloc(sizeof(NodeListElement));
        
    newElement = step;

    if(index == 0){
        newElement->next = list->start;
        list->start = newElement;
    }
    else{
        NodeListElement* aux = list->start;
        for(int i=0; i<index-1; i++)
            aux = aux->next;
        newElement->next = aux->next;
        aux->next = newElement;
    }

    list->size++;
    return true;
}


bool insertIntoEndOfNodeList(NodeList* list, NodeListElement* step){
    return insertIntoNodeList(list, step, 0);
}


bool innitInsideList(NodeList* list, int index, GridVector elementPosition, GridVector start, GridVector end){
    NodeListElement* step = (NodeListElement*) malloc(sizeof(NodeListElement));
    innitGridStep(step, elementPosition, start, end);
    insertIntoNodeList(list, step, index);
}


void printNodeList(NodeList list){
    NodeListElement* currentStep = list.start;
    printf("NodeList: [");

    printf("\n\t\tPosition\tgCost\thCost\tHeuristc");
    while(currentStep != NULL){
        printf("\n\t\t(%d, %d)", currentStep->node.position.x, currentStep->node.position.y);
        printf("\t\t%d", currentStep->node.distanceFromStart);
        printf("\t%d", currentStep->node.aproximateDistanceFromEnd);
        printf("\t%d", currentStep->node.heuristicGuess);
        currentStep = currentStep->next;
    }
    printf("\n]");
}


// NodeList findPath(GridVector start, GridVector end, Map map){
//     // NodeList openList;   innitNodeList(&openList);
//     // NodeList closedList; innitNodeList(&closedList);

//     // while(openList.size != 0){
//     //     GridStep step = *openList.start;

//     // }

// }


int main(){
    GridVector pathStart = {0,0};
    GridVector pathEnd = {4,3};

    NodeList list;
    innitNodeList(&list);

    innitInsideList(&list, 0, (GridVector){1,1}, pathStart, pathEnd);
    innitInsideList(&list, 0, (GridVector){3,1}, pathStart, pathEnd);
    innitInsideList(&list, 0, (GridVector){4,2}, pathStart, pathEnd);

    printNodeList(list);
    printf("\n");
}