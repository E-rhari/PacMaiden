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
    int gCost;  // distancia de manhathan a partir do inicio
    int hCost;  // um chute da distância de manhatan até o fim
    int fCost;  // uma heuristica pra prioziar os caminhos. gCost + fCost
    struct TNode* parent; // !!!!!! O caminho é construido através da hierarquia dos parents !!!!! 
} Node;


typedef struct TNodeElement {
    Node node;
    struct TNodeElement* next;
} NodeListElement;


typedef struct {
    NodeListElement* start;
    int size;
} NodeList;


void innitNode(Node* node,  GridVector nodePosition, GridVector start, GridVector end){
    node->position = nodePosition;

    node->gCost = abs(start.x - end.x) + abs(start.y - end.y); // Distância de Manhatan
    node->hCost = abs(nodePosition.x - end.x) + abs(nodePosition.y - end.y);
    node->fCost = node->hCost + node->gCost;
}


void innitNodeListElement(NodeListElement* element, GridVector nodePosition, GridVector start, GridVector end){
    element->next = NULL;
    innitNode(&element->node, nodePosition, start, end);
    element->node.parent = NULL;
}


void innitNodeList(NodeList* list){
    list->start = NULL;
    list->size = 0;
}


bool insertIntoNodeList(NodeList* list, NodeListElement* element, int index){
    if(index<0 || index > list->size)
        return false;
    
    NodeListElement* newElement = (NodeListElement*) malloc(sizeof(NodeListElement));
        
    newElement = element;

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


bool insertIntoEndOfNodeList(NodeList* list, NodeListElement* element){
    return insertIntoNodeList(list, element, 0);
}


bool innitInsideNodeList(NodeList* list, int index, GridVector nodePosition, GridVector start, GridVector end){
    NodeListElement* element = (NodeListElement*) malloc(sizeof(NodeListElement));
    innitNodeListElement(element, nodePosition, start, end);
    insertIntoNodeList(list, element, index);
}


Node* getFromNodeList(NodeList* list, int index){
    if(index<0 || index > list->size)
        return NULL;
    
    NodeListElement* current = list->start;
    for(int i=0; i<index; i++)
        current = current->next;
    return &current->node;
}

bool removeIndexFromNodeList(NodeList* list, int index){
    if(index<0 || index > list->size)
        return false;
    
    NodeListElement* elToRemove;
    if(index == 0){
        elToRemove = list->start;
        list->start = list->start->next;
    }
    else{
        NodeListElement* current = list->start;
        for(int i=0; i<index-1; i++)
            current = current->next;
        elToRemove = current->next;
        current->next = elToRemove->next;
    }
    free(elToRemove);
    list->size--;
    return true;
}


bool removeNodeFromNodeList(NodeList* list, Node* node){
    for(int i=0; i<list->size; i++)
        if(getFromNodeList(list, i) == node)
            return removeIndexFromNodeList(list, i);
    return false;
}


void printNodeList(NodeList list){
    NodeListElement* currentElement = list.start;
    printf("NodeList:[");

    printf("\n\t\tPosition\tgCost\thCost\tHeuristc");
    while(currentElement != NULL){
        printf("\n\t\t(%d, %d)", currentElement->node.position.x, currentElement->node.position.y);
        printf("\t\t%d", currentElement->node.gCost);
        printf("\t%d", currentElement->node.hCost);
        printf("\t%d", currentElement->node.fCost);
        currentElement = currentElement->next;
    }
    printf("\n\t ]");
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

    printf("Start: (%d, %d)\n", pathStart.x, pathStart.y);
    printf("Start: (%d, %d)\n", pathEnd.x, pathEnd.y);
    printf("\n");

    innitInsideNodeList(&list, 0, (GridVector){1,1}, pathStart, pathEnd);
    innitInsideNodeList(&list, 0, (GridVector){3,1}, pathStart, pathEnd);
    innitInsideNodeList(&list, 0, (GridVector){1,2}, pathStart, pathEnd);

    printNodeList(list);
    printf("\n\n");

    printf("Best Node: (%d, %d)\n", getBestNode(&list)->position.x, getBestNode(&list)->position.y);
}